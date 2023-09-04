# AVEL Benchmark Results
Note: Measurements should only be compared relative to others in the same table
Note: Measurements for masks should not be compared between implementations that do and don't have AVX-512
CPU: Intel(R) Core(TM) i7-1065G7 CPU @ 1.30GHz
## popcount_16u
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512BW, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* POPCNT, X86

#### Config 5
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSSE3, SSE3, SSE2, X86

#### Config 6
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512BITALG

#### Config 7
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512BW, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512BITALG

### Results:
| Config | Benchmark                                          | Time     | CPU Time |
| ------ | -------------------------------------------------- | -------- | -------- |
| 0      | popcount_16u::scalar_nibble_lookup                 | 20809ns  | 20808ns  |
| 0      | popcount_16u::scalar_bitwise_accumulate            | 80546ns  | 80543ns  |
| 0      | popcount_16u::scalar_bitwise_divide_and_conquer    | 53837ns  | 53830ns  |
| 0      | popcount_16u::scalar_bitwise_accumulate_branchless | 62735ns  | 62733ns  |
| 0      | popcount_16u::scalar_byte_lookup                   | 177985ns | 177977ns |
| 0      | popcount_16u::scalar_native                        | 27699ns  | 27697ns  |
| 1      | popcount_16u::vec8x16u_bitwise_divide_and_conquer  | 2471ns   | 2471ns   |
| 2      | popcount_16u::vec16x16u_pshufb                     | 965ns    | 965ns    |
| 3      | popcount_16u::vec32x16u_pshufb                     | 506ns    | 506ns    |
| 4      | popcount_16u::scalar_popcnt_op                     | 7510ns   | 7509ns   |
| 5      | popcount_16u::vec8x16u_pshufb                      | 1904ns   | 1904ns   |
| 6      | popcount_16u::vec16x16u_popcnt_op                  | 439ns    | 438ns    |
| 6      | popcount_16u::vec8x16u_popcnt_op                   | 843ns    | 843ns    |
| 7      | popcount_16u::vec32x16u_popcnt_op                  | 227ns    | 227ns    |


## popcount_32u
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* POPCNT, X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSSE3, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512BITALG

#### Config 5
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512VPOPCNTDQ

### Results:
| Config | Benchmark                                          | Time     | CPU Time |
| ------ | -------------------------------------------------- | -------- | -------- |
| 0      | popcount_32u::scalar_bitwise_accumulate            | 8576ns   | 8576ns   |
| 0      | popcount_32u::scalar_byte_lookup                   | 103064ns | 103059ns |
| 0      | popcount_32u::scalar_native                        | 13793ns  | 13792ns  |
| 0      | popcount_32u::scalar_bitwise_accumulate_branchless | 153144ns | 153138ns |
| 0      | popcount_32u::scalar_bitwise_divide_and_conquer    | 10517ns  | 10517ns  |
| 0      | popcount_32u::scalar_nibble_lookup                 | 17463ns  | 17463ns  |
| 1      | popcount_32u::vec4x32u_bitwise_divide_and_conquer  | 3223ns   | 3223ns   |
| 2      | popcount_32u::scalar_popcnt_op                     | 3350ns   | 3350ns   |
| 3      | popcount_32u::vec4x32u_pshufb                      | 2525ns   | 2524ns   |
| 4      | popcount_32u::vec4x32u_pocnt16_op                  | 1277ns   | 1277ns   |
| 5      | popcount_32u::vec4x32u_pocnt_op                    | 851ns    | 851ns    |


## popcount_64u
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* POPCNT, X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSSE3, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512BITALG

#### Config 5
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512VPOPCNTDQ

#### Config 6
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86, POPCNT

