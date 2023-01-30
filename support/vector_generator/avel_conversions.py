from avel_types import *


def generate_single_to_single_mask_conversion(source: VectorType, target: VectorType):
    conversion_template = """
    template<>
    [[nodiscard]]
    AVEL_FINL std::array<{to}, {size}> convert<{to}, {from}>({from} m) {
        return std::array<{to}, {size}>{{to}{decay(m)}};
    }
"""

    size = 1
    ret = conversion_template
    ret = ret.replace('{size}', str(size))
    ret = ret.replace('{from}', source.mask_alias)
    ret = ret.replace('{to}', target.mask_alias)

    return ret


def generate_mask_conversion(source: VectorType, target: VectorType):
    if source.width == 1 and target.width == 1:
        return generate_single_to_single_mask_conversion(source, target)

    if source == target:
        conversion_template = """
    template<>
    [[nodiscard]]
    AVEL_FINL std::array<{to}, {size}> convert<{to}, {from}>({from} m) {
        return std::array<{to}, {size}>{m};
    }
"""
    else:
        conversion_template = """
    template<>
    [[nodiscard]]
    AVEL_FINL std::array<{to}, {size}> convert<{to}, {from}>({from} m) {
        $<convert<{to}>({from})>
    }
"""

    size = max(1, source.width // target.width)
    ret = conversion_template
    ret = ret.replace('{size}', str(size))
    ret = ret.replace('{from}', source.mask_alias)
    ret = ret.replace('{to}', target.mask_alias)

    return ret


def generate_mask_conversions(vector_type: VectorType):
    conversions_from = ""
    conversions_to = ""

    for other_vector in vector_types:
        if vector_type == other_vector:
            break

        conversions_from += generate_mask_conversion(vector_type, other_vector)
        conversions_to += generate_mask_conversion(other_vector, vector_type)

    conversions_from += generate_mask_conversion(vector_type, vector_type)

    return conversions_from + conversions_to


def generate_equal_size_integer_conversion(source: VectorType, target: VectorType):
    conversion_template = """
    template<>
    [[nodiscard]]
    AVEL_FINL std::array<{to}, {size}> convert<{to}, {from}>({from} m) {
        return std::array<{to}, {size}>{{to}{static_cast<{to}::scalar>(decay(m))}};
    }
"""

    size = 1
    ret = conversion_template
    ret = ret.replace('{size}', str(size))
    ret = ret.replace('{from}', source.mask_alias)
    ret = ret.replace('{to}', target.mask_alias)

    return ret


def generate_single_smaller_to_larger_integer_vector_conversion(source: VectorType, target: VectorType):
    conversion_template = """
    template<>
    [[nodiscard]]
    AVEL_FINL std::array<{to}, {size}> convert<{to}, {from}>({from} m) {
        return std::array<{to}, {size}>{{to}{static_cast<{to}::scalar>(decay(m))}};
    }
"""

    size = 1
    ret = conversion_template
    ret = ret.replace('{size}', str(size))
    ret = ret.replace('{from}', source.mask_alias)
    ret = ret.replace('{to}', target.mask_alias)

    return ret


def generate_single_larger_to_smaller_integer_vector_conversion(source: VectorType, target: VectorType):
    conversion_template = """
    template<>
    [[nodiscard]]
    AVEL_FINL std::array<{to}, {size}> convert<{to}, {from}>({from} m) {
        return std::array<{to}, {size}>{{to}{static_cast<{to}::scalar>(decay(m) & {mask})}};
    }
"""

    size = 1
    ret = conversion_template
    ret = ret.replace('{size}', str(size))
    ret = ret.replace('{from}', source.mask_alias)
    ret = ret.replace('{to}', target.mask_alias)

    return ret


def generate_single_integer_to_integer_vector_conversion(source: VectorType, target: VectorType):
    conversion_template = """
    template<>
    [[nodiscard]]
    AVEL_FINL std::array<{to}, {size}> convert<{to}, {from}>({from} m) {
        return std::array<{to}, {size}>{{to}{static_cast<{to}::scalar>(decay(m) & {mask})}};
    }
"""

    size = 1
    ret = conversion_template
    ret = ret.replace('{size}', str(size))
    ret = ret.replace('{from}', source.mask_alias)
    ret = ret.replace('{to}', target.mask_alias)

    return ret

    #size0 = source.scalar_type.size
    #size1 = target.scalar_type.size
#
    #if size0 == size1:
    #    return generate_equal_size_integer_conversion(source, target)
#
    #if size0 < size1:
    #    return generate_single_smaller_to_larger_integer_vector_conversion(source, target)
#
    #if size1 > size0:
    #    return generate_single_larger_to_smaller_integer_vector_conversion(source, target)


def generate_vector_conversion(source: VectorType, target: VectorType):
    if is_integer_vector(source) and is_integer_vector(target) and source.width == 1 and target.width == 1:
        return generate_single_integer_to_integer_vector_conversion(source, target)

    if source == target:
        conversion_template = """
    template<>
    [[nodiscard]]
    AVEL_FINL std::array<{to}, {size}> convert<{to}, {from}>({from} m) {
        return std::array<{to}, {size}>{m};
    }
"""
    else:
        conversion_template = """
    template<>
    [[nodiscard]]
    AVEL_FINL std::array<{to}, {size}> convert<{to}, {from}>({from} m) {
        $<convert<{to}>({from})>
    }
"""

    size = max(1, source.width // target.width)
    ret = conversion_template
    ret = ret.replace('{size}', str(size))
    ret = ret.replace('{from}', source.vector_alias)
    ret = ret.replace('{to}', target.vector_alias)

    return ret


def generate_vector_conversions(vector_type: VectorType):
    conversions_from = ""
    conversions_to = ""

    for other_vector in vector_types:
        if vector_type == other_vector:
            break

        conversions_from += generate_vector_conversion(vector_type, other_vector)
        conversions_to += generate_vector_conversion(other_vector, vector_type)

    conversions_from += generate_vector_conversion(vector_type, vector_type)

    return conversions_from + conversions_to
