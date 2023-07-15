#!/usr/bin/env python

"""
Testing script which is used to run AVEL's tests.

Identifies CPU feature combinations used within the avel/impl/vector folder and
only enables the relevant tests to cut down on testing times

Example usages:
Run all tests for x86 CPUs: run_avel_tests.py -A"x86" -C"/usr/bin/g++-9"
Run all tests for ARM CPUs: run_avel_tests.py -A"arm" -C"/usr/bin/g++-9"
Run all tests for x86 CPUs using Intel SDE: run_avel_tests -A"x86" -C"/usr/bin/g++-9" -L"sde64 -- <exec>"

"""

import os
import sys
import shutil
import re
import copy

from math import floor
from multiprocessing import cpu_count
from threading import Thread, Lock, Semaphore
from itertools import product
from functools import reduce

"""
Map associating feature macros with their directly implied features, according to AVEL
"""
features_x86 = {
    'AVEL_X86': ['', []],
    'AVEL_POPCNT': ['-mpopcnt', ['AVEL_X86']],
    'AVEL_LZCNT': ['-mlzcnt', ['AVEL_X86']],
    'AVEL_BMI': ['-mbmi', ['AVEL_X86']],
    'AVEL_BMI2': ['-mbmi2', ['AVEL_BMI']],
    'AVEL_SSE2': ['-msse2', ['AVEL_X86']],
    'AVEL_SSE3': ['-msse3', ['AVEL_SSE2']],
    'AVEL_SSSE3': ['-mssse3', ['AVEL_SSE3']],
    'AVEL_SSE41': ['-msse4.1', ['AVEL_SSSE3', 'AVEL_POPCNT']],
    'AVEL_SSE42': ['-msse4.2', ['AVEL_SSE41']],
    'AVEL_AVX': ['-mavx', ['AVEL_SSE42']],
    'AVEL_AVX2': ['-mavx2', ['AVEL_AVX']],
    'AVEL_FMA': ['-mfma', ['AVEL_AVX']],
    'AVEL_AVX512F': ['-mavx512f', ['AVEL_AVX2', 'AVEL_FMA']],
    'AVEL_AVX512BW': ['-mavx512bw', ['AVEL_AVX512F']],
    'AVEL_AVX512DQ': ['-mavx512dq', ['AVEL_AVX512F']],
    'AVEL_AVX512VL': ['-mavx512vl', ['AVEL_AVX512F']],
    'AVEL_AVX512CD': ['-mavx512cd', ['AVEL_AVX512F']],
    'AVEL_AVX512VPOPCNTDQ': ['-mavx512vpopcntdq', ['AVEL_AVX512F']],
    'AVEL_AVX512BITALG': ['-mavx512bitalg', ['AVEL_AVX512F']],
    'AVEL_AVX512VBMI': ['-mavx512vbmi', ['AVEL_AVX512F']],
    'AVEL_AVX512VBMI2': ['-mavx512vbmi2', ['AVEL_AVX512F']],
    'AVEL_GFNI': ['-mgfni', ['AVEL_AVX512F']]
}

"""
Map associating feature macros with their implied features, according to AVEL
"""
features_arm = {
    'AVEL_ARM': ['', []],
    'AVEL_NEON': ['', ['AVEL_ARM']],
    'AVEL_AARCH64': ['', ['AVEL_ARM']]
}

target_features = features_x86

"""
List of AVEL's compiler macros
"""
compiler_macros = [
    'AVEL_GCC',
    'AVEL_CLANG',
    'AVEL_ICPX'
]

"""
Dictionary containing command line parameters
"""
parameters = {}

"""
Path to compilers and names used for their build directories
"""
compilers = []

"""
List that is used to store status codes produced by test cases after running
"""
test_exit_codes = []

"""
List that is used to store information about tests cases to print out in case of failure
"""
test_run_infos = []


def unroll_feature_implications():
    """
    Applies transitive implications to the target_features map, making each feature contains its indirect implications
    in addition to its direct implications
    """
    for key in target_features:
        implications = target_features[key][1]
        for implication in implications:
            implied_features = target_features[implication][1]
            target_features[key][1] += implied_features