#### Config 7
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 8
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512BITALG, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 9
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VPOPCNTDQ, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Benchmark                                                | Time     | CPU Time |
| ------ | -------------------------------------------------------- | -------- | -------- |
| 0      | popcount_64u::scalar_native                              | 7351ns   | 7350ns   |
| 0      | popcount_64u::scalar_byte_lookup                         | 72576ns  | 72575ns  |
| 0      | popcount_64u::scalar_bitwise_divide_and_conquer          | 8475ns   | 8475ns   |
| 0      | popcount_64u::scalar_nibble_lookup                       | 17395ns  | 17395ns  |
| 0      | popcount_64u::scalar_bitwise_accumulate_branchless       | 135606ns | 135603ns |
| 0      | popcount_64u::scalar_bitwise_accumulate                  | 4318ns   | 4317ns   |
| 1      | popcount_64u::vec2x64u_bitwise_divide_and_conquer        | 3633ns   | 3633ns   |
| 2      | popcount_64u::scalar_popcnt_op                           | 1682ns   | 1681ns   |
| 3      | popcount_64u::vec2x64u_pshufb                            | 2515ns   | 2515ns   |
| 4      | popcount_64u::vec4x64u_popcnt16_op                       | 685ns    | 685ns    |
| 4      | popcount_64u::vec2x64u_popcnt16_op                       | 1274ns   | 1274ns   |
| 5      | popcount_64u::vec4x64u_popcnt_op                         | 428ns    | 428ns    |
| 5      | popcount_64u::vec2x64u_popcnt_op                         | 846ns    | 846ns    |
| 6      | popcount_64u::vec2x64u_scalarized_popcnt_op              | 1823ns   | 1823ns   |
| 7      | popcount_64u::vec2x64u_scalarized_popcnt_op_with_extract | 1824ns   | 1824ns   |
| 8      | popcount_64u::vec8x64u_popcnt16_op                       | 343ns    | 343ns    |
| 9      | popcount_64u::vec8x64u_popcnt_op                         | 239ns    | 238ns    |


## mask_from_bool_8i
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512BW, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512BW, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512VL

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Benchmark                             | Time    | CPU Time |
| ------ | ------------------------------------- | ------- | -------- |
| 0      | mask_from_bool_8i::scalar_ternary     | 12120ns | 12120ns  |
| 0      | mask_from_bool_8i::scalar_negate_bool | 11467ns | 11467ns  |
| 1      | mask_from_bool_8i::mask64x8i_negate   | 190ns   | 190ns    |
| 2      | mask_from_bool_8i::mask16x8i_set1     | 1130ns  | 1130ns   |
| 2      | mask_from_bool_8i::mask16x8i_ternary  | 1439ns  | 1438ns   |
| 3      | mask_from_bool_8i::mask32x8i_negate   | 427ns   | 427ns    |
| 3      | mask_from_bool_8i::mask16x8i_negate   | 752ns   | 751ns    |
| 4      | mask_from_bool_8i::mask32x8i_set1     | 537ns   | 537ns    |
| 4      | mask_from_bool_8i::mask32x8i_ternary  | 756ns   | 756ns    |


## fpclassify_32f
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 5
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512DQ

#### Config 6
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512DQ, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Benchmark                                   | Time    | CPU Time |
| ------ | ------------------------------------------- | ------- | -------- |
| 0      | fpclassify_32f::scalar_blending0            | 23603ns | 23602ns  |
| 0      | fpclassify_32f::scalar_native               | 7712ns  | 7711ns   |
| 0      | fpclassify_32f::scalar_branching            | 6685ns  | 6685ns   |
| 0      | fpclassify_32f::scalar_blending1            | 16655ns | 16654ns  |
| 1      | fpclassify_32f::vec4x32f_blending           | 2652ns  | 2652ns   |
| 1      | fpclassify_32f::vec4x32f_avel               | 2623ns  | 2622ns   |
| 2      | fpclassify_32f::vec8x32f_blending           | 1332ns  | 1332ns   |
| 3      | fpclassify_32f::vec16x32f_blending_ternlog  | 954ns   | 954ns    |
| 4      | fpclassify_32f::vec8x32f_blending_ternlog   | 1217ns  | 1217ns   |
| 4      | fpclassify_32f::vec4x32f_blending_ternlog   | 2353ns  | 2353ns   |
| 5      | fpclassify_32f::vec4x32f_blending_vfpclass  | 3157ns  | 3156ns   |
| 5      | fpclassify_32f::vec8x32f_blending_vfpclass  | 1708ns  | 1708ns   |
| 6      | fpclassify_32f::vec16x32f_blending_vfpclass | 808ns   | 808ns    |


