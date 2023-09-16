#!/usr/bin/python3

import os.path
import avel_types

uint_vector_tests_template_path = './uint_vector_tests.txt'
sint_vector_tests_template_path = './sint_vector_tests.txt'
float_vector_tests_template_path = './float_vector_tests.txt'

output_folder = './tests_output'

dynamic_substitution_templates = {
    '$<extract_tests>': '            EXPECT_EQ(inputs[${lane_index_exclusive}], extract<${lane_index_exclusive}>(v));',
    '$<insert_tests>': '            v = insert<${lane_index_exclusive}>(v, inputs[${lane_index_exclusive}]);',
    '$<bit_shift_left_tests>': '            EXPECT_TRUE(all(bit_shift_left<${bit_index}>(v) == (v << ${bit_index})));',
    '$<bit_shift_right_tests>': '            EXPECT_TRUE(all(bit_shift_right<${bit_index}>(v) == (v >> ${bit_index})));',
    '$<rotl_tests>': '            EXPECT_TRUE(all(rotl<${bit_index}>(v) == (rotl(v, ${bit_index}))));',
    '$<rotr_tests>': '            EXPECT_TRUE(all(rotr<${bit_index}>(v) == (rotr(v, ${bit_index}))));',
    '$<load_tests>': '            EXPECT_TRUE(all(load<$<vector_alias>, ${lane_index}>(inputs.data()) == load<$<vector_alias>>(inputs.data(), ${lane_index})));',
    '$<aligned_load_tests>': '            EXPECT_TRUE(all(aligned_load<$<vector_alias>, ${lane_index}>(inputs.data()) == aligned_load<$<vector_alias>>(inputs.data(), ${lane_index})));',
    '$<gather_tests>': '            EXPECT_TRUE(all(gather<$<vector_alias>, ${lane_index}>(test_data.data(), indices) == gather<$<vector_alias>>(test_data.data(), indices, ${lane_index})));',
    '$<store_tests>': '            store<${lane_index}>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, ${lane_index}));',
    '$<aligned_store_tests>': '            aligned_store<${lane_index}>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, ${lane_index}));',
    '$<scatter_tests>': '            scatter<${lane_index}>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, ${lane_index}));'
}


def generate_variable_dict(vector: avel_types.VectorType):
    # Universal substitutions
    ret = {
        '$<header_guard>': vector.test_header_guard,
        '$<Mask_alias>': vector.mask_alias.capitalize(),
        '$<mask_alias>': vector.mask_alias,
        '$<bool_array_alias>': vector.bool_array_alias,
        '$<Vector_alias>': vector.vector_alias.capitalize(),
        '$<vector_alias>': vector.vector_alias,
        '$<signed_vector_alias>': vector.signed_vector_alias,
        '$<array_alias>': vector.array_alias,
        '$<signed_array_alias>': vector.signed_array_alias,
        '$<unsigned_vector_alias>': vector.uvector_alias
    }

    # Vector-specific substitutions
    for variable, substitution_template in dynamic_substitution_templates.items():
        if '${bit_index}' in substitution_template:
            substitution = ''
            for bit_index in range(0, vector.scalar_type.size * 8 + 1):
                substitution += substitution_template.replace('${bit_index}', "0x{:02x}".format(bit_index)) + '\n'

            ret[variable] = substitution.lstrip()

        elif '${lane_index}' in substitution_template:
            substitution = ''
            for bit_index in range(0, vector.width + 1):
                substitution += substitution_template. \
                                    replace('${lane_index}', "0x{:02x}".format(bit_index)). \
                                    replace('$<vector_alias>', vector.vector_alias) + '\n'

            ret[variable] = substitution.lstrip()

        elif '${lane_index_exclusive}' in substitution_template:
            substitution = ''
            for bit_index in range(0, vector.width):
                substitution += substitution_template. \
                                    replace('${lane_index_exclusive}', "0x{:02x}".format(bit_index)). \
                                    replace('$<vector_alias>', vector.vector_alias) + '\n'

            ret[variable] = substitution.lstrip()

    return ret


def evaluate_conditions(vector_type: avel_types.VectorType, source: str):
    ret = source
    while True:
        condition_begin = ret.find('$BEGIN<')
        condition_end = ret.find('>$')
        end_index = ret.find('$END')

        if condition_begin == -1:
            return ret

        condition_name = ret[condition_begin + 7: condition_end]

        condition_was_set = condition_name in vector_type.variables

        if condition_was_set:
            ret = ret[:condition_begin] + ret[condition_end + 2:end_index] + ret[end_index + 4:]
        else:
            ret = ret[:condition_begin] + ret[end_index + 4:]


def substitute_variables(template_text: str, variable_definitions: dict):
    for variable, definition in variable_definitions.items():
        template_text = template_text.replace(variable, definition)

    return template_text


def generate_vector_test_source(vector: avel_types.VectorType):
    try:
        template_file = open(vector.template_test_file_name, 'r')
    except FileNotFoundError:
        print('Could not open template file: ', vector.template_test_file_name)
        return

    template = template_file.read()
    template_file.close()

    variable_dict = generate_variable_dict(vector)
    generated_code = evaluate_conditions(vector, template)
    generated_code = substitute_variables(generated_code, variable_dict)

    output_file = open('tests/vectors/' + vector.output_test_file_name, 'w+')
    output_file.writelines([generated_code])
    output_file.close()


def generate_vector_test_sources():
    for vector in avel_types.vector_types:
        generate_vector_test_source(vector)


def generate_conversion_tests():
    lines = [
        '#ifndef AVEL_VECTOR_MASK_TESTS_HPP\n',
        '#define AVEL_VECTOR_MASK_TESTS_HPP\n',
        '\n',
        'namespace avel_tests {\n\n'
        '    using namespace avel;\n'
    ]

    conversion_test_template = '''
    TEST(Convert, <VECTOR_FROM>_to_<VECTOR_TO>) {
        for (std::size_t i = 0; i < iterations; ++i) {
            auto inputs = random_array<<ARRAY_FROM>>();

            auto v0 = <VECTOR_FROM>{inputs};
            auto results = avel::convert<<VECTOR_TO>>(v0);

            <ARRAY_TO> expected{};
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                expected[j] = static_cast<<SCALAR_TO>>(inputs[j]);
            }
            
            std::size_t w = <VECTOR_FROM>::width / results.size();
            for (std::size_t j = 0; j < results.size(); ++j) {
                auto observed = avel::to_array(results[j]);

                for (std::size_t k = 0; k < observed.size(); ++k) {
                    EXPECT_TRUE(expected[j * w + k] == observed[k]);
                }
            }
        }
    }
    '''

    # Conversions up to single-width vector conversion
    for v0 in avel_types.vector_types:
        for v1 in avel_types.vector_types:
            if v0.width > 1 or v1.width > 1:
                continue

            test_code = conversion_test_template
            test_code = test_code.replace('<VECTOR_FROM>', v0.vector_alias)
            test_code = test_code.replace('<VECTOR_TO>', v1.vector_alias)
            test_code = test_code.replace('<SCALAR_FROM>', v0.scalar_type.full_name)
            test_code = test_code.replace('<SCALAR_TO>', v1.scalar_type.full_name)
            test_code = test_code.replace('<ARRAY_FROM>', v0.array_alias)
            test_code = test_code.replace('<ARRAY_TO>', v1.array_alias)

            lines += test_code

    # Conversions up to 128-bit vector conversion
    lines.append('\n#if defined(AVEL_SSE2) || defined(AVEL_NEON)')
    for v0 in avel_types.vector_types:
        for v1 in avel_types.vector_types:
            if v0.size < 16 and v1.size < 16:
                continue

            if v0.size > 16 or v1.size > 16:
                continue

            test_code = conversion_test_template
            test_code = test_code.replace('<VECTOR_FROM>', v0.vector_alias)
            test_code = test_code.replace('<VECTOR_TO>', v1.vector_alias)
            test_code = test_code.replace('<SCALAR_FROM>', v0.scalar_type.full_name)
            test_code = test_code.replace('<SCALAR_TO>', v1.scalar_type.full_name)
            test_code = test_code.replace('<ARRAY_FROM>', v0.array_alias)
            test_code = test_code.replace('<ARRAY_TO>', v1.array_alias)

            lines += test_code
    lines.append('\n#endif\n')

    # Conversions up to 256-bit vector conversion
    lines.append('\n#if defined(AVEL_AVX2)')
    for v0 in avel_types.vector_types:
        for v1 in avel_types.vector_types:
            if v0.size < 32 and v1.size < 32:
                continue

            if v0.size > 32 or v1.size > 32:
                continue

            test_code = conversion_test_template
            test_code = test_code.replace('<VECTOR_FROM>', v0.vector_alias)
            test_code = test_code.replace('<VECTOR_TO>', v1.vector_alias)
            test_code = test_code.replace('<SCALAR_FROM>', v0.scalar_type.full_name)
            test_code = test_code.replace('<SCALAR_TO>', v1.scalar_type.full_name)
            test_code = test_code.replace('<ARRAY_FROM>', v0.array_alias)
            test_code = test_code.replace('<ARRAY_TO>', v1.array_alias)

            lines += test_code
    lines.append('\n#endif\n')

    # Conversions up to 512-bit vector conversions, not including those dealing with 8 and 16-bit 512-bit elements
    lines.append('\n#if defined(AVEL_AVX512F)')
    for v0 in avel_types.vector_types:
        for v1 in avel_types.vector_types:
            if v0.size < 64 and v1.size < 64:
                continue

            if v0.scalar_type.size <= 2 or v1.scalar_type.size <= 2:
                continue

            test_code = conversion_test_template
            test_code = test_code.replace('<VECTOR_FROM>', v0.vector_alias)
            test_code = test_code.replace('<VECTOR_TO>', v1.vector_alias)
            test_code = test_code.replace('<SCALAR_FROM>', v0.scalar_type.full_name)
            test_code = test_code.replace('<SCALAR_TO>', v1.scalar_type.full_name)
            test_code = test_code.replace('<ARRAY_FROM>', v0.array_alias)
            test_code = test_code.replace('<ARRAY_TO>', v1.array_alias)

            lines += test_code
    lines.append('\n#endif\n')

    # Conversions up to 512-bit vector conversions, only including those dealing with 8 and 16-bit 512-bit elements
    lines.append('\n#if defined(AVEL_AVX512BW)')
    for v0 in avel_types.vector_types:
        for v1 in avel_types.vector_types:
            if v0.size < 64 and v1.size < 64:
                continue

            if not (v0.scalar_type.size <= 2 or v1.scalar_type.size <= 2):
                continue

            test_code = conversion_test_template
            test_code = test_code.replace('<VECTOR_FROM>', v0.vector_alias)
            test_code = test_code.replace('<VECTOR_TO>', v1.vector_alias)
            test_code = test_code.replace('<SCALAR_FROM>', v0.scalar_type.full_name)
            test_code = test_code.replace('<SCALAR_TO>', v1.scalar_type.full_name)
            test_code = test_code.replace('<ARRAY_FROM>', v0.array_alias)
            test_code = test_code.replace('<ARRAY_TO>', v1.array_alias)

            lines += test_code
    lines.append('\n#endif\n')

    lines += [
        '\n}\n',
        '\n',
        '#endif //AVEL_VECTOR_MASK_TESTS_HPP\n'
    ]

    output_file = open('tests/vectors/Vector_conversion_tests.hpp', 'w+')
    output_file.writelines(lines)
    output_file.close()


def main():
    if not os.path.exists('tests/vectors/'):
        os.makedirs('tests/vectors/')
    # generate_vector_test_sources()
    generate_conversion_tests()


if __name__ == '__main__':
    main()
