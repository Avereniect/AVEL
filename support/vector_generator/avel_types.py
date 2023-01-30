from enum import Enum


class TypeCategory(Enum):
    Unsigned_int = 0,
    Signed_int = 1,
    Float = 2


def to_string(type_category: TypeCategory):
    match type_category:
        case TypeCategory.Unsigned_int:
            return 'uint'
        case TypeCategory.Signed_int:
            return 'sint'
        case TypeCategory.Float:
            return 'float'
        case _:
            raise Exception('Unrecognized enum value')


class ScalarType:
    full_name: str = None
    abbreviation: str = None
    type_category: TypeCategory = None
    size: int = None

    def __init__(self, full_name, abbreviation, type_category, size):
        self.full_name = full_name
        self.abbreviation = abbreviation
        self.type_category = type_category
        self.size = size


uint8_t  = ScalarType('std::uint8_t',  '8U',  TypeCategory.Unsigned_int, 1)
uint16_t = ScalarType('std::uint16_t', '16U', TypeCategory.Unsigned_int, 2)
uint32_t = ScalarType('std::uint32_t', '32U', TypeCategory.Unsigned_int, 4)
uint64_t = ScalarType('std::uint64_t', '64U', TypeCategory.Unsigned_int, 8)

int8_t  = ScalarType('std::int8_t',  '8I',  TypeCategory.Signed_int, 1)
int16_t = ScalarType('std::int16_t', '16I', TypeCategory.Signed_int, 2)
int32_t = ScalarType('std::int32_t', '32I', TypeCategory.Signed_int, 4)
int64_t = ScalarType('std::int64_t', '64I', TypeCategory.Signed_int, 8)

float32_t = ScalarType('float',  '32F', TypeCategory.Float, 4)
float64_t = ScalarType('double', '64F', TypeCategory.Float, 8)

scalar_types = [
    uint8_t,
    uint16_t,
    uint32_t,
    uint64_t,

    int8_t,
    int16_t,
    int32_t,
    int64_t,

    float32_t,
    float64_t
]


def to_unsigned(input_type):
    table = {
        uint8_t:  uint8_t,
        uint16_t: uint16_t,
        uint32_t: uint32_t,
        uint64_t: uint64_t,

        int8_t:  uint8_t,
        int16_t: uint16_t,
        int32_t: uint32_t,
        int64_t: uint64_t,

        float32_t: uint32_t,
        float64_t: uint64_t
    }

    return table[input_type]


def to_signed(input_type):
    table = {
        uint8_t:  int8_t,
        uint16_t: int16_t,
        uint32_t: int32_t,
        uint64_t: int64_t,

        int8_t:  int8_t,
        int16_t: int16_t,
        int32_t: int32_t,
        int64_t: int64_t,

        float32_t: int32_t,
        float64_t: int64_t
    }

    return table[input_type]


class VectorType:
    width: int = 0
    alignment: int = None

    scalar_type: ScalarType = None

    vector_alias: str = None
    array_alias: str = None
    mask_alias: str = None

    output_file_name: str = None
    input_files: list = None
    template_file_name: str = None

    uvector_alias: str = None
    svector_alias: str = None

    umask_alias: str = None

    header_guard: str = None

    variables: [str] = None

    def __init__(self, scalar_type: ScalarType, width: int, input_files, variables):
        self.width = width
        self.alignment = scalar_type.size * width

        self.scalar_type = scalar_type

        self.vector_alias = 'vec' + str(width) + 'x' + scalar_type.abbreviation.lower()
        self.array_alias = 'arr' + str(width) + 'x' + scalar_type.abbreviation.lower()
        self.mask_alias = 'mask' + str(width) + 'x' + scalar_type.abbreviation.lower()

        self.output_file_name = 'Vec' + str(width) + 'x' + scalar_type.abbreviation.lower() + '.hpp'
        self.input_files = input_files
        self.template_file_name = to_string(scalar_type.type_category) + '_vector.txt'

        self.uvector_alias = 'vec' + str(width) + 'x' + to_unsigned(scalar_type).abbreviation.lower()
        self.svector_alias = 'vec' + str(width) + 'x' + to_signed(scalar_type).abbreviation.lower()

        self.umask_alias = 'mask' + str(width) + 'x' + to_unsigned(scalar_type).abbreviation.lower()

        self.header_guard = 'AVEL_VEC' + str(width) + 'X' + scalar_type.abbreviation.upper() + '_HPP'

        self.variables = variables


def is_integer_vector(vector_type: VectorType) -> bool:
    a = vector_type.scalar_type.type_category == TypeCategory.Unsigned_int
    b = vector_type.scalar_type.type_category == TypeCategory.Signed_int
    return a or b