## bit_ceil_8u
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* LZCNT, X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 5
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512BW, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 6
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSSE3, SSE3, SSE2, X86

#### Config 7
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512BW, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512VBMI

### Results:
| Config | Benchmark                           | Time    | CPU Time |
| ------ | ----------------------------------- | ------- | -------- |
| 0      | bit_ceil_8u::scalar_native          | 36443ns | 36442ns  |
| 0      | bit_ceil_8u::scalar_bitwise         | 31860ns | 31859ns  |
| 1      | bit_ceil_8u::scalar_bsr_and_shift   | 40844ns | 40837ns  |
| 2      | bit_ceil_8u::scalar_lzcnt_and_shift | 21885ns | 21884ns  |
| 3      | bit_ceil_8u::vec16x8u_bitwise       | 2672ns  | 2672ns   |
| 4      | bit_ceil_8u::vec32x8u_pshufb        | 881ns   | 881ns    |
| 5      | bit_ceil_8u::vec64x8u_pshufb        | 621ns   | 621ns    |
| 6      | bit_ceil_8u::vec16x8u_pshufb        | 2109ns  | 2109ns   |
| 7      | bit_ceil_8u::vec64x8u_vpermi2b      | 627ns   | 627ns    |


## popcount_8u
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512BW, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* POPCNT, X86

#### Config 5
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSSE3, SSE3, SSE2, X86

#### Config 6
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512BITALG

#### Config 7
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512BW, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512BITALG

### Results:
| Config | Benchmark                                         | Time     | CPU Time |
| ------ | ------------------------------------------------- | -------- | -------- |
| 0      | popcount_8u::scalar_bitwise_accumulate_branchless | 59883ns  | 59882ns  |
| 0      | popcount_8u::scalar_native                        | 56057ns  | 56056ns  |
| 0      | popcount_8u::scalar_bitwise_accumulate            | 166003ns | 165984ns |
| 0      | popcount_8u::scalar_byte_lookup                   | 109847ns | 109844ns |
| 0      | popcount_8u::scalar_bitwise_divide_and_conquer    | 33498ns  | 33497ns  |
| 0      | popcount_8u::scalar_nibble_lookup                 | 20915ns  | 20914ns  |
| 1      | popcount_8u::vec16x8u_bitwise_divide_and_conquer  | 1954ns   | 1954ns   |
| 2      | popcount_8u::vec32x8u_pshufb                      | 764ns    | 764ns    |
| 3      | popcount_8u::vec64x8u_pshufb                      | 351ns    | 351ns    |
| 4      | popcount_8u::scalar_popcnt_op                     | 11815ns  | 11814ns  |
| 5      | popcount_8u::vec16x8u_pshufb                      | 1480ns   | 1479ns   |
| 6      | popcount_8u::vec32x8u_popcnt_op                   | 439ns    | 439ns    |
| 6      | popcount_8u::vec16x8u_popcnt_op                   | 847ns    | 847ns    |
| 7      | popcount_8u::vec64x8u_popcnt_op                   | 225ns    | 225ns    |


## fpclassify_64f
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512DQ, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 5
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Benchmark                                  | Time   | CPU Time |
| ------ | ------------------------------------------ | ------ | -------- |
| 0      | fpclassify_64f::scalar_native              | 3526ns | 3526ns   |
| 0      | fpclassify_64f::scalar_branching           | 2676ns | 2676ns   |
| 1      | fpclassify_64f::vec2x64f_blend_sse2        | 2708ns | 2708ns   |
| 1      | fpclassify_64f::vec2x64f_scalarized        | 9381ns | 9381ns   |
| 2      | fpclassify_64f::vec4x64f_lend_avx2         | 1322ns | 1322ns   |
| 3      | fpclassify_64f::vec8x64f_blend_avx512f     | 1034ns | 1034ns   |
| 4      | fpclassify_64f::vec8x64f_blending_vfpclass | 811ns  | 811ns    |
| 5      | fpclassify_64f::vec2x64f_blend_sse42       | 2630ns | 2629ns   |


