#!/usr/bin/python3

import os

# Path to compilers and names used for their build directories
compilers = [
    ['/usr/bin/g++-12', 'gnu-12'],
    ['/usr/bin/g++-11', 'gnu-11'],
    ['/usr/bin/g++-10', 'gnu-10'],
    ['/usr/bin/g++-9',  'gnu-9'],
    ['/usr/bin/clang++-14', 'clang-14'],
    ['/usr/bin/clang++-13', 'clang-13'],
    ['/usr/bin/clang++-12', 'clang-12'],
    ['/usr/bin/clang++-11', 'clang-11'],
    ['/opt/intel/oneapi/compiler/2023.0.0/linux/bin/icpx', 'icpx-2023.0.0']
]

feature_flag_combinations_x86 = [
    [''],
    ['-mpopcnt'],
    ['-mlzcnt'],
    ['-mbmi'],
    ['-mbmi2'],
    ['-msse2'],
    ['-msse3'],
    ['-mssse3'],
    ['-msse4.1'],
    ['-msse4.2'],
    ['-mavx'],
    ['-mavx2'],
    ['-mfma'],
    ['-mavx512f'],
    ['-mavx512bw'],
    ['-mavx512dq'],
    ['-mavx512vl'],
    ['-mavx512cd'],
    ['-mavx512vpopcntdq'],
    ['-mavx512bitalg'],
    ['-mavx512vbmi'],
    ['-mavx512vbmi2'],
    ['-mgfni'],
    ['-mavx512vl', '-mavx512bw'],
    ['-mavx512vl', '-mavx512dq'],
    ['-mavx512vl', '-mavx512cd'],
    ['-mavx512vl', '-mavx512bitalg'],
    ['-mavx512vl', '-mavx512vpopcntdq'],
    ['-mavx512vl', '-mavx512cd', '-mgfni']
]

feature_flag_combinations_arm = [
    [''],
    ['', ''],
    ['', ''],
    ['', '', ''],
]

feature_flag_combinations = feature_flag_combinations_x86

feature_combinations_x86 = [
    ['AVEL_X86'],
    ['AVEL_POPCNT'],
    ['AVEL_LZCNT'],
    ['AVEL_BMI'],
    ['AVEL_BMI2'],
    ['AVEL_SSE2'],
    ['AVEL_SSE3'],
    ['AVEL_SSSE3'],
    ['AVEL_SSE41'],
    ['AVEL_SSE42'],
    ['AVEL_AVX'],
    ['AVEL_AVX2'],
    ['AVEL_FMA'],
    ['AVEL_AVX512F'],
    ['AVEL_AVX512BW'],
    ['AVEL_AVX512DQ'],
    ['AVEL_AVX512VL'],
    ['AVEL_AVX512CD'],
    ['AVEL_AVX512VPOPCNTDQ'],
    ['AVEL_AVX512BITALG'],
    ['AVEL_AVX512VBMI'],
    ['AVEL_AVX512VBM2'],
    ['AVEL_GFNI'],
    ['AVEL_AVX512VL', 'AVEL_AVX512BW'],
    ['AVEL_AVX512VL', 'AVEL_AVX512DQ'],
    ['AVEL_AVX512VL', 'AVEL_AVX512CD'],
    ['AVEL_AVX512VL', 'AVEL_AVX512BITALG'],
    ['AVEL_AVX512VL', 'AVEL_AVX512VPOPCNTDQ'],
    ['AVEL_AVX512VL', 'AVEL_GFNI'],
    ['AVEL_AVX512VL', 'AVEL_AVX512CD', 'AVEL_GFNI']
]

feature_combinations_arm = [
    ['AVEL_ARM'],
    ['AVEL_ARM', 'AVEL_NEON'],
    ['AVEL_ARM', 'AVEL_AARCH64'],
    ['AVEL_ARM', 'AVEL_NEON', 'AVEL_AARCH64']
]

feature_combinations = feature_combinations_x86


def test_on_compiler(compiler_path, build_dir_name):
    failed = False
    for (feature_combination, flag_combination) in zip(feature_combinations, feature_flag_combinations):
        flags = '-w'
        cmake_variables = ''

        print(flag_combination)
        print(feature_combination)

        for flag in flag_combination:
            flags += ' ' + flag

        for feature in feature_combination:
            if feature != '':
                cmake_variables += ' ' + "-D" + feature + ":BOOL=ON"

        cmake_command_format_string = \
            'cmake ' \
            '-S"./" ' \
            '-B"./test_build_dirs/{}" ' \
            '-G"CodeBlocks - Unix Makefiles" ' \
            '-DCMAKE_CXX_COMPILER={} ' \
            '-DAVEL_BUILD_TESTS:BOOL=ON{} ' \
            '-DCMAKE_CXX_FLAGS="{}"'
        cmake_command = cmake_command_format_string.format(build_dir_name, compiler_path, cmake_variables, flags)
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

        run_command = './test_build_dirs/{}/AVEL_TESTS'.format(build_dir_name)
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

    return failed


def main():
    failed = False
    for (path, build_dir_name) in compilers:
        failed |= test_on_compiler(path, build_dir_name)
        if failed:
            print("\n\nTesting script: Encountered failed tests")
            return

    print("\n\nTesting script: All tests passed")


if __name__ == '__main__':
    main()
