#!/usr/bin/env python

"""
Script that runs AVEL's benchmarks.



Example usages:

Run all benchmarks for x86 CPUs: run_avel_benchmarks.py -A"x86" -C"/usr/bin/g++-9"
Run benchmarks from specific file: run_avel_benchmarks.py -A"x86" -C"/usr/bin/g++-9 -F"fpclassify_f32"
"""

import sys
import os
import re
import platform
import subprocess
import itertools
import shutil

from functools import reduce
from functools import cmp_to_key

bench_output_dir = './benchmark_results/'
"""Directory where benchmark results should be placed"""

build_path = './benchmark_build_dir/'
"""Path to build benchmark executables at"""

cli_arguments = {}
"""
Dictionary containing command line arguments
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
    'AVEL_GFNI': ['-mgfni', ['AVEL_AVX512F']],
    #'AVEL_AVX10_1': ['-mavx10.1', ['AVEL_AVX2']],
    #'AVEL_AVX10_2': ['-mavx10.2', ['AVEL_AVX10_1']],
}
"""
Dictionary associating feature macros with their directly implied features, according to AVEL
"""

features_arm = {
    'AVEL_ARM': ['', []],
    'AVEL_NEON': ['', ['AVEL_ARM']],
    'AVEL_AARCH64': ['', ['AVEL_ARM']]
}
"""
Map associating feature macros with their implied features, according to AVEL
"""

arch_feature_list = [features_x86, features_arm]
"""List of all feature maps"""

target_features = features_x86

compiler_macros = [
    'AVEL_GCC',
    'AVEL_CLANG',
    'AVEL_ICPX'
]
"""
List of AVEL's compiler macros
"""


class Compiler:
    name: str
    path: str
    avel_macro: str


class FeatureCheck:
    expression: str
    variables: [str]


class VariableAssignments:
    backing_dict = {}

    def __getitem__(self, key):
        return self.backing_dict[key]

    def __setitem__(self, key, value):
        if value not in ['True', 'False']:
            print('Error: Attempted to assign unrecognized value to variable in VariableAssignments')
            exit(1)

        self.backing_dict[key] = value

    def __hash__(self):
        tmp0 = reduce(lambda x, y: x + y, [hash(x) for x in self.backing_dict.keys()])
        tmp1 = reduce(lambda x, y: x + y, [hash(x) for x in self.backing_dict.values()])

        return hash(tmp0 + tmp1)

    def __eq__(self, other):
        return self.backing_dict == other.backing_dict

    def items(self):
        return self.backing_dict.items()

    def update(self, **kwargs):
        self.backing_dict.update(kwargs)


class BuildConfiguration:
    compiler: Compiler
    variable_assignments: VariableAssignments
    bench_file_names = []


class TimingResults:
    """
    Represents the results from a single timing
    """

    config: BuildConfiguration = None

    config_idx = -1

    name: str = None

    time: str

    cpu_time: str


class FeatureResults:
    """
    Represents the information produced by the testing of a particular feature
    """

    feature_name: str = None
    """Name of feature being benchmarked"""

    configs = [BuildConfiguration]
    """A list of configurations which were used in the benchmarking of the current feature"""

    timings: [TimingResults]
    """List of timings that were performed as part of  benchmarking the specified feature"""


def get_processor_name():
    if platform.system() == "Windows":
        return platform.processor()

    elif platform.system() == "Darwin":
        os.environ['PATH'] = os.environ['PATH'] + os.pathsep + '/usr/sbin'
        command = "sysctl -n machdep.cpu.brand_string"
        return subprocess.check_output(command).strip()

    elif platform.system() == "Linux":
        command = "cat /proc/cpuinfo"
        all_info = subprocess.check_output(command, shell=True).decode().strip()
        for line in all_info.split("\n"):
            if "model name" in line:
                return re.sub(".*model name.*:", "", line, 1)

    return platform.processor()


def default_foreign_architecture_variables(a: VariableAssignments):
    if cli_arguments['arch'] == 'x86':
        for feature in features_arm:
            a[feature] = 'False'
    elif cli_arguments['arch'] == 'arm':
        for feature in features_x86:
            a[feature] = 'False'

    return a


def compiler_name_to_avel_macro(compiler_name):
    """
    Attempts to convert a compiler's path the AVEL macro corresponding to that
    compiler
    :param compiler_name: Name of compiler to convert to an AVEL macro
    :return: string containing macro that AVEL uses to indicate which compiler
    is being used
    """

    if 'g++' in compiler_name:
        return 'AVEL_GCC'
    elif 'clang' in compiler_name:
        return 'AVEL_CLANG'
    elif 'Intel(R) oneAPI DPC++/C++ Compiler' in compiler_name:
        return 'AVEL_ICPX'
    else:
        print('Compiler not recognized as g++, clang++, or icpx.')
        exit(1)


def parse_command_line_arguments(arguments: [str]):
    """
    Populates the global cli_arguments dictionary

    :param arguments: Array of command line arguments
    :return: dictionary associating recognized parameter to values specified at the command line
    """

    results = {}
    for argument in arguments:
        if argument.startswith('-A'):
            tail = argument.lstrip('-A')
            results['arch'] = tail
            continue

        if argument.startswith('-C'):
            tail = argument.lstrip('-C')
            results['compiler_list_str'] = tail
            continue

        if argument.startswith('-F'):
            tail = argument.lstrip('-F')
            results['bench_files'] = tail
            continue

        print('Unrecognized argument: ', argument)
        exit(1)

    global cli_arguments
    cli_arguments = results


def validate_command_line_arguments():
    """
    Ensures that the command line parameters specified exist and meet script requirements.
    Terminate otherwise
    """

    if not cli_arguments:
        print('Recognized parameters:')
        print('  -A    (Required) The target architecture to run benchmarks on. Should be either \'x86\' or \'arm\'')
        print('  -C    (Required) A colon-delimited list of paths to compilers to run the benchmarks with.')
        print('  -F    (Optional) A colon-delimited list of file names from the AVEL/benchmarks/ directory whose '
              '        contained benchmarks should be run.')
        exit(1)

    # Handle arch argument
    if 'arch' not in cli_arguments:
        print('Target architecture not specified!')
        print('Specify the -A argument to be either \'x86\' or \'arm\'')
        exit(1)

    global target_features
    if cli_arguments['arch'] == 'x86':
        target_features = features_x86
    elif cli_arguments['arch'] == 'arm':
        target_features = features_arm
    else:
        print('Unrecognized architecture: ', cli_arguments['arch'])
        exit(1)

    # Handle compiler paths argument
    if 'compiler_list_str' not in cli_arguments:
        print('No compiler specified')
        exit(1)

    for compiler_path in cli_arguments['compiler_list_str'].split(':'):
        if not get_compiler_name(compiler_path):
            print('Could not launch compiler at path: ' + compiler_path)
            exit(1)

    if 'bench_file' in cli_arguments:
        bench_file = cli_arguments['bench_file']
        if not os.path.isfile('benchmarks/' + bench_file):
            print('Could not find bench file at path: ' + bench_file)
            exit(1)


def unroll_feature_implications():
    """
    Applies transitive implications to the target_features map, making each feature contains its indirect implications
    in addition to its direct implications
    """

    global features_x86
    global features_arm

    for key in target_features:
        implications = target_features[key][1]
        for implication in implications:
            implied_features = target_features[implication][1]
            target_features[key][1] += implied_features


def list_bench_files(bench_path: str):
    """
    Produces a list of AVEL benchmark files

    :param bench_path: path to directory containing benchmark files to run
    :return: List of file names for AVEL benchmark files
    """
    # Get list of files in directory
    files = [f for f in os.listdir(bench_path) if os.path.isfile(bench_path + f)]

    suffixes = (
        '_8u.hpp', '_16u.hpp', '_32u.hpp', '_64u.hpp',
        '_8i.hpp', '_16i.hpp', '_32i.hpp', '_64i.hpp',
        '_32f.hpp', '_64f.hpp'
    )

    # Filter for bench files based on suffix
    files = [f for f in files if f.endswith(suffixes)]

    return files


def parse_feature_check(line):
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

    line = line.replace('!', ' not ')

    # TODO: Adjust this when multiple C++ versions are handled
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

    ret = FeatureCheck()
    ret.expression = line
    ret.variables = var_list
    return ret


def identify_feature_checks(bench_file_path):
    """

    :param bench_file_path: Path to file containing benchmarks
    :return: A map associating expressions to a list of variables that they utilize
    """

    file = open(bench_file_path, 'r')
    lines = file.readlines()
    file.close()

    def is_line_feature_check_macro(l: str):
        return l.lstrip().startswith('#') and (('#if' in l) or ('#elif' in l)) and ('defined' in l) and ('AVEL' in l)

    feature_checks = []

    # Turn feature check macros into Boolean Python expressions and a list of Boolean variable names
    for line in lines:
        if not is_line_feature_check_macro(line):
            continue

        feature_checks.append(parse_feature_check(line))

    return feature_checks


def substitute_variable(expression: str, variable_name: str, value: str):
    """

    :param expression:
    :param variable_name:
    :param value:
    :return:
    """
    search_pattern = r'\b' + variable_name + r'\b'
    return re.sub(search_pattern, value, expression)


def substitute_variables(expression: str, variable_assignments: VariableAssignments):
    for variable, value in variable_assignments.items():
        expression = substitute_variable(expression, variable, value)

    return expression


def evaluate_feature_check_expression(expression: str, variable_assignments: VariableAssignments):
    return eval(substitute_variables(expression, variable_assignments))


def create_feature_assignments(feature_check: FeatureCheck, base_assignments: VariableAssignments = None):
    """
    Find a set of feature assignments which will cause the feature check to
    pass.

    :param feature_check: A feature check to create an assignment for
    :param base_assignments: A variable map whose assignments will be respected
    :return: A variable assignment map which will cause the check to pass or
    None if no such assignment exists.
    """

    expression = feature_check.expression

    # Apply pre-selected assignments
    if base_assignments:
        expression = substitute_variables(feature_check.expression, base_assignments)

    # Enumerate all possible assignments of specified variables
    num_vars = len(feature_check.variables)
    possible_solutions = [seq for seq in itertools.product(('True', 'False'), repeat=num_vars)]

    # Prefer assignments which enable the smallest number of features
    possible_solutions = sorted(possible_solutions, key=lambda x: len(x))

    # Find suitable assignment
    for solution in possible_solutions:
        assignments = VariableAssignments()
        assignments.backing_dict = dict(zip(feature_check.variables, solution))

        if evaluate_feature_check_expression(expression, assignments):
            if base_assignments:
                assignments.backing_dict |= base_assignments.backing_dict

            return assignments

    return None


def expand_feature_assignments(assignments: VariableAssignments):
    """
    Applies the implications of the features specified within the assignment
    dictionary and gives them explicit assignments.

    :param assignments: Variable assignment map to expand
    :return: Expanded variable assignment map
    """

    new_assignments = {}
    for variable, value in assignments.items():
        new_assignments[variable] = value

        if variable in target_features and value == 'True':
            for implied_feature in target_features[variable][1]:
                new_assignments[implied_feature] = 'True'

    # TODO: Make more generic
    if cli_arguments['arch'] == 'x86':
        for feature in features_arm.keys():
            new_assignments[feature] = 'False'

    ret = VariableAssignments()
    ret.backing_dict = new_assignments
    return ret


def run_benchmarks_on_compiler(config: BuildConfiguration):
    """

    :param config: Build configuration
    :return:
    """

    # Create variable assignments
    variable_assignments = config.variable_assignments

    # Create string for CMake macros and compiler flags
    flags = '-w -std=c++11'
    cmake_variables = ''

    for variable, value in variable_assignments.items():
        if variable in target_features.keys():
            if value == 'True':
                cmake_variables += ' ' + '-D' + variable + ':BOOL=ON'
                flags += ' ' + target_features[variable][0]
            else:
                cmake_variables += ' ' + '-D' + variable + ':BOOL=OFF'

    # Populate AVEL_benchmarks_include.hpp
    include_lines = []
    for bench_file in config.bench_file_names:
        include_lines.append('#include "{}"'.format(bench_file))

    bench_file_include_line = '\n'.join(include_lines)
    with open("benchmarks/AVEL_benchmarks_include.hpp", "w") as f:
        f.write(bench_file_include_line)

    # Run CMake command
    cmake_command_format_string = \
        'cmake ' \
        '-S"./" ' \
        '-B{} ' \
        '-G"CodeBlocks - Unix Makefiles" ' \
        '-DCMAKE_BUILD_TYPE=Release ' \
        '-DCMAKE_CXX_COMPILER={} ' \
        '-DAVEL_BUILD_BENCHMARKS:BOOL=ON{} ' \
        '-DCMAKE_CXX_FLAGS="{}"'

    cmake_command = cmake_command_format_string.format(
        build_path,
        config.compiler.path,
        cmake_variables,
        flags
    )
    os.system(cmake_command)

    # Run Make command
    make_command = 'make AVEL_benchmarks -C {}'.format(build_path)
    print(make_command)
    os.system(make_command)

    # Run benchmark executable
    run_command = build_path + 'benchmarks/AVEL_benchmarks'
    run_arguments = [
        run_command,
        '--benchmark_min_warmup_time=0.1',
        '--benchmark_repetitions=1',
        '--benchmark_display_aggregates_only=true',
        '--benchmark_out_format=console',
        '--benchmark_color=false',
        '--benchmark_enable_random_interleaving=true'
    ]

    try:
        result = subprocess.run(run_arguments, stdout=subprocess.PIPE)
    except subprocess.CalledProcessError as e:
        return None

    output = str(result.stdout.decode())

    # Remove build directory
    shutil.rmtree(build_path)

    # Clear contents of
    open('benchmarks/AVEL_benchmarks_include.hpp', 'w').close()

    return construct_result_objects(output, config)


def construct_result_objects(bench_outputs: str, config: BuildConfiguration):
    """
    Turn the output of the benchmark program into a list of 3-tuples for
    further processing.

    :param bench_outputs: A string produced by the outputs produced by the benchmark
    :param config: j
    :return: List of TimingResults objects
    """

    lines = str.splitlines(bench_outputs)

    # Skip boilerplate results
    lines = lines[3:]

    def unit_space_remover(x):
        return x.replace(' ns', 'ns').replace(' us', 'us').replace(' ms', 'ms').replace(' s', 's')

    # Split columns into 3-tuple. Discard last column
    lines = [' '.join(line.split()) for line in lines]
    lines = [unit_space_remover(line) for line in lines]
    columns = [tuple(line.split(' ')[0:3]) for line in lines]

    ret = []
    for col0, col1, col2 in columns:
        results = TimingResults()
        results.config = config
        results.name = col0
        results.time = col1
        results.cpu_time = col2

        ret.append(results)

    return ret


def generate_table_lines(timings: [TimingResults]):
    """

    :param timings: List of 3-tuples containing benchmark names, times, and
    cpu times
    :return: List of strings that represent a Markdown table displaying the
    contents of the timings list
    """

    col0_strings = [str(t.config_idx) for t in timings]
    col1_strings = [t.name for t in timings]
    col2_strings = [t.time for t in timings]
    col3_strings = [t.cpu_time for t in timings]

    col0_strings.insert(0, 'Config')
    col1_strings.insert(0, 'Implementation')
    col2_strings.insert(0, 'Time')
    col3_strings.insert(0, 'CPU Time')

    col0_string_width = max(len(x) for x in col0_strings)
    col1_string_width = max(len(x) for x in col1_strings)
    col2_string_width = max(len(x) for x in col2_strings)
    col3_string_width = max(len(x) for x in col3_strings)

    col0_strings.insert(1, '-' * col0_string_width)
    col1_strings.insert(1, '-' * col1_string_width)
    col2_strings.insert(1, '-' * col2_string_width)
    col3_strings.insert(1, '-' * col3_string_width)

    col0_strings = [s.ljust(col0_string_width) for s in col0_strings]
    col1_strings = [s.ljust(col1_string_width) for s in col1_strings]
    col2_strings = [s.ljust(col2_string_width) for s in col2_strings]
    col3_strings = [s.ljust(col3_string_width) for s in col3_strings]

    table_lines = []
    for col0, col1, col2, col3 in zip(col0_strings, col1_strings, col2_strings, col3_strings):
        line = '| ' + col0 + ' | ' + col1 + ' | ' + col2 + ' | ' + col3 + ' |'
        table_lines.append(line)

    return table_lines


def remove_type_suffix(name: str):
    suffixes = [
        '_8u',
        '_16u',
        '_32u',
        '_64u',
        '_8i',
        '_16i',
        '_32i',
        '_64i',
        '_16f',
        '_32f',
        '_64f'
    ]

    for suffix in suffixes:
        if name.endswith(suffix):
            return tuple([name.removesuffix(suffix), suffix])

    return tuple([name, None])


def benchmark_name_comparator(name0: str, name1: str):
    suffixes = [
        None,
        '_8u',
        '_16u',
        '_32u',
        '_64u',
        '_8i',
        '_16i',
        '_32i',
        '_64i',
        '_16f',
        '_32f',
        '_64f'
    ]

    (n0_trimmed, suffix0) = remove_type_suffix(name0)
    (n1_trimmed, suffix1) = remove_type_suffix(name1)

    if n0_trimmed < n1_trimmed:
        return -1

    if n0_trimmed > n1_trimmed:
        return 1

    idx0 = suffixes.index(suffix0)
    idx1 = suffixes.index(suffix1)

    return idx0 - idx1


def generate_markdown_file(grouped_results: {str: [TimingResults]}):
    """
    Generate Markdown table containing benchmark results.

    :param grouped_results: Dictionary of results to print out
    :return: Markdown text for
    """

    # Sort results so that they appear in the appropriate order
    lines = []
    lines.append('# AVEL Benchmark Results')
    lines.append('Note: Measurements should only be compared relative to others in the same table')
    lines.append('Note: Measurements for masks should not be compared between configurations that do and don\'t have AVX-512')
    lines.append('CPU:' + get_processor_name())

    sorted_groups = sorted(grouped_results.items())

    for bench_name, results_list in sorted_groups:
        lines.append('## ' + bench_name)

        # Get list of configurations
        configurations = []
        for results in results_list:
            if results.config not in configurations:
                configurations.append(results.config)

        # Output configurations
        lines.append('### Configurations:')
        for idx, config in enumerate(configurations):
            lines.append('#### Config ' + str(idx))
            lines.append('* ' + config.compiler.name)

            feature_strings = []
            for var, value in config.variable_assignments.items():
                if var in target_features and value == 'True':
                    feature_strings.append(var.removeprefix('AVEL_'))

            if feature_strings:
                config_line = '* ' + ', '.join(feature_strings)
                lines.append(config_line)
            lines.append('')

        lines.append('### Results:')

        # Assign indices to each configuration
        for results in results_list:
            results.config_idx = configurations.index(results.config)

        # Generate table
        lines += generate_table_lines(results_list)
        lines.append('\n')

    return '\n'.join(lines)


def get_compiler_name(compiler_path: str):
    """
    Ask the compiler to identify itself
    :param compiler_path: Path to compiler
    :return: String containing compiler self-identification or None if a problem occurred
    """

    try:
        result = subprocess.run([compiler_path, '--version'], stdout=subprocess.PIPE)
    except subprocess.CalledProcessError as e:
        return None

    output = result.stdout.decode()
    newline_index = output.find('\n')

    if newline_index == -1:
        return output

    return output[0:newline_index]


def is_subset(a: VariableAssignments, b: VariableAssignments):
    for variable, value in a.items():
        if value == 'False':
            continue

        if variable not in b.backing_dict.keys() or b[variable] == 'False':
            return False

    return True


def deduplicate_results(config_results_dict: {BuildConfiguration: [TimingResults]}):
    for config0, results0 in config_results_dict.items():
        for config1, results1 in config_results_dict.items():
            if config0 == config1:
                continue

            if is_subset(config0.variable_assignments, config1.variable_assignments):
                new_results1 = []

                for result1 in results1:
                    is_in_subset = False
                    for result0 in results0:
                        if result1.name == result0.name:
                            is_in_subset = True
                            break

                    if not is_in_subset:
                        new_results1.append(result1)

                config_results_dict[config1] = new_results1

    return config_results_dict


def group_results(results: {BuildConfiguration: TimingResults}):
    prefix_map = {}

    for results_list in results.values():
        for result in results_list:
            double_colon_index = result.name.find('::')
            if double_colon_index == -1:
                print("Error test case does not contain underscore in name as expected")
                exit(1)

            prefix = result.name[0:double_colon_index]

            if prefix in prefix_map:
                prefix_map[prefix].append(result)
            else:
                prefix_map[prefix] = [result]

    return prefix_map


def write_to_file(path: os.path, text: str):
    """

    :param path: File path to write to
    :param text: String whose contents will be written out
    :return:
    """

    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w") as f:
        f.write(text)


def main():
    parse_command_line_arguments(sys.argv[1:])
    validate_command_line_arguments()

    unroll_feature_implications()

    # Create of bench file name to compile
    if 'bench_files' in cli_arguments.keys():
        bench_files_names = cli_arguments['bench_files'].split(':')

        # Append .hpp is not present
        bench_files_names = [f if f.endswith('.hpp') else f + '.hpp' for f in bench_files_names]
    else:
        bench_files_names = list_bench_files('./benchmarks/')

    # List compilers
    compilers = []
    for path in cli_arguments['compiler_list_str'].split(':'):
        compiler = Compiler()
        compiler.path = path
        compiler.name = get_compiler_name(path)
        compiler.avel_macro = compiler_name_to_avel_macro(compiler.name)
        compilers.append(compiler)

    # Extract feature checks from each bench file
    bench_file_feature_checks_map = {}
    for name in bench_files_names:
        bench_file_feature_checks_map[name] = identify_feature_checks("./benchmarks/" + name)

    # Make map of feature assignments to list of benchmark files that need to be run
    config_variable_assignments_map: {(Compiler, VariableAssignments): [str]} = {}
    for compiler in compilers:
        base_assignments = VariableAssignments()
        base_assignments[compiler.avel_macro] = 'True'
        for macro in compiler_macros:
            if macro != compiler.avel_macro:
                base_assignments[macro] = 'False'
        base_assignments = default_foreign_architecture_variables(base_assignments)

        # Add base assignments case to variable_assignment_set
        config_variable_assignments_map[(compiler, base_assignments)] = bench_files_names

        for bench_file, feature_checks in bench_file_feature_checks_map.items():
            for feature_check in feature_checks:
                assignments = create_feature_assignments(feature_check, base_assignments)

                # create_feature_assignments will return None if no successful assignment exists
                if not assignments:
                    continue

                compiler_assignment_tuple = (compiler, assignments)

                if compiler_assignment_tuple in config_variable_assignments_map.keys():
                    if bench_file not in config_variable_assignments_map[compiler_assignment_tuple]:
                        config_variable_assignments_map[compiler_assignment_tuple].append(bench_file)
                else:
                    config_variable_assignments_map[compiler_assignment_tuple] = [bench_file]

    # Convert to list of configurations
    configurations = []
    for compiler_assignment_tuple, bench_file_list in config_variable_assignments_map.items():
        (compiler, assignments) = compiler_assignment_tuple

        config = BuildConfiguration()
        config.compiler = compiler
        config.variable_assignments = expand_feature_assignments(assignments)
        config.bench_file_names = bench_file_list

        configurations.append(config)

    # Run benchmarks
    results_dict = {}
    for configuration in configurations:
        results_dict[configuration] = run_benchmarks_on_compiler(configuration)

    # Write results to Markdown file
    deduplicated_results = deduplicate_results(results_dict)
    grouped_results = group_results(deduplicated_results)
    markdown_contents = generate_markdown_file(grouped_results)
    write_to_file(bench_output_dir + 'benchmark_results.md', markdown_contents)


if __name__ == '__main__':
    main()