def compiler_path_to_macro(compiler_path):
    """
    Attempts to convert a compiler's path the AVEL macro corresponding to that
    compiler
    :param compiler_path: Path to compiler
    :return: string containing macro that AVEL uses to indicate which compiler
    is being used
    """

    if 'g++' in compiler_path:
        return 'AVEL_GCC'
    elif 'clang++' in compiler_path:
        return 'AVEL_CLANG'
    elif 'icpx' in compiler_path:
        return 'AVEL_ICPX'
    else:
        print('Compiler not recognized as g++, clang++, or icpx.')
        exit(1)


def macro_to_boolean_expression(line):
    """
    Converts a preprocessor line to a boolean expression which can be evaluated
    via the eval() function

    :param line: A string containing a line from the AVEL code base which checks
    if a certain combination of features are satisfied
    :return: A tuple containing the boolean expression as a string and a list of
    strings which contain the names of the variables used in the expression
    """

    line = line.replace('#if', '')
    line = line.replace('#elif', '')

    line = line.replace('||', ' or ')
    line = line.replace('&&', ' and ')

    line = line.replace('|', ' or ')
    line = line.replace('&', ' and ')

    line = line.replace('__cplusplus', '201103')

    var_list = []
    while True:
        match = re.search(r'defined\(([A-Z0-9_]+)\)', line)
        if not match:
            break

        match = match.group()

        var_name = match.removeprefix('defined(').removesuffix(')')
        var_list.append(var_name)
        line = line.replace(match, var_name)

    line = line.strip()
    return tuple([line, var_list])


def substitute_variable(expression, variable_name, value):
    """

    :param expression:
    :param variable_name:
    :param value:
    :return:
    """
    search_pattern = r'\b' + variable_name + r'\b'
    return re.sub(search_pattern, value, expression)


def substitute_variables(expressions, variable_assignments):
    if expressions is list:
        return [substitute_variables(expression, variable_assignments) for expression in expressions]
    else:
        for variable, value in variable_assignments.items():
            expressions = substitute_variable(expressions, variable, value)

        return expressions


def evaluate_expression(expression, variable_assignments):
    return eval(substitute_variables(expression, variable_assignments))


def find_variable_assignment(expression, variable_list):
    num_vars = len(variable_list)
    possible_solutions = [seq for seq in product(('True', 'False'), repeat=num_vars)]

    # Prefer solutions which enable the smallest number of features
    possible_solutions = sorted(possible_solutions, key=lambda x: len(x))

    assignments = {}
    for solution in possible_solutions:
        assignments.update(zip(variable_list, solution))
        if evaluate_expression(expression, assignments):
            return assignments

    return None


def identify_feature_combinations(file_path):
    file = open(file_path, 'r')
    lines = file.readlines()
    file.close()

    def is_feature_check_macro(l: str):
        return (('#if' in l) or ('#elif' in l)) and ('defined' in l) and ('AVEL' in l)

    expressions_and_variables = {}

    for line in lines:
        if not is_feature_check_macro(line):
            continue

        (expression, variables) = macro_to_boolean_expression(line)

        expressions_and_variables[expression] = variables

    # Remove lines that involve macros from other architectures
    to_remove = []
    for expression in expressions_and_variables:
        variables = expressions_and_variables[expression]

        for variable in variables:
            if variable not in compiler_macros and variable not in target_features.keys():
                to_remove.append(expression)
                break

    for expression in to_remove:
        del expressions_and_variables[expression]

    return expressions_and_variables


def identify_vector_features():
    vector_implementation_path = 'include/avel/impl/vectors/'

    directory_contents = os.listdir(vector_implementation_path)

    # Filter only files
    queue = [path for path in directory_contents if os.path.isfile(vector_implementation_path + path)]

    # Filter only vector headers
    queue = [file_name for file_name in queue if ('x' in file_name)]

    names_and_features = {}
    for file_name in queue:
        file_path = vector_implementation_path + file_name
        vector_name = file_name.removesuffix('.hpp').lower()

        names_and_features[vector_name] = identify_feature_combinations(file_path)

    return names_and_features


