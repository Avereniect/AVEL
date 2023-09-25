# Capabilities
The use of AVEL requires that you specify which ISA extensions you'd like to be
used via defining a set of macros before any of AVEL's headers are included.
Alternatively, they may be defined at the project level by defining the CMake 
variables by the same name. Most feature sets imply other feature sets, with 
these implications being transitive.  

## x86 ISA Macros
* `AVEL_AVX10_2`
  * implies `AVEL_AVX10_1`
* `AVEL_AVX10_1`
  * implies `AVEL_AVX2`
* `AVEL_GFNI`
  * implies `AVEL_AVX512F`
* `AVEL_AVX512BITALG`
  * implies `AVEL_AVX512F`
* `AVEL_AVX512VBMI2`
  * implies `AVEL_AVX512F`
* `AVEL_AVX512VBMI`
  * implies `AVEL_AVX512F`
* `AVEL_AVX512VPOPCNTDQ`
  * implies `AVEL_AVX512F`
* `AVEL_AVX512BW`
  * implies `AVEL_AVX512F`
* `AVEL_AVX512VL`
  * implies `AVEL_AVX512F`
* `AVEL_AVX512DQ`
  * implies `AVEL_AVX512F`
* `AVEL_AVX512CD`
  * implies `AVEL_AVX512F`
* `AVEL_AVX512F`
  * implies `AVEL_AVX2`
  * implies `AVEL_FMA`
* `AVEL_FMA`
  * implies `AVEL_AVX`
* `AVEL_AVX2`
  * implies `AVEL_AVX`
* `AVEL_AVX`
  * implies `AVEL_SSE42`
* `AVEL_SSE42`
  * implies `AVEL_SSE41`
* `AVEL_SSE41`
  * implies `AVEL_SSSE3`
  * implies `AVEL_POPCNT`
* `AVEL_SSSE3`
  * implies `AVEL_SSE3`
* `AVEL_SSE3`
  * implies `AVEL_SSE2`
* `AVEL_SSE2`
  * implies `AVEL_X86`
* `AVEL_BMI2`
  * implies `AVEL_BMI`
* `AVEL_BMI`
  * implies `AVEL_X86`
* `AVEL_PREFETCH`
  * implies `AVEL_X86`
* `AVEL_LZCNT`
  * implies `AVEL_X86`
* `AVEL_POPCNT`
  * implies `AVEL_X86`
* `AVEL_X86`

## ARM ISA Macros
* `AVEL_AARCH64`
  * implies `AVEL_ARM`
* `AVEL_NEON`
  * implies `AVEL_ARM`
* `AVEL_ARM`