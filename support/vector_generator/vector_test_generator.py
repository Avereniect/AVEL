#!/usr/bin/python3

import os.path
import avel_types

uint_vector_tests_template_path = './uint_vector_tests.txt'
sint_vector_tests_template_path = './sint_vector_tests.txt'
float_vector_tests_template_path = './float_vector_tests.txt'

output_folder = './tests_output'

dynamic_substitution_templates = {
    '$<extract_tests>'         : '            EXPECT_EQ(inputs[${lane_index_exclusive}], extract<${lane_index_exclusive}>(v));',
    '$<insert_tests>'          : '            v = insert<${lane_index_exclusive}>(v, inputs[${lane_index_exclusive}]);',
    '$<bit_shift_left_tests>'  : '            EXPECT_TRUE(all(bit_shift_left<${bit_index}>(v) == (v << ${bit_index})));',
    '$<bit_shift_right_tests>' : '            EXPECT_TRUE(all(bit_shift_right<${bit_index}>(v) == (v >> ${bit_index})));',
    '$<rotl_tests>'            : '            EXPECT_TRUE(all(rotl<${bit_index}>(v) == (rotl(v, ${bit_index}))));',
    '$<rotr_tests>'            : '            EXPECT_TRUE(all(rotr<${bit_index}>(v) == (rotr(v, ${bit_index}))));',
    '$<load_tests>'            : '            EXPECT_TRUE(all(load<$<vector_alias>, ${lane_index}>(inputs.data()) == load<$<vector_alias>>(inputs.data(), ${lane_index})));',
    '$<aligned_load_tests>'    : '            EXPECT_TRUE(all(aligned_load<$<vector_alias>, ${lane_index}>(inputs.data()) == aligned_load<$<vector_alias>>(inputs.data(), ${lane_index})));',
    '$<gather_tests>'          : '            EXPECT_TRUE(all(gather<$<vector_alias>, ${lane_index}>(test_data.data(), indices) == gather<$<vector_alias>>(test_data.data(), indices, ${lane_index})));',
    '$<store_tests>'           : '            store<${lane_index}>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, ${lane_index}));',
    '$<aligned_store_tests>'   : '            aligned_store<${lane_index}>(arr, v); EXPECT_TRUE(compare_stored_data(arr, v, ${lane_index}));',
    '$<scatter_tests>'         : '            scatter<${lane_index}>(test_data.data(), input0, indices); EXPECT_TRUE(compare_stored_data(test_data.data(), input0, indices, ${lane_index}));'
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
                substitution += substitution_template.\
                    replace('${lane_index}', "0x{:02x}".format(bit_index)).\
                    replace('$<vector_alias>', vector.vector_alias) + '\n'

            ret[variable] = substitution.lstrip()

        elif '${lane_index_exclusive}' in substitution_template:
            substitution = ''
            for bit_index in range(0, vector.width):
                substitution += substitution_template.\
                    replace('${lane_index_exclusive}', "0x{:02x}".format(bit_index)).\
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


def main():
    if not os.path.exists('tests/vectors/'):
        os.makedirs('tests/vectors/')
    generate_vector_test_sources()


if __name__ == '__main__':
    main()
