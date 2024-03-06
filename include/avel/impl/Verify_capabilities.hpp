#ifndef AVEL_VERIFY_CAPABILITIES_HPP
#define AVEL_VERIFY_CAPABILITIES_HPP

#if defined(AVEL_GFNI) && !defined(__GFNI__)
static_assert(false, "GFNI feature flag was not passed to compiler");
#endif

#if defined(AVEL_AVX512VMI2) && !defined(__AVX512VBMI2__)
static_assert(false, "AVX-512VBMI2 feature flag was not passed to compiler");
#endif

#if defined(AVEL_AVX512VBMI) && !defined(__AVX512VBMI__)
static_assert(false, "AVX-512VBMI feature flag was not passed to compiler");
#endif

#if defined(AVEL_AVX512BITALG) && !defined(__AVX512BITALG__)
static_assert(false, "AVX-512BITALG feature flag was not passed to compiler");
#endif

#if defined(AVEL_AVX512VPOPCNTDQ) && !defined(__AVX512VPOPCNTDQ__)
static_assert(false, "AVX-512VPOPCNTDQ feature flag was not passed to compiler");
#endif

#if defined(AVEL_AVX512CD) && !defined(__AVX512CD__)
static_assert(false, "AVX-512VCD feature flag was not passed to compiler");
#endif

#if defined(AVEL_AVX512VL) && !defined(__AVX512VL__)
static_assert(false, "AVX-512VL feature flag was not passed to compiler");
#endif

#if defined(AVEL_AVX512DQ) && !defined(__AVX512DQ__)
static_assert(false, "AVX-512DQ feature flag was not passed to compiler");
#endif

#if defined(AVEL_AVX512BW) && !defined(__AVX512BW__)
static_assert(false, "AVX-512BW feature flag was not passed to compiler");
#endif

#if defined(AVEL_AVX512F) && !defined(__AVX512F__)
static_assert(false, "AVX-512F feature flag was not passed to compiler");
#endif

#if defined(AVEL_FMA) && !defined(__FMA__)
static_assert(false, "FMA feature flag was not passed to compiler");
#endif

#if defined(AVEL_AVX2) && !defined(__AVX2__)
static_assert(false, "AVX2 feature flag was not passed to compiler");
#endif

#if defined(AVEL_AVX) && !defined(__AVX__)
static_assert(false, "AVX feature flag was not passed to compiler");
#endif

#if defined(AVEL_SSE4_2) && !defined(__SSE4_2__)
static_assert(false, "SSE4.2 feature flag was not passed to compiler");
#endif

#if defined(AVEL_SSE4_1) && !defined(__SSE4_1__)
static_assert(false, "SSE4.1 feature flag was not passed to compiler");
#endif

#if defined(AVEL_SSSE3) && !defined(__SSSE3__)
static_assert(false, "SSSE3 feature flag was not passed to compiler");
#endif

#if defined(AVEL_SSE3) && !defined(__SSE3__)
static_assert(false, "SSE3 feature flag was not passed to compiler");
#endif

#if defined(AVEL_SSE2) && !defined(__SSE2__)
static_assert(false, "SSE2 feature flag was not passed to compiler");
#endif

#if defined(AVEL_SSE) && !defined(__SSE__)
static_assert(false, "SSE feature flag was not passed to compiler");
#endif

#if defined(AVEL_BMI2) && !defined(__BMI2__)
static_assert(false, "BMI2 feature flag was not passed to compiler");
#endif

#if defined(AVEL_BMI) && !defined(__BMI__)
static_assert(false, "BMI feature flag was not passed to compiler");
#endif

#if defined(AVEL_LZCNT) && !defined(__LZCNT__)
static_assert(false, "LZCNT feature flag was not passed to compiler");
#endif

#if defined(AVEL_POPCNT) && !defined(__POPCNT__)
static_assert(false, "POPCNT feature flag was not passed to compiler");
#endif

#if defined(AVEL_PREFETCH) && !defined(__PREFETCH__)
static_assert(false, "POPCNT feature flag was not passed to compiler");
#endif



// ARM capabilities

#if defined(AVEL_AARCH64) && !defined(__ARM_ARCH_ISA_A64)
static_assert(false, "Compiler not targetting Aarch64");
#endif

#if defined(AVEL_NEON) && !defined(__ARM_NEON)
static_assert(false, "Neon feature flag was not passed to compiler");
#endif

#endif //AVEL_VERIFY_CAPABILITIES_HPP