## bit_ceil_16u
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* LZCNT, X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 5
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512BW, AVX512CD

#### Config 6
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512BW, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Benchmark                            | Time    | CPU Time |
| ------ | ------------------------------------ | ------- | -------- |
| 0      | bit_ceil_16u::scalar_bitwise         | 20342ns | 20341ns  |
| 0      | bit_ceil_16u::scalar_native          | 18627ns | 18626ns  |
| 1      | bit_ceil_16u::scalar_bsr_and_shift   | 20583ns | 20579ns  |
| 2      | bit_ceil_16u::scalar_lzcnt_and_shift | 12043ns | 12043ns  |
| 3      | bit_ceil_16u::vec8x16u_bitwise       | 2584ns  | 2584ns   |
| 4      | bit_ceil_16u::vec16x16u_bitwise      | 1109ns  | 1109ns   |
| 5      | bit_ceil_16u::vec8x16u_lzcnt32       | 2192ns  | 2192ns   |
| 6      | bit_ceil_16u::vec32x16u_bitwise      | 734ns   | 734ns    |


## scalbn_32f
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Benchmark                  | Time    | CPU Time |
| ------ | -------------------------- | ------- | -------- |
| 0      | scalbn_32f::scalar_native  | 12690ns | 12689ns  |
| 1      | scalbn_32f::vec4x32f_avel  | 3377ns  | 3376ns   |
| 2      | scalbn_32f::vec8x32f_avel  | 1080ns  | 1080ns   |
| 3      | scalbn_32f::vec16x32f_avel | 183ns   | 183ns    |


## bit_ceil_64u
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* LZCNT, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512CD

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 5
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Benchmark                          | Time   | CPU Time |
| ------ | ---------------------------------- | ------ | -------- |
| 0      | bit_ceil_64u::scalar_bitwise       | 7166ns | 7166ns   |
| 0      | bit_ceil_64u::scalar_native        | 3298ns | 3298ns   |
| 1      | bit_ceil_64u::scalar_lzcnt         | 3507ns | 3507ns   |
| 1      | bit_ceil_64u::scalar_bsr_and_shift | 3451ns | 3450ns   |
| 2      | bit_ceil_64u::vec2x64u_bitwise     | 3079ns | 3079ns   |
| 3      | bit_ceil_64u::vec8x64u_lzcnt       | 441ns  | 440ns    |
| 3      | bit_ceil_64u::vec4x64u_lzcnt       | 975ns  | 975ns    |
| 3      | bit_ceil_64u::vec2x64u_lzcnt       | 1540ns | 1539ns   |
| 4      | bit_ceil_64u::vec4x64u_bitwise     | 1279ns | 1279ns   |
| 5      | bit_ceil_64u::vec8x64u_bitwise     | 903ns  | 903ns    |


## bit_ceil_32u
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* LZCNT, X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512CD

#### Config 5
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 6
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 7
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512CD, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Benchmark                            | Time    | CPU Time |
| ------ | ------------------------------------ | ------- | -------- |
| 0      | bit_ceil_32u::scalar_native          | 6581ns  | 6580ns   |
| 0      | bit_ceil_32u::scalar_bitwise         | 12391ns | 12391ns  |
| 1      | bit_ceil_32u::scalar_bsr_and_shift   | 7115ns  | 7115ns   |
| 2      | bit_ceil_32u::scalar_lzcnt_and_shift | 5990ns  | 5990ns   |
| 3      | bit_ceil_32u::vec4x32u_bitwise       | 2634ns  | 2633ns   |
| 4      | bit_ceil_32u::vec4x32u_lzcnt         | 1329ns  | 1329ns   |
| 4      | bit_ceil_32u::vec8x32u_lzcnt         | 684ns   | 684ns    |
| 5      | bit_ceil_32u::vec8x32u_bitwise       | 1231ns  | 1231ns   |
| 6      | bit_ceil_32u::vec16x32u_bitwise      | 919ns   | 919ns    |
| 7      | bit_ceil_32u::vec16x32u_lzcnt        | 339ns   | 339ns    |