def run_test_case(compiler_path, build_dir_name, feature_assignments, test_groups, test_index):
    flags = '-w -std=c++11'
    cmake_variables = ''

    # Set flags and variables based on features
    for variable, value in feature_assignments.items():
        if value == 'True':
            cmake_variables += ' ' + "-D" + variable + ':BOOL=ON'
            flags += ' ' + target_features[variable][0]
        else:
            cmake_variables += ' ' + "-D" + variable + ':BOOL=OFF'

    # Set variables based on test groups
    for test_group_name, value in test_groups.items():
        test_group_name = 'AVEL_ENABLE_' + test_group_name.upper() + '_TESTS'
        if value:
            cmake_variables += ' ' + "-D" + test_group_name + ':BOOL=ON'
        else:
            cmake_variables += ' ' + "-D" + test_group_name + ':BOOL=OFF'

    build_path = "./test_build_dirs/" + build_dir_name

    cmake_command_format_string = \
        'cmake ' \
        '-S"./" ' \
        '-B{} ' \
        '-G"CodeBlocks - Unix Makefiles" ' \
        '-DCMAKE_CXX_COMPILER={} ' \
        '-DAVEL_BUILD_TESTS:BOOL=ON{} ' \
        '-DCMAKE_CXX_FLAGS="{}"'
    cmake_command = cmake_command_format_string.format(build_path, compiler_path, cmake_variables, flags)

    run_info = [None, None, None, None, None]
    test_run_infos[test_index] = run_info

    run_info[0] = flags
    run_info[1] = cmake_variables
    run_info[2] = cmake_command

    exit_code = os.system(cmake_command)
    if exit_code != 0:
        return

    make_command = 'make AVEL_TESTS -C ./test_build_dirs/{}'.format(build_dir_name)
    run_info[3] = make_command

    memory_semaphore.acquire()
    exit_code = os.system(make_command)
    memory_semaphore.release()

    if exit_code != 0:
        return

    run_command = ''
    if 'launcher' not in parameters or parameters['launcher'] == '':
        run_command = './test_build_dirs/{}/tests/AVEL_TESTS'.format(build_dir_name)
    elif '<exec>' in parameters['launcher']:
        run_command = parameters['launcher'].replace('<exec>',
                                                     './test_build_dirs/{}/tests/AVEL_TESTS'.format(build_dir_name))
    else:
        run_command = parameters['launcher'] + ' ./test_build_dirs/{}/tests/AVEL_TESTS'.format(build_dir_name)

    run_info[4] = run_command

    exit_code = os.system(run_command)
    if exit_code != 0:
        return

    shutil.rmtree(build_path)

    test_exit_codes[test_index] = True


def substitute_compiler_macros(compiler_macro, names_and_features):
    variable_assignments = dict(zip([macro for macro in compiler_macros], ['False'] * len(compiler_macros)))
    variable_assignments[compiler_macro] = 'True'

    for vec_names in names_and_features:
        features = names_and_features[vec_names]

        expressions = []
        for expression in features.keys():
            expressions.append(substitute_variables(expression, variable_assignments))

        variables = []
        for variable_list in features.values():
            variables.append([x for x in variable_list if x not in variable_assignments.keys()])

        names_and_features[vec_names] = dict(zip(expressions, variables))


def expand_assignments(assignments):
    ret = []
    # TODO: Replace with solution with less nesting
    for assignment in assignments:
        new_assignment = {}
        for variable, value in assignment.items():
            new_assignment[variable] = value
            if value == 'True':
                for implied_feature in target_features[variable][1]:
                    new_assignment[implied_feature] = 'True'

        ret.append(new_assignment)

    return ret


work_queue = []

work_queue_lock = Lock()

memory_semaphore = None


def thread_worker():
    while True:
        work_queue_lock.acquire()
        if not work_queue:
            work_queue_lock.release()
            return

        task = work_queue.pop(0)
        work_queue_lock.release()

        run_test_case(*task)


def test_on_compiler(compiler_path, build_dir_name, names_and_features):
    substitute_compiler_macros(compiler_path_to_macro(compiler_path), names_and_features)

    # Gather all expressions
    expressions_and_variables = {}
    for features in names_and_features.values():
        expressions_and_variables |= features

    # Find solutions for all expressions
    expression_solutions = []
    for expression, variables in expressions_and_variables.items():
        solution = find_variable_assignment(expression, variables)
        if solution is not None:
            expression_solutions.append(solution)

    # Add dummy to run tests without any special features enabled
    expression_solutions.insert(0, {})

    # Add implied features
    expression_solutions = expand_assignments(expression_solutions)

    # Remove any potential duplicate solutions
    expression_solutions = \
        [dict(t) for t in {tuple(expression_solutions[0].items()) for expression_solutions[0] in expression_solutions}]

    # Sort solutions by features
    feature_order = dict(zip(target_features.keys(), range(0, len(target_features))))
    expression_solutions = sorted(expression_solutions, key=lambda x: [feature_order[x] for x in x.keys()])

    run_test_parameters = []

    for index, partial_solution in enumerate(expression_solutions):
        # Construct feature combinations to test with
        full_variable_assignments = dict(zip(target_features.keys(), ['False'] * len(target_features)))
        for variable in partial_solution:
            full_variable_assignments[variable] = partial_solution[variable]

        # Identify vectors to test
        test_groups = {}
        for vec_name, expressions in names_and_features.items():
            test_groups[vec_name] = False
            for expression, variables in expressions.items():
                if evaluate_expression(expression, full_variable_assignments):
                    test_groups[vec_name] = True
                    break

        # Run tests with specified features enabled
        run_test_parameters.append([
            compiler_path,
            build_dir_name + '/' + str(index) + '/',
            full_variable_assignments,
            test_groups,
            index
        ])

        # Remove feature combinations that were satisfied by the previous test
        for vec_name in names_and_features:
            feature_map = names_and_features[vec_name]

            names_and_features[vec_name] = \
                {k: v for k, v in feature_map.items() if not evaluate_expression(k, full_variable_assignments)}

    # Create list of booleans based on test mask
    format_string = '#0' + str(2 + len(expression_solutions)) + 'b'
    test_enabled_list = [b == '0' for b in format(parameters['test_mask'], format_string)[2:][::-1]]

    # Compute number of builds to run in parallel based on memory contraints
    memory_bytes = os.sysconf('SC_PAGE_SIZE') * os.sysconf('SC_PHYS_PAGES')
    memory_gigs = memory_bytes / (1024 * 1024 * 1024)
    # 2 gigs for OS, 3 gigs for build. Values derived empirically from Ubuntu 22.10 machine
    safe_parallel_build_count = min(int(floor((memory_gigs - 2) / 3)), cpu_count())

    global memory_semaphore
    memory_semaphore = Semaphore(safe_parallel_build_count)

    # Launch a thread for each test case that is enabled
    global test_exit_codes
    test_exit_codes = [False] * len(expression_solutions)

    global test_run_infos
    test_run_infos = [None] * len(expression_solutions)

    # Enqueue test cases into work queue
    for params, test_enabled in zip(run_test_parameters, test_enabled_list):
        if test_enabled:
            work_queue.append(params)


    # Print Debug info
    print(
        'Testing script: Running {} tests on {} threads on machine with {:.3f}GiB of memory'.format(
            len(work_queue),
            cpu_count(),
            memory_gigs),
        flush=True
    )

    # Launch workers threads
    threads = []
    for i in range(0, cpu_count()):
        th = Thread(target=thread_worker, args=[])
        threads.append(th)
        th.start()

    # Wait for threads to complete their work
    for th in threads:
        th.join()

    # Print results of tests if any failed
    print('Testing script:')
    for result, run_infos in zip(test_exit_codes, test_run_infos):
        if result or not run_infos:
            continue

        print('Testing failed for:')
        print('Compiler:', compiler_path)
        print('Compiler flags: ', run_infos[0])
        print('CMake variables: ', run_infos[1])
        print('CMake command: ', run_infos[2])
        print('Make command: ', run_infos[3])
        print('run command: ', run_infos[4])
        print()

    is_run_successful = all(test_exit_codes)
    success_string = reduce(lambda byte, bit: byte * 2 + bit, test_exit_codes, 0)

    return is_run_successful, success_string