vector_types = [
    # Native vectors
    VectorType(uint8_t,  1, ['single_mask_implementations.cpp', 'single_int_vectors_implementations.cpp', 'single_uint_vectors_implementations.cpp'], []),
    VectorType(int8_t,  1, ['single_mask_implementations.cpp', 'single_int_vectors_implementations.cpp', 'single_sint_vectors_implementations.cpp'], []),

    VectorType(uint16_t, 1, ['single_mask_implementations.cpp', 'single_int_vectors_implementations.cpp', 'single_uint_vectors_implementations.cpp'], []),
    VectorType(int16_t, 1, ['single_mask_implementations.cpp', 'single_int_vectors_implementations.cpp', 'single_sint_vectors_implementations.cpp'], []),

    VectorType(uint32_t, 1, ['single_mask_implementations.cpp', 'single_int_vectors_implementations.cpp', 'single_uint_vectors_implementations.cpp'], ['HAS_SCATTER', 'HAS_GATHER']),
    VectorType(int32_t, 1, ['single_mask_implementations.cpp', 'single_int_vectors_implementations.cpp', 'single_sint_vectors_implementations.cpp'], ['HAS_SCATTER', 'HAS_GATHER']),

    VectorType(uint64_t, 1, ['single_mask_implementations.cpp', 'single_int_vectors_implementations.cpp', 'single_uint_vectors_implementations.cpp'], ['HAS_SCATTER', 'HAS_GATHER']),
    VectorType(int64_t, 1, ['single_mask_implementations.cpp', 'single_int_vectors_implementations.cpp', 'single_sint_vectors_implementations.cpp'], ['HAS_SCATTER', 'HAS_GATHER']),

#    VectorType(float32_t, 1, ["single_mask_implementations.cpp"], ['HAS_SCATTER', 'HAS_GATHER']),
#    VectorType(float64_t, 1, ["single_mask_implementations.cpp"], ['HAS_SCATTER', 'HAS_GATHER']),

    # 128-bit vectors
    VectorType(uint8_t, 16, [], ['MULTIPLE']),
    VectorType(int8_t, 16, [], ['MULTIPLE']),

    VectorType(uint16_t, 8, [], ['MULTIPLE']),
    VectorType(int16_t, 8, [], ['MULTIPLE']),

    VectorType(uint32_t, 4, [], ['MULTIPLE', 'HAS_SCATTER', 'HAS_GATHER']),
    VectorType(int32_t, 4, [], ['MULTIPLE', 'HAS_SCATTER', 'HAS_GATHER']),

    VectorType(int64_t, 2, [], ['MULTIPLE', 'HAS_SCATTER', 'HAS_GATHER']),
    VectorType(uint64_t, 2, [], ['MULTIPLE', 'HAS_SCATTER', 'HAS_GATHER']),

#    VectorType(float32_t, 4, [], ['MULTIPLE', 'HAS_SCATTER', 'HAS_GATHER']),
#    VectorType(float64_t, 2, [], ['MULTIPLE', 'HAS_SCATTER', 'HAS_GATHER']),

    # 256-bit vectors
    VectorType(uint8_t,  32, [], ['MULTIPLE']),
    VectorType(int8_t,  32, [], ['MULTIPLE']),

    VectorType(uint16_t, 16, [], ['MULTIPLE']),
    VectorType(int16_t, 16, [], ['MULTIPLE']),

    VectorType(uint32_t,  8, [], ['MULTIPLE', 'HAS_SCATTER', 'HAS_GATHER']),
    VectorType(int32_t,  8, [], ['MULTIPLE', 'HAS_SCATTER', 'HAS_GATHER']),

    VectorType(uint64_t,  4, [], ['MULTIPLE', 'HAS_SCATTER', 'HAS_GATHER']),
    VectorType(int64_t,  4, [], ['MULTIPLE', 'HAS_SCATTER', 'HAS_GATHER']),

#    VectorType(float32_t, 8, [], ['MULTIPLE', 'HAS_SCATTER', 'HAS_GATHER']),
#    VectorType(float64_t, 4, [], ['MULTIPLE', 'HAS_SCATTER', 'HAS_GATHER']),

    # 512-bit vectors
    VectorType(uint8_t,  64, [], ['MULTIPLE']),
    VectorType(int8_t,  64, [], ['MULTIPLE']),

    VectorType(uint16_t, 32, [], ['MULTIPLE']),
    VectorType(int16_t, 32, [], ['MULTIPLE']),

    VectorType(uint32_t, 16, [], ['MULTIPLE', 'HAS_SCATTER', 'HAS_GATHER']),
    VectorType(int32_t, 16, [], ['MULTIPLE', 'HAS_SCATTER', 'HAS_GATHER']),

    VectorType(uint64_t,  8, [], ['MULTIPLE', 'HAS_SCATTER', 'HAS_GATHER']),
    VectorType(int64_t,  8, [], ['MULTIPLE', 'HAS_SCATTER', 'HAS_GATHER']),

#    VectorType(float32_t, 16, [], ['MULTIPLE', 'HAS_SCATTER', 'HAS_GATHER']),
#    VectorType(float64_t, 8,  [], ['MULTIPLE', 'HAS_SCATTER', 'HAS_GATHER'])
]
