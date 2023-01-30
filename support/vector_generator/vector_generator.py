#!/usr/bin/python3

from avel_conversions import *
from avel_types import *

import os.path

output_directory = './output/'
input_directory = './implementations/'


def substitute_variables(vector_type: VectorType, template: str):
    ret = template
    ret = ret.replace('$<HEADER_GUARD>', vector_type.header_guard)
    ret = ret.replace('$<VECTOR_ALIAS>', vector_type.vector_alias)
    ret = ret.replace('$<ARRAY_ALIAS>', vector_type.array_alias)
    ret = ret.replace('$<MASK_ALIAS>', vector_type.mask_alias)
    ret = ret.replace('$<VECTOR_WIDTH>', str(vector_type.width))
    ret = ret.replace('$<VECTOR_ALIGNMENT>', str(vector_type.alignment))
    ret = ret.replace('$<SCALAR_NAME>', vector_type.scalar_type.full_name)
    ret = ret.replace('$<SCALAR_ABBREVIATION>', vector_type.scalar_type.abbreviation)
    ret = ret.replace('$<SCALAR_WIDTH>', str(vector_type.scalar_type.size * 8))
    ret = ret.replace('$<USCALAR_NAME>', to_unsigned(vector_type.scalar_type).full_name)
    ret = ret.replace('$<UVECTOR_ALIAS>', vector_type.uvector_alias)
    ret = ret.replace('$<UMASK_ALIAS>', vector_type.umask_alias)
    return ret


def parse_sources(vector_type: VectorType, sources: str):
    ret = {}

    sources_list = sources.split('@')

    sources_list = [x.rstrip('\n') for x in sources_list]

    for source in sources_list:
        if source == '':
            continue

        header_end = source.find('\n')
        if header_end == -1:
            ret[source] = ''
            continue

        source_header = source[:header_end]
        source_code = source[header_end + 1:]

        ret[source_header] = substitute_variables(vector_type, source_code)

    return ret


def indent_replacement(replacement: str, level: int):
    replacement = replacement.replace('\t', '    ')
    new_indentation = '\n' + level * ' '

    #TODO: Trim indentation off of replacement first

    replacement = replacement.replace('\n', new_indentation)

    return replacement


def substitute_sources(vector_type: VectorType, template: str, dictionary):
    ret = substitute_variables(vector_type, template)

    for key in dictionary:
        search_term = '$<' + key + '>'

        while True:
            try:
                index = ret.index(search_term)
            except ValueError:
                break

            spaces = 0
            while ret[index - 1 - spaces] == ' ':
                spaces += 1

            replacement = indent_replacement(dictionary[key], spaces)
            ret = ret.replace(search_term, replacement, 1)

    return ret


def evaluate_conditions(vector_type: VectorType, source: str):
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


def generate_vector_file(vector_type: VectorType):
    try:
        template_file = open(vector_type.template_file_name, 'r')
    except FileNotFoundError:
        print('Could not open template file: ', vector_type.template_file_name)
        return

    template = template_file.read()
    template_file.close()

    sources = ''

    for input_file_path in vector_type.input_files:
        path = input_directory + input_file_path
        try:
            input_file = open(path, 'r')
        except FileNotFoundError:
            print('Could not open sources file: ', path)
            return

        sources += input_file.read()
        input_file.close()

    template = evaluate_conditions(vector_type, template)

    mask_conversions = generate_mask_conversions(vector_type)
    template = template.replace('$<MASK_CONVERSIONS>', mask_conversions)

    vector_conversions = generate_vector_conversions(vector_type)
    template = template.replace('$<VECTOR_CONVERSIONS>', vector_conversions)

    sources_dict = parse_sources(vector_type, sources)
    generated_code = substitute_sources(vector_type, template, sources_dict)

    output_file = open(output_directory + vector_type.output_file_name, 'w+')
    output_file.writelines([generated_code])
    output_file.close()


def main():
    if not os.path.exists(output_directory):
        os.makedirs(output_directory)

    for vector_type in vector_types:
        generate_vector_file(vector_type)


if __name__ == '__main__':
    main()
