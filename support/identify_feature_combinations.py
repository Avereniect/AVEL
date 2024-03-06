#!/usr/bin/python3

# Script that identifies the features which have been used by AVEL in its
# implementation, so we know which feature combinations to check for in the
# run_avel_tests.py script

import os

feature_macros = [
    'AVEL_X86',
    'AVEL_POPCNT',
    'AVEL_BMI',
    'AVEL_BMI2',
    'AVEL_SSE2',
    'AVEL_SSE3',
    'AVEL_SSSE3',
    'AVEL_SSE4_1',
    'AVEL_SSE4_2',
    'AVEL_AVX',
    'AVEL_AVX2',
    'AVEL_FMA',
    'AVEL_AVX512F',
    'AVEL_AVX512BW',
    'AVEL_AVX512DQ'
    'AVEL_AVX512VL'
    'AVEL_AVX512CD'
    'AVEL_AVX512VPOPCNTDQ'
    'AVEL_AVX512BITALG'
    'AVEL_AVX512VBMI'
    'AVEL_AVX512VBMI2'
    'AVEL_GFNI'
]


root_dir = '../include/avel'

file_counter = 0


def is_directive(line):
    if 'AVEL_' in line and "#ifndef" not in line and "#define" not in line:
        return True

    return False


def extract_features(line):
    used_features = []

    for macro in feature_macros:
        if macro in line:
            used_features.append(macro)

    return used_features, line.strip().lstrip('#elif').lstrip('#if').lstrip('#ifdef')


def process_file(path):
    global file_counter
    file_counter += 1
    file = open(path, 'r')
    lines = file.readlines()
    file.close()

    feature_combinations = []
    preprocessor_lines = {}

    for line in lines:
        if is_directive(line):
            a, b = extract_features(line)
            feature_combinations.append(a)
            if a:
                preprocessor_lines.update([[b, 1]])

    return feature_combinations, preprocessor_lines


def main():
    queue = [root_dir]

    feature_combinations = []
    preprocessor_lines = {}

    while len(queue) != 0:
        curr = queue.pop()
        if os.path.isfile(curr) and not curr.endswith('Verify_capabilities.hpp'):
            a, b = process_file(curr)
            feature_combinations.append(a)
            preprocessor_lines.update(b)
        elif os.path.isdir(curr):
            queue += [curr + '/' + p for p in os.listdir(curr)]

    print(file_counter)
    print('\n'.join(sorted(preprocessor_lines.keys())))


if __name__ == "__main__":
    main()
