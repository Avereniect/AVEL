#!/usr/bin/python3
"""
Testing script which is used to run AVEL's tests.

Identifies CPU feature combinations used within the avel/impl/vector folder and
only enables the relevant tests to cut down on testing times

Example usages:
Run all tests for x86 CPUs: run_avel_tests.py -A"x86"
Run all tests for ARM CPUs: run_avel_tests.py -A"x86"
Run all tests for x86 CPUs using Intel SDE as launcher: run_avel_tests -A"x86" -L"sde64 -- <exec>"
Run tests for x86 CPU's, starting on the second compiler, on the third case: run_avel_tests -A"x86" -C"1:2"

"""

import os
import sys
import shutil
import re
import copy
from itertools import product

"""
Path to compilers and names used for their build directories
"""
compilers = [
    # ['/usr/bin/g++-12', 'gnu-12'],
    # ['/usr/bin/g++-11', 'gnu-11'],
    # ['/usr/bin/g++-10', 'gnu-10'],
    ['/usr/bin/g++-9',  'gnu-9'],
    # ['/usr/bin/clang++-14', 'clang-14'],
    # ['/usr/bin/clang++-13', 'clang-13'],
    # ['/usr/bin/clang++-12', 'clang-12'],
    # ['/usr/bin/clang++-11', 'clang-11'],
    # ['/opt/intel/oneapi/compiler/2023.0.0/linux/bin/icpx', 'icpx-2023.0.0']
]

"""
Map associating feature macros with their implied features, according to AVEL
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

compiler_macros = [
    'AVEL_GCC',
    'AVEL_CLANG',
    'AVEL_ICPX'
]

"""
Map containing command line parameters
"""
parameters = {}


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
        print('Unrecognized compiler name')
        exit(1)


def construct_expression(line):
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

        (expression, variables) = construct_expression(line)

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
    queue = [file_name for file_name in queue if ('x' in file_name and 'f' not in file_name)]
    # TODO: Remove the test for files containing f once implementations of float vectors are added

    names_and_features = {}
    for file_name in queue:
        file_path = vector_implementation_path + file_name
        vector_name = file_name.removesuffix('.hpp').lower()

        names_and_features[vector_name] = identify_feature_combinations(file_path)

    return names_and_features


def run_test(compiler_path, build_dir_name, feature_assignments, test_groups):
    print('\nRunning test:')
    print('Flags:')
    for variable, value in feature_assignments.items():
        if value == 'True':
            print(target_features[variable][0])

    print()
    print('Feature variables:')
    for variable, value in feature_assignments.items():
        if value == 'True':
            print(variable)

    print()
    print('Test group variables:')
    for test_group_name, value in test_groups.items():
        if value:
            variable = 'AVEL_ENABLE_' + test_group_name.upper() + '_TESTS'
            print(variable)

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
    print(cmake_command)

    ret = os.system(cmake_command)
    if ret != 0:
        print()
        print("Testing script: ")
        print("Testing failed for")
        print("Compiler:", compiler_path)
        print("Compiler flags:", flags)
        print("CMake Variables:", cmake_variables)
        failed = True
        return failed

    make_command = 'make AVEL_TESTS -C ./test_build_dirs/{}'.format(build_dir_name)
    ret = os.system(make_command)
    if ret != 0:
        print()
        print("Testing script: ")
        print("Testing failed for")
        print("Compiler:", compiler_path)
        print("Compiler flags:", flags)
        print("CMake Variables:", cmake_variables)
        failed = True
        return failed

    run_command = ''
    if 'launcher' not in parameters or parameters['launcher'] == '':
        run_command = './test_build_dirs/{}/tests/AVEL_TESTS'.format(build_dir_name)
    elif '<exec>' in parameters['launcher']:
        run_command = parameters['launcher'].replace('<exec>', './test_build_dirs/{}/tests/AVEL_TESTS'.format(build_dir_name))
    else:
        run_command = parameters['launcher'] + ' ./test_build_dirs/{}/tests/AVEL_TESTS'.format(build_dir_name)

    ret = os.system(run_command)
    if ret != 0:
        print()
        print("Testing script: ")
        print("Testing failed for")
        print("Compiler:", compiler_path)
        print("Compiler flags:", flags)
        print("CMake Variables:", cmake_variables)
        failed = True
        return failed

    shutil.rmtree(build_path)

    print()

    return False


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


def test_on_compiler(compiler_index, starting_case, compiler_path, build_dir_name, names_and_features):
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

    count = 0
    for partial_solution in expression_solutions:
        if count < starting_case:
            count += 1
            continue

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
        failed = run_test(compiler_path, build_dir_name, full_variable_assignments, test_groups)
        if failed:
            print('Failed on test case', compiler_index, ':', count)
            return failed

        # Remove feature combinations that were satisfied by the previous test
        for vec_name in names_and_features:
            feature_map = names_and_features[vec_name]

            names_and_features[vec_name] = \
                {k: v for k, v in feature_map.items() if not evaluate_expression(k, full_variable_assignments)}

        count += 1

    return False


def parse_command_line_arguments(arguments):
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

        if argument.startswith('-C'):
            tail = argument.lstrip('-C')
            results['case'] = tail
            continue

        print('Unrecognized parameter: ', argument)
        exit(1)

    global parameters
    parameters = results


def main():
    parse_command_line_arguments(sys.argv[1:])

    if not parameters:
        print("Recognized parameters:")
        print("  -A    (Required) The target architecture to run tests on. Should be either \'x86\' or \'arm\'")
        print("  -L    (Optional) Command which is invoked to run executable. If this parameter contains \'<exec>\' t"
              "en it will be replaced with the path of the test executable. Otherwise, the path of test executable "
              "will be appended to the end of this parameter. If this parameter is not specified, the test executable "
              "is invoked directly.")
        print("  -C    (Optional) The test case to start from. Should be a pair of integer values seperated by a colon."
              " Values are printed out when the test script runs into issues. This parameter allows tests which have "
              "passed in previous runs to be skipped. If this parameter is not specified, result is equivalent to "
              "\'0:0\'.")

        exit(1)

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

    # Identify test case
    starting_compiler_index = 0
    starting_test_case_index = 0

    if 'case' in parameters:
        case = parameters['case']
        colon_index = case.find(':')
        if colon_index == -1:
            print('Test case parameter should be two integers delimited by a \':\'')
            exit(1)

        compiler_index_string = case[0:colon_index]
        test_case_index_string = case[colon_index + 1:]

        try:
            starting_compiler_index = int(compiler_index_string)
        except ValueError:
            print('Test case parameter should be two integers delimited by a \':\'')
            exit(1)

        try:
            starting_test_case_index = int(test_case_index_string)
        except ValueError:
            print('Test case parameter should be two integers delimited by a \':\'')
            exit(1)

    # Unroll feature implications
    for key in target_features:
        implications = target_features[key][1]
        for implication in implications:
            implied_features = target_features[implication][1]
            target_features[key][1] += implied_features

    # Identify features used by vector implementations
    names_and_features = identify_vector_features()

    # Run tests
    for (compiler_index, compiler) in enumerate(compilers):
        if compiler_index < starting_compiler_index:
            continue

        (compiler_path, build_directory) = compiler

        test_case_index = starting_test_case_index if compiler_index == compiler_index else 0

        failed = test_on_compiler(
            compiler_index,
            test_case_index,
            compiler_path,
            build_directory,
            copy.deepcopy(names_and_features)
        )

        if failed:
            print("\nTesting script: Testing script did not complete")
            return

    print("\nTesting script: All tests passed")


if __name__ == '__main__':
    main()
