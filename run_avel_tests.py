#!/usr/bin/python3

import os
import sys
import shutil
import re
import copy
from itertools import product

# Path to compilers and names used for their build directories
compilers = [
    ['/usr/bin/g++-12', 'gnu-12'],
    #['/usr/bin/g++-11', 'gnu-11'],
    #['/usr/bin/g++-10', 'gnu-10'],
    #['/usr/bin/g++-9',  'gnu-9'],
    #['/usr/bin/clang++-14', 'clang-14'],
    #['/usr/bin/clang++-13', 'clang-13'],
    #['/usr/bin/clang++-12', 'clang-12'],
    #['/usr/bin/clang++-11', 'clang-11'],
    #['/opt/intel/oneapi/compiler/2023.0.0/linux/bin/icpx', 'icpx-2023.0.0']
]

# Map associating feature macros with their implied features, according to AVEL
features_x86 = {
    'AVEL_X86':             ['',                  []],
    'AVEL_POPCNT':          ['-mpopcnt',          ['AVEL_X86']],
    'AVEL_LZCNT':           ['-mlzcnt',           ['AVEL_X86']],
    'AVEL_BMI':             ['-mbmi',             ['AVEL_X86']],
    'AVEL_BMI2':            ['-mbmi2',            ['AVEL_BMI']],
    'AVEL_SSE2':            ['-msse2',            ['AVEL_X86']],
    'AVEL_SSE3':            ['-msse3',            ['AVEL_SSE2']],
    'AVEL_SSSE3':           ['-mssse3',           ['AVEL_SSE3']],
    'AVEL_SSE41':           ['-msse4.1',          ['AVEL_SSSE3', 'AVEL_POPCNT']],
    'AVEL_SSE42':           ['-msse4.2',          ['AVEL_SSE41']],
    'AVEL_AVX':             ['-mavx',             ['AVEL_SSE42']],
    'AVEL_AVX2':            ['-mavx2',            ['AVEL_AVX']],
    'AVEL_FMA':             ['-mfma',             ['AVEL_AVX']],
    'AVEL_AVX512F':         ['-mavx512f',         ['AVEL_AVX2', 'AVEL_FMA']],
    'AVEL_AVX512BW':        ['-mavx512bw',        ['AVEL_AVX512F']],
    'AVEL_AVX512DQ':        ['-mavx512dq',        ['AVEL_AVX512F']],
    'AVEL_AVX512VL':        ['-mavx512vl',        ['AVEL_AVX512F']],
    'AVEL_AVX512CD':        ['-mavx512cd',        ['AVEL_AVX512F']],
    'AVEL_AVX512VPOPCNTDQ': ['-mavx512vpopcntdq', ['AVEL_AVX512F']],
    'AVEL_AVX512BITALG':    ['-mavx512bitalg',    ['AVEL_AVX512F']],
    'AVEL_AVX512VBMI':      ['-mavx512vbmi',      ['AVEL_AVX512F']],
    'AVEL_AVX512VBMI2':     ['-mavx512vbmi2',     ['AVEL_AVX512F']],
    'AVEL_GFNI':            ['-mgfni',            ['AVEL_AVX512F']]
}

feature_arm = {
    'AVEL_ARM':     ['', []],
    'AVEL_NEON':    ['', ['AVEL_ARM']],
    'AVEL_ARRCH64': ['', ['AVEL_ARM']]
}

target_features = features_x86

compiler_macros = [
    'AVEL_GCC',
    'AVEL_CLANG',
    'AVEL_ICPX'
]


# def extract_features(line: str):
#    # If the condition is more complex than an intersection of features, it's
#    # currently ok to skip the specific combination as they'll be handled via
#    # other test configurations
#    if '||' in line or '!' in line:
#        return tuple()
#
#    used_features = [macro for macro in feature_macros if ('defined(' + macro + ')') in line]
#    used_features = sorted(used_features)
#    return tuple(used_features)


def compiler_path_to_macro(compiler_path):
    if 'g++' in compiler_path:
        return 'AVEL_GCC'
    elif 'clang++' in compiler_path:
        return 'AVEL_CLANG'
    elif 'icpx' in compiler_path:
        return 'AVEL_ICPX'
    else:
        print('Unrecognized compiler name')
        exit(1)


# Converts a preprocessor line to a boolean expression which can be evaluated
# via the eval() function
#
# Returns a tuple containing the boolean expression as a string and a list of
# strings which contain the names of the variables used in the expression
#
def construct_expression(line):
    line = line.replace('#if', '')
    line = line.replace('#elif', '')

    line = line.replace('||', 'or')
    line = line.replace('&&', 'and')

    line = line.replace('|', 'or')
    line = line.replace('&', 'and')

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
    for test_group_name, value  in test_groups.items():
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

    #return False

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

    run_command = './test_build_dirs/{}/tests/AVEL_TESTS'.format(build_dir_name)

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
    # TODO: Tidy this up
    for assignment in assignments:
        new_assignment = {}
        for variable, value in assignment.items():
            new_assignment[variable] = value
            if value == 'True':
                for implied_feature in target_features[variable][1]:
                    new_assignment[implied_feature] = 'True'

        ret.append(new_assignment)

    return ret


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

    failed = False
    for partial_solution in expression_solutions:
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
        failed |= run_test(compiler_path, build_dir_name, full_variable_assignments, test_groups)
        if failed:
            return failed

        # Remove feature combinations that were satisfied by the previous test
        for vec_name in names_and_features:
            feature_map = names_and_features[vec_name]

            names_and_features[vec_name] = \
                {k: v for k, v in feature_map.items() if not evaluate_expression(k, full_variable_assignments)}

        # Remove vectors which have no more feature combinations to check for
        #names_and_features = {k: v for k, v in names_and_features.items() if len(v) > 0}

    return failed


def main():
    global target_features

    if len(sys.argv) > 1:
        if sys.argv[1] == 'x86':
            target_features = features_x86
        elif sys.argv[1] == 'arm':
            target_features = feature_arm
        else:
            print('Unrecognized command line parameter: ', sys.argv[1])
            exit(1)

    else:
        print('Pass "x86" or "arm" as a parameter')
        exit(1)

    failed = False

    # Unroll feature implications
    for key in target_features:
        implications = target_features[key][1]
        for implication in implications:
            implied_features = target_features[implication][1]
            target_features[key][1] += implied_features

    # Identify features used by vector implementations
    names_and_features = identify_vector_features()

    # Run tests
    for (path, build_dir_name) in compilers:
        failed |= test_on_compiler(path, build_dir_name, copy.deepcopy(names_and_features))
        if failed:
            print("\nTesting script: Encountered failed tests")
            return

    print("\nTesting script: All tests passed")


if __name__ == '__main__':
    main()
