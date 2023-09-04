//#define AVEL_FORCE_INLINE
//#define AVEL_AUTO_DETECT
//#define AVEL_GFNI
//#define AVEL_AVX512VBMI2
//#define AVEL_AVX512VBMI
//#define AVEL_AVX512VPOPCNTDQ
//#define AVEL_AVX512BITALG
//#define AVEL_AVX512BW
//#define AVEL_AVX512VL
//#define AVEL_AVX512DQ
//#define AVEL_AVX512CD
//#define AVEL_AVX512F
//#define AVEL_FMA
//#define AVEL_AVX2
//#define AVEL_AVX
//#define AVEL_SSE42
//#define AVEL_SSE41
//#define AVEL_SSSE3
//#define AVEL_SSE3
//#define AVEL_SSE2
//#define AVEL_BMI2
//#define AVEL_BMI
//#define AVEL_LZCNT
//#define AVEL_POPCNT
//#define AVEL_X86

//#define AVEL_AARCH64
//#define AVEL_NEON

#include <avel/Avel.hpp>

#include <cstdint>
#include <bit>

#include "AVEL_benchmarks.hpp"

//#include "average_uint8/Average_uint8.hpp"
//#include "division_uint8/Division_uint8.hpp"
//#include "midpoint_uint8/Midpoint_uint8.hpp"

//#include "division_uint16/Division_uint16.hpp"
//#include "countr_zero_uint16/Countr_zero_uint16.hpp"

//#include "Division_uint8.hpp"
//#include "Division_uint16.hpp"
//#include "Division_uint32.hpp"

//#include "Mulhi_uint8.hpp"
//#include "Mulhi_uint16.hpp"e

//#include "Popcnt_uint64.hpp"

//#include "Isqrt_uint8.hpp"
//#include "Isqrt_uint16.hpp"
//#include "Isqrt_uint32.hpp"
//#include "Isqrt_uint64.hpp"

//#include "fpclassify_32f.hpp"
//#include "fpclassify_64f.hpp"

//#include "Mask_from_bool_8i.hpp"
//#include "Mask_from_bool_16i.hpp"
//#include "Mask_from_bool_32i.hpp"
//#include "Mask_from_bool_64i.hpp"

//TODO: Consider using conversions to 32-bit and 64-bit integers
//#include "Bit_ceil_8u.hpp"
//#include "Bit_ceil_16u.hpp"
//#include "Bit_ceil_32u.hpp"
//#include "Bit_ceil_64u.hpp"

//#include "Popcount_8u.hpp"
//#include "Popcount_16u.hpp"
//#include "Popcount_32u.hpp"
//#include "Popcount_64u.hpp"

//#include "scalbn_32f.hpp"

//#include "Memory_access.hpp"

// TODO: Use RegisterBenchmark() to programmatically register benchmarks

#include "AVEL_benchmarks_include.hpp"

BENCHMARK_MAIN();