def parse_command_line_arguments(arguments):
    """
    Populates the global parameters map with the contents

    :param arguments: List of strings containing command line arguments passed to script
    :return: dictionary associating recognized parameters to values specified at command line
    """
    results = {}
    for argument in arguments:
        if argument.startswith('-A'):
            tail = argument.lstrip('-A')
            results['arch'] = tail
            continue

        if argument.startswith('-L'):
            tail = argument.lstrip('-L')
            results['launcher'] = tail
            continue

        if argument.startswith('-T'):
            tail = argument.lstrip('-T')
            results['test_start'] = tail
            continue

        if argument.startswith('-C'):
            tail = argument.lstrip('-C')
            results['compiler_list_str'] = tail
            continue

        print('Unrecognized parameter: ', argument)
        exit(1)

    global parameters
    parameters = results


def validate_command_line_arguments():
    # Handle case where script is invoked with no recognized parameters
    if not parameters:
        print('Recognized parameters:')
        print('  -A    (Required) The target architecture to run tests on. Should be either \'x86\' or \'arm\'')
        print('  -C    (Required) A colon-delimited list of paths to compilers to run the tests with.')
        print('  -L    (Optional) Command which is invoked to run executable. If this parameter contains \'<exec>\' t'
              'en it will be replaced with the path of the test executable. Otherwise, the path of test executable '
              'will be appended to the end of this parameter. If this parameter is not specified, the test executable '
              'is invoked directly.')
        print('  -T   (Optional) A pair of hexadecimal strings seperated by a color. Indicate which tests have '
              'previously passed, allowing the script to avoid rerunning tests that have completed in previous runs. '
              'The script will print out this value upon completion if any tests failed.')

        exit(1)

    # Handle arch argument
    if 'arch' not in parameters:
        print('Target architecture not specified!')
        print('Specify the -A parameter to be either \'x86\' or \'arm\'')
        exit(1)

    global target_features
    if parameters['arch'] == 'x86':
        target_features = features_x86
    elif parameters['arch'] == 'arm':
        target_features = features_arm
    else:
        print('Unrecognized architecture: ', parameters['arch'])
        exit(1)

    # Handle compiler paths argument
    if 'compiler_list_str' not in parameters:
        print('No compiler specified')
        exit(1)

    global compilers
    compilers = []

    compiler_paths = parameters['compiler_list_str'].split(':')
    for compiler_path in compiler_paths:
        compilers.append([compiler_path, str(hash(compiler_path))])

    # Handle test start argument
    if 'test_start' in parameters:
        delimited = parameters['test_start'].split(':')
        if len(delimited) != 2:
            print('Expected exactly one colon in parameter -T:{}'.format(parameters['test_start']))

        starting_compiler_index_str, test_mask_str = delimited

        try:
            starting_compiler_index = int(starting_compiler_index_str, 16)
            parameters['starting_compiler_index'] = starting_compiler_index

        except ValueError:
            print('-{} could not be parsed as a hexadecimal string'.format(starting_compiler_index_str))
            exit(1)

        try:
            test_mask = int(test_mask_str, 16)
            parameters['test_mask'] = test_mask

        except ValueError:
            print('{} could not be parsed as a hexadecimal string'.format(test_mask_str))
            exit(1)
    else:
        parameters['starting_compiler_index'] = 0
        parameters['test_mask'] = 0


def main():
    parse_command_line_arguments(sys.argv[1:])
    validate_command_line_arguments()

    unroll_feature_implications()

    # Identify features used by vector implementations
    vector_names_and_required_features = identify_vector_features()

    # Run tests
    for (compiler_index, compiler) in enumerate(compilers):
        if compiler_index < parameters['starting_compiler_index']:
            continue

        (compiler_path, build_directory) = compiler

        success, success_string = test_on_compiler(
            compiler_path,
            build_directory,
            copy.deepcopy(vector_names_and_required_features)
        )

        if not success:
            print()
            print('Testing script: Tests failed')
            print('Run again with 0x{:x}:0x{:x} to skip tests that have already passed'.format(
                compiler_index,
                success_string
            ))

            exit(1)

    print('\nTesting script: All tests passed')


if __name__ == '__main__':
    main()
