# AVEL Benchmark Results
Note: Measurements should only be compared relative to others in the same table
Note: Measurements for masks should not be compared between configurations that do and don't have AVX-512
CPU: Intel(R) Core(TM) i7-1065G7 CPU @ 1.30GHz
## bit_ceil_16u
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
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* X86

#### Config 5
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* LZCNT, X86

#### Config 6
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512BW, AVX512CD

### Results:
| Config | Implementation                        | Time    | CPU Time |
| ------ | ------------------------------------- | ------- | -------- |
| 0      | bit_ceil_16u::scalar_bitwise          | 19193ns | 19178ns  |
| 0      | bit_ceil_16u::scalar_native           | 14957ns | 14949ns  |
| 1      | bit_ceil_16u::vec32x16u_bitwise       | 703ns   | 703ns    |
| 2      | bit_ceil_16u::vec8x16u_bitwise        | 1942ns  | 1940ns   |
| 3      | bit_ceil_16u::vec16x16u_bitwise       | 990ns   | 989ns    |
| 4      | bit_ceil_16u::scalar_bsr_and_shift    | 10490ns | 10491ns  |
| 5      | bit_ceil_16u::scalar_lzcnt_and_rotate | 10190ns | 10191ns  |
| 6      | bit_ceil_16u::vec8x16u_lzcnt32        | 2113ns  | 2113ns   |


## bit_ceil_32u
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
* X86

#### Config 5
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* LZCNT, X86

#### Config 6
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512CD

#### Config 7
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512CD, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                                | Time    | CPU Time |
| ------ | --------------------------------------------- | ------- | -------- |
| 0      | bit_ceil_32u::scalar_native                   | 5069ns  | 5065ns   |
| 0      | bit_ceil_32u::scalar_bitwise                  | 11214ns | 11208ns  |
| 1      | bit_ceil_32u::vec4x32u_bitwise                | 2220ns  | 2218ns   |
| 2      | bit_ceil_32u::vec8x32u_bitwise                | 1129ns  | 1128ns   |
| 3      | bit_ceil_32u::vec16x32u_bitwise               | 786ns   | 787ns    |
| 4      | bit_ceil_32u::scalar_bsr_and_shift_branchless | 5205ns  | 5206ns   |
| 4      | bit_ceil_32u::scalar_bsr_and_shift            | 5288ns  | 5288ns   |
| 5      | bit_ceil_32u::scalar_lzcnt_and_shift          | 4797ns  | 4798ns   |
| 5      | bit_ceil_32u::scalar_lzcnt_and_rotate         | 6335ns  | 6336ns   |
| 6      | bit_ceil_32u::vec4x32u_lzcnt                  | 1329ns  | 1330ns   |
| 6      | bit_ceil_32u::vec8x32u_lzcnt                  | 680ns   | 680ns    |
| 7      | bit_ceil_32u::vec16x32u_lzcnt                 | 321ns   | 321ns    |


## bit_ceil_64u
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
* LZCNT, X86

#### Config 5
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512CD

### Results:
| Config | Implementation                        | Time   | CPU Time |
| ------ | ------------------------------------- | ------ | -------- |
| 0      | bit_ceil_64u::scalar_bitwise          | 6714ns | 6677ns   |
| 0      | bit_ceil_64u::scalar_native           | 2533ns | 2531ns   |
| 1      | bit_ceil_64u::vec2x64u_bitwise        | 2538ns | 2538ns   |
| 2      | bit_ceil_64u::vec4x64u_bitwise        | 1126ns | 1126ns   |
| 3      | bit_ceil_64u::vec8x64u_bitwise        | 788ns  | 788ns    |
| 4      | bit_ceil_64u::scalar_lzcnt_and_rotate | 2564ns | 2564ns   |
| 4      | bit_ceil_64u::scalar_bsr_and_shift    | 2439ns | 2439ns   |
| 5      | bit_ceil_64u::vec4x64u_lzcnt          | 922ns  | 922ns    |
| 5      | bit_ceil_64u::vec2x64u_lzcnt          | 1455ns | 1455ns   |
| 5      | bit_ceil_64u::vec8x64u_lzcnt          | 381ns  | 381ns    |


## bit_ceil_8u
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
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* X86

#### Config 5
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* LZCNT, X86

#### Config 6
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSSE3, SSE3, SSE2, X86

#### Config 7
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512BW, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512VBMI

### Results:
| Config | Implementation                       | Time    | CPU Time |
| ------ | ------------------------------------ | ------- | -------- |
| 0      | bit_ceil_8u::scalar_bitwise          | 31110ns | 31093ns  |
| 0      | bit_ceil_8u::scalar_native           | 30295ns | 30313ns  |
| 1      | bit_ceil_8u::vec64x8u_pshufb         | 595ns   | 595ns    |
| 2      | bit_ceil_8u::vec16x8u_bitwise        | 1618ns  | 1618ns   |
| 3      | bit_ceil_8u::vec32x8u_pshufb         | 807ns   | 806ns    |
| 4      | bit_ceil_8u::scalar_bsr_and_shift    | 20808ns | 20807ns  |
| 5      | bit_ceil_8u::scalar_lzcnt_and_rotate | 19157ns | 19158ns  |
| 6      | bit_ceil_8u::vec16x8u_pshufb         | 1609ns  | 1609ns   |
| 7      | bit_ceil_8u::vec64x8u_vpermi2b       | 589ns   | 589ns    |


## comparison_equals_16u
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86, AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512BW

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512BW, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                            | Time   | CPU Time |
| ------ | ----------------------------------------- | ------ | -------- |
| 0      | comparison_equals_16u::scalar_native      | 4241ns | 4238ns   |
| 1      | comparison_equals_16u::vec8x16u_cmpeqw    | 544ns  | 544ns    |
| 1      | comparison_equals_16u::vec16x16u_cmpeqw   | 266ns  | 266ns    |
| 2      | comparison_equals_16u::vec16x16u_cmpeqw_k | 273ns  | 272ns    |
| 2      | comparison_equals_16u::vec8x16u_cmpeqw_k  | 514ns  | 514ns    |
| 3      | comparison_equals_16u::vec32x16u_cmpeqw_k | 149ns  | 149ns    |
| 3      | comparison_equals_16u::vec16x16u_cmpeqw   | 276ns  | 276ns    |
| 3      | comparison_equals_16u::vec8x16u_cmpeqw    | 519ns  | 519ns    |


## comparison_equals_32f
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                           | Time   | CPU Time |
| ------ | ---------------------------------------- | ------ | -------- |
| 0      | comparison_equals_32f::scalar_native     | 2418ns | 2417ns   |
| 1      | comparison_equals_32f::vec16x32f_vcmpps  | 146ns  | 146ns    |
| 2      | comparison_equals_32f::vec4x32f_cmpps    | 517ns  | 517ns    |
| 3      | comparison_equals_32f::vec4x32f_cmpeqd_k | 511ns  | 511ns    |
| 3      | comparison_equals_32f::vec8x32f_vcmpps   | 271ns  | 271ns    |
| 4      | comparison_equals_32f::vec8x32f_cmpeqd   | 266ns  | 266ns    |


## comparison_equals_32u
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                            | Time   | CPU Time |
| ------ | ----------------------------------------- | ------ | -------- |
| 0      | comparison_equals_32u::scalar_native      | 2144ns | 2143ns   |
| 1      | comparison_equals_32u::vec16x32u_cmpeqd_k | 143ns  | 143ns    |
| 2      | comparison_equals_32u::vec4x32u_cmpeqd    | 537ns  | 537ns    |
| 3      | comparison_equals_32u::vec8x32u_cmpeqd_k  | 276ns  | 276ns    |
| 3      | comparison_equals_32u::vec4x32u_cmpeqd_k  | 515ns  | 515ns    |
| 4      | comparison_equals_32u::vec8x32u_cmpeqd    | 264ns  | 264ns    |


## comparison_equals_64f
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                           | Time   | CPU Time |
| ------ | ---------------------------------------- | ------ | -------- |
| 0      | comparison_equals_64f::scalar_native     | 1067ns | 1066ns   |
| 1      | comparison_equals_64f::vec8x64f_vcmppd   | 139ns  | 140ns    |
| 2      | comparison_equals_64f::vec2x64f_cmppd    | 609ns  | 609ns    |
| 3      | comparison_equals_64f::vec2x64f_cmpltd_k | 542ns  | 542ns    |
| 3      | comparison_equals_64f::vec4x64f_vcmppd   | 260ns  | 260ns    |
| 4      | comparison_equals_64f::vec4x64f_cmpltd   | 272ns  | 272ns    |


## comparison_equals_8u
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86, AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512BW

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512BW, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                          | Time   | CPU Time |
| ------ | --------------------------------------- | ------ | -------- |
| 0      | comparison_equals_8u::scalar_native     | 8145ns | 8137ns   |
| 1      | comparison_equals_8u::vec32x8u_cmpeqb   | 262ns  | 262ns    |
| 1      | comparison_equals_8u::vec16x8u_cmpeqb   | 608ns  | 608ns    |
| 2      | comparison_equals_8u::vec32x8u_cmpeqb_k | 262ns  | 262ns    |
| 2      | comparison_equals_8u::vec16x8u_cmpeqb_k | 512ns  | 511ns    |
| 3      | comparison_equals_8u::vec64x8u_cmpeqb_k | 141ns  | 141ns    |
| 3      | comparison_equals_8u::vec16x8u_cmpeqb   | 610ns  | 610ns    |
| 3      | comparison_equals_8u::vec32x8u_cmpeqb   | 264ns  | 264ns    |


## comparison_less_than_16i
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86, AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512BW

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512BW, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                              | Time   | CPU Time |
| ------ | ------------------------------------------- | ------ | -------- |
| 0      | comparison_less_than_16i::scalar_native     | 4284ns | 4276ns   |
| 1      | comparison_less_than_16i::vec16x16i_pcmpw   | 276ns  | 276ns    |
| 1      | comparison_less_than_16i::vec8x16i_pcmpgtw  | 609ns  | 609ns    |
| 2      | comparison_less_than_16i::vec16x16i_pcmpw_k | 260ns  | 260ns    |
| 2      | comparison_less_than_16i::vec8x16i_pcmpw_k  | 541ns  | 541ns    |
| 3      | comparison_less_than_16i::vec16x16i_pcmpw   | 279ns  | 279ns    |
| 3      | comparison_less_than_16i::vec32x16i_pcmpw_k | 148ns  | 148ns    |
| 3      | comparison_less_than_16i::vec8x16i_pcmpgtw  | 544ns  | 544ns    |


## comparison_less_than_16u
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86, AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512BW

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512BW, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                                  | Time   | CPU Time |
| ------ | ----------------------------------------------- | ------ | -------- |
| 0      | comparison_less_than_16u::scalar_native         | 4291ns | 4287ns   |
| 1      | comparison_less_than_16u::vec8x16u_pcmpgtw_xor  | 608ns  | 608ns    |
| 1      | comparison_less_than_16u::vec16x16u_pcmpgtw_xor | 314ns  | 314ns    |
| 2      | comparison_less_than_16u::vec8x16u_pcmpuw       | 543ns  | 543ns    |
| 2      | comparison_less_than_16u::vec16x16u_pcmpuw      | 262ns  | 262ns    |
| 3      | comparison_less_than_16u::vec16x16u_pcmpgtw_xor | 313ns  | 313ns    |
| 3      | comparison_less_than_16u::vec32x16u_pcmpuw      | 149ns  | 149ns    |
| 3      | comparison_less_than_16u::vec8x16u_pcmpgtw_xor  | 612ns  | 612ns    |


## comparison_less_than_32i
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                              | Time   | CPU Time |
| ------ | ------------------------------------------- | ------ | -------- |
| 0      | comparison_less_than_32i::scalar_native     | 2150ns | 2142ns   |
| 1      | comparison_less_than_32i::vec16x32i_pcmpd_k | 146ns  | 146ns    |
| 2      | comparison_less_than_32i::vec4x32i_pcmpd    | 512ns  | 512ns    |
| 3      | comparison_less_than_32i::vec4x32i_pcmpd_k  | 513ns  | 513ns    |
| 3      | comparison_less_than_32i::vec8x32i_pcmpd_k  | 272ns  | 272ns    |
| 4      | comparison_less_than_32i::vec8x32i_pcmpd    | 263ns  | 263ns    |


## comparison_less_than_32u
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                                 | Time   | CPU Time |
| ------ | ---------------------------------------------- | ------ | -------- |
| 0      | comparison_less_than_32u::scalar_native        | 2128ns | 2126ns   |
| 1      | comparison_less_than_32u::vec16x32u_pcmpud     | 143ns  | 143ns    |
| 2      | comparison_less_than_32u::vec4x32u_pcmpgtd_xor | 610ns  | 610ns    |
| 3      | comparison_less_than_32u::vec4x32u_pcmpud      | 511ns  | 511ns    |
| 3      | comparison_less_than_32u::vec8x32u_pcmpud      | 273ns  | 273ns    |
| 4      | comparison_less_than_32u::vec8x32u_pcmpgtd_xor | 308ns  | 308ns    |


## comparison_less_than_64i
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 5
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                                                 | Time   | CPU Time |
| ------ | -------------------------------------------------------------- | ------ | -------- |
| 0      | comparison_less_than_64i::scalar_native                        | 1070ns | 1069ns   |
| 1      | comparison_less_than_64i::vec8x64i_pcmpq_k                     | 139ns  | 139ns    |
| 2      | comparison_less_than_64i::vec2x64i_scalarized                  | 1463ns | 1463ns   |
| 2      | comparison_less_than_64i::vec2x64i_32_bit_comparison_emulation | 811ns  | 811ns    |
| 3      | comparison_less_than_64i::vec2x64i_pcmpq                       | 609ns  | 609ns    |
| 4      | comparison_less_than_64i::vec4x64i_pcmpq_k                     | 312ns  | 312ns    |
| 4      | comparison_less_than_64i::vec2x64i_pcmpq_k                     | 607ns  | 607ns    |
| 5      | comparison_less_than_64i::vec4x64i_pcmpq                       | 265ns  | 265ns    |


## comparison_less_than_64u
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 5
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                                                     | Time   | CPU Time |
| ------ | ------------------------------------------------------------------ | ------ | -------- |
| 0      | comparison_less_than_64u::scalar_native                            | 1070ns | 1070ns   |
| 1      | comparison_less_than_64u::vec8x64u_pcmpuq                          | 143ns  | 143ns    |
| 2      | comparison_less_than_64u::vec2x64u_scalarized                      | 1251ns | 1251ns   |
| 2      | comparison_less_than_64u::vec2x64u_32_bit_comparison_emulation_xor | 927ns  | 927ns    |
| 3      | comparison_less_than_64u::vec2x64u_pcmpgtq                         | 609ns  | 609ns    |
| 4      | comparison_less_than_64u::vec4x64u_pcmpuq                          | 309ns  | 309ns    |
| 4      | comparison_less_than_64u::vec2x64u_pcmpuq                          | 538ns  | 538ns    |
| 5      | comparison_less_than_64u::vec4x64u_pcmpgtq                         | 311ns  | 311ns    |


## comparison_less_than_8i
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86, AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512BW

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512BW, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                            | Time   | CPU Time |
| ------ | ----------------------------------------- | ------ | -------- |
| 0      | comparison_less_than_8i::scalar_native    | 8142ns | 8137ns   |
| 1      | comparison_less_than_8i::vec32x8i_pcmpgtb | 262ns  | 262ns    |
| 1      | comparison_less_than_8i::vec16x8i_pcmpgtb | 510ns  | 510ns    |
| 2      | comparison_less_than_8i::vec32x8i_pcmpb   | 264ns  | 264ns    |
| 2      | comparison_less_than_8i::vec16x8i_pcmpb   | 509ns  | 508ns    |
| 3      | comparison_less_than_8i::vec32x8i_pcmpgtb | 267ns  | 267ns    |
| 3      | comparison_less_than_8i::vec64x8i_pcmpb   | 143ns  | 143ns    |
| 3      | comparison_less_than_8i::vec16x8i_pcmpgtb | 524ns  | 524ns    |


## comparison_less_than_8u
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86, AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512BW

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512BW, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                                | Time   | CPU Time |
| ------ | --------------------------------------------- | ------ | -------- |
| 0      | comparison_less_than_8u::scalar_native        | 8242ns | 8238ns   |
| 1      | comparison_less_than_8u::vec16x8u_pcmpgtb_xor | 604ns  | 604ns    |
| 1      | comparison_less_than_8u::vec32x8u_pcmpgtb_xor | 311ns  | 311ns    |
| 2      | comparison_less_than_8u::vec16x8u_pcmpub      | 509ns  | 509ns    |
| 2      | comparison_less_than_8u::vec32x8u_pcmpub      | 257ns  | 257ns    |
| 3      | comparison_less_than_8u::vec32x8u_pcmpgtb_xor | 314ns  | 314ns    |
| 3      | comparison_less_than_8u::vec16x8u_pcmpgtb_xor | 606ns  | 606ns    |
| 3      | comparison_less_than_8u::vec64x8u_pcmpub      | 140ns  | 140ns    |


## comparison_not_equals_16u
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86, AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512BW

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512BW, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                                | Time   | CPU Time |
| ------ | --------------------------------------------- | ------ | -------- |
| 0      | comparison_not_equals_16u::scalar_native      | 4242ns | 4239ns   |
| 1      | comparison_not_equals_16u::vec16x16u_cmpeqw   | 312ns  | 312ns    |
| 1      | comparison_not_equals_16u::vec8x16u_cmpeqw    | 606ns  | 606ns    |
| 2      | comparison_not_equals_16u::vec8x16u_cmpeqw_k  | 538ns  | 538ns    |
| 2      | comparison_not_equals_16u::vec16x16u_cmpeqw_k | 262ns  | 262ns    |
| 3      | comparison_not_equals_16u::vec8x16u_cmpeqw    | 603ns  | 603ns    |
| 3      | comparison_not_equals_16u::vec16x16u_cmpeqw   | 312ns  | 312ns    |
| 3      | comparison_not_equals_16u::vec32x16u_cmpeqw_k | 151ns  | 151ns    |


## comparison_not_equals_32f
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                               | Time   | CPU Time |
| ------ | -------------------------------------------- | ------ | -------- |
| 0      | comparison_not_equals_32f::scalar_native     | 2409ns | 2407ns   |
| 1      | comparison_not_equals_32f::vec16x32f_vcmpps  | 140ns  | 140ns    |
| 2      | comparison_not_equals_32f::vec4x32f_cmpps    | 609ns  | 609ns    |
| 3      | comparison_not_equals_32f::vec8x32f_vcmpps   | 262ns  | 262ns    |
| 3      | comparison_not_equals_32f::vec4x32f_cmpeqd_k | 540ns  | 540ns    |
| 4      | comparison_not_equals_32f::vec8x32f_cmpeqd   | 273ns  | 273ns    |


## comparison_not_equals_32u
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                                | Time   | CPU Time |
| ------ | --------------------------------------------- | ------ | -------- |
| 0      | comparison_not_equals_32u::scalar_native      | 2136ns | 2135ns   |
| 1      | comparison_not_equals_32u::vec16x32u_cmpeqd_k | 139ns  | 139ns    |
| 2      | comparison_not_equals_32u::vec4x32u_cmpeqd    | 605ns  | 605ns    |
| 3      | comparison_not_equals_32u::vec4x32u_cmpeqd_k  | 607ns  | 607ns    |
| 3      | comparison_not_equals_32u::vec8x32u_cmpeqd_k  | 276ns  | 276ns    |
| 4      | comparison_not_equals_32u::vec8x32u_cmpeqd    | 308ns  | 308ns    |


## comparison_not_equals_64f
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                               | Time   | CPU Time |
| ------ | -------------------------------------------- | ------ | -------- |
| 0      | comparison_not_equals_64f::scalar_native     | 1204ns | 1203ns   |
| 1      | comparison_not_equals_64f::vec8x64f_vcmppd   | 150ns  | 150ns    |
| 2      | comparison_not_equals_64f::vec2x64f_cmppd    | 514ns  | 514ns    |
| 3      | comparison_not_equals_64f::vec4x64f_vcmppd   | 271ns  | 271ns    |
| 3      | comparison_not_equals_64f::vec2x64f_cmpeqd_k | 515ns  | 515ns    |
| 4      | comparison_not_equals_64f::vec4x64f_cmpeqd   | 266ns  | 266ns    |


## comparison_not_equals_64u
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 5
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                                                    | Time   | CPU Time |
| ------ | ----------------------------------------------------------------- | ------ | -------- |
| 0      | comparison_not_equals_64u::scalar_native                          | 1076ns | 1073ns   |
| 1      | comparison_not_equals_64u::vec8x64u_cmpeqq_k                      | 147ns  | 147ns    |
| 2      | comparison_not_equals_64u::vec2x64u_32_bit_comparison_and_combine | 609ns  | 609ns    |
| 3      | comparison_not_equals_64u::vec2x64u_cmpeqq                        | 610ns  | 610ns    |
| 4      | comparison_not_equals_64u::vec2x64u_cmpeqq_k                      | 512ns  | 512ns    |
| 4      | comparison_not_equals_64u::vec4x64u_cmpeqq_k                      | 306ns  | 306ns    |
| 5      | comparison_not_equals_64u::vec4x64u_cmpeqq                        | 311ns  | 312ns    |


## countl_zero_64u
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

### Results:
| Config | Implementation                 | Time   | CPU Time |
| ------ | ------------------------------ | ------ | -------- |
| 0      | countl_zero_64u::scalar_native | 1608ns | 1607ns   |


## countl_zero_8u
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* LZCNT, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSSE3, SSE3, SSE2, X86

#### Config 5
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512BW, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512VBMI

### Results:
| Config | Implementation                                      | Time    | CPU Time |
| ------ | --------------------------------------------------- | ------- | -------- |
| 0      | countl_zero_8u::scalar_byte_lookup                  | 10022ns | 10002ns  |
| 0      | countl_zero_8u::scalar_nibble_lookup                | 22998ns | 22981ns  |
| 0      | countl_zero_8u::scalar_native                       | 13727ns | 13718ns  |
| 0      | countl_zero_8u::scalar_float_conversion             | 19455ns | 19443ns  |
| 0      | countl_zero_8u::scalar_bitwise_accumulate           | 76950ns | 76911ns  |
| 1      | countl_zero_8u::vec16x8u_bitwise_divide_and_conquer | 2659ns  | 2654ns   |
| 2      | countl_zero_8u::scalar_bsr                          | 17103ns | 17104ns  |
| 3      | countl_zero_8u::scalar_lzcnt                        | 13009ns | 13010ns  |
| 4      | countl_zero_8u::vec16x8u_pshufb                     | 1199ns  | 1199ns   |
| 5      | countl_zero_8u::vec64x8u_vpermi2b                   | 326ns   | 326ns    |


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
| Config | Implementation                              | Time    | CPU Time |
| ------ | ------------------------------------------- | ------- | -------- |
| 0      | fpclassify_32f::scalar_branching            | 6384ns  | 6378ns   |
| 0      | fpclassify_32f::scalar_native               | 7122ns  | 7116ns   |
| 0      | fpclassify_32f::scalar_blending0            | 22588ns | 22575ns  |
| 0      | fpclassify_32f::scalar_blending1            | 15413ns | 15404ns  |
| 1      | fpclassify_32f::vec4x32f_blending           | 1952ns  | 1951ns   |
| 1      | fpclassify_32f::vec4x32f_avel               | 1939ns  | 1936ns   |
| 2      | fpclassify_32f::vec8x32f_blending           | 981ns   | 981ns    |
| 3      | fpclassify_32f::vec16x32f_blending_ternlog  | 765ns   | 766ns    |
| 4      | fpclassify_32f::vec4x32f_blending_ternlog   | 2114ns  | 2114ns   |
| 4      | fpclassify_32f::vec8x32f_blending_ternlog   | 1072ns  | 1072ns   |
| 5      | fpclassify_32f::vec8x32f_blending_vfpclass  | 1367ns  | 1367ns   |
| 5      | fpclassify_32f::vec4x32f_blending_vfpclass  | 2884ns  | 2885ns   |
| 6      | fpclassify_32f::vec16x32f_blending_vfpclass | 759ns   | 759ns    |


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
| Config | Implementation                             | Time   | CPU Time |
| ------ | ------------------------------------------ | ------ | -------- |
| 0      | fpclassify_64f::scalar_native              | 3261ns | 3260ns   |
| 0      | fpclassify_64f::scalar_branching           | 2428ns | 2427ns   |
| 1      | fpclassify_64f::vec2x64f_blend_sse2        | 2056ns | 2056ns   |
| 1      | fpclassify_64f::vec2x64f_scalarized        | 8854ns | 8837ns   |
| 2      | fpclassify_64f::vec4x64f_lend_avx2         | 1033ns | 1032ns   |
| 3      | fpclassify_64f::vec8x64f_blend_avx512f     | 762ns  | 763ns    |
| 4      | fpclassify_64f::vec8x64f_blending_vfpclass | 760ns  | 760ns    |
| 5      | fpclassify_64f::vec2x64f_blend_sse42       | 2020ns | 2021ns   |


## mask_from_bool_8i
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512BW

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512BW, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                        | Time    | CPU Time |
| ------ | ------------------------------------- | ------- | -------- |
| 0      | mask_from_bool_8i::scalar_negate_bool | 11151ns | 11143ns  |
| 0      | mask_from_bool_8i::scalar_ternary     | 11126ns | 11119ns  |
| 1      | mask_from_bool_8i::mask32x8i_negate   | 404ns   | 404ns    |
| 1      | mask_from_bool_8i::mask16x8i_negate   | 731ns   | 731ns    |
| 2      | mask_from_bool_8i::mask64x8i_negate   | 186ns   | 186ns    |
| 3      | mask_from_bool_8i::mask16x8i_ternary  | 1369ns  | 1370ns   |
| 3      | mask_from_bool_8i::mask16x8i_set1     | 948ns   | 948ns    |
| 4      | mask_from_bool_8i::mask32x8i_ternary  | 459ns   | 459ns    |
| 4      | mask_from_bool_8i::mask32x8i_set1     | 512ns   | 512ns    |


## popcount_16u
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
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSSE3, SSE3, SSE2, X86

#### Config 5
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* POPCNT, X86

#### Config 6
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512BITALG

#### Config 7
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512BW, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512VBMI

#### Config 8
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512BW, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512BITALG

### Results:
| Config | Implementation                                          | Time    | CPU Time |
| ------ | ------------------------------------------------------- | ------- | -------- |
| 0      | popcount_16u::scalar_bitwise_accumulate                 | 54848ns | 54817ns  |
| 0      | popcount_16u::scalar_bitwise_divide_and_conquer         | 60076ns | 60043ns  |
| 0      | popcount_16u::scalar_bitwise_accumulate_branchless      | 57703ns | 57671ns  |
| 0      | popcount_16u::scalar_byte_lookup                        | 9641ns  | 9621ns   |
| 0      | popcount_16u::scalar_nibble_lookup                      | 16995ns | 16986ns  |
| 0      | popcount_16u::scalar_native                             | 6860ns  | 6856ns   |
| 1      | popcount_16u::vec8x16u_bitwise_divide_and_conquer_madd  | 1942ns  | 1942ns   |
| 1      | popcount_16u::vec32x16u_bitwise_divide_and_conquer_madd | 623ns   | 623ns    |
| 1      | popcount_16u::vec32x16u_pshufb                          | 502ns   | 502ns    |
| 1      | popcount_16u::vec16x16u_bitwise_divide_and_conquer_madd | 987ns   | 987ns    |
| 1      | popcount_16u::vec32x16u_pshufb_madd                     | 401ns   | 401ns    |
| 2      | popcount_16u::vec8x16u_bitwise_divide_and_conquer       | 2130ns  | 2129ns   |
| 3      | popcount_16u::vec16x16u_pshufb_madd                     | 741ns   | 741ns    |
| 3      | popcount_16u::vec16x16u_pshufb                          | 832ns   | 832ns    |
| 4      | popcount_16u::vec8x16u_pshufb                           | 1651ns  | 1652ns   |
| 4      | popcount_16u::vec8x16u_pshufb_madd                      | 1470ns  | 1469ns   |
| 5      | popcount_16u::scalar_popcnt_op                          | 6816ns  | 6816ns   |
| 6      | popcount_16u::vec16x16u_popcnt_op                       | 406ns   | 406ns    |
| 6      | popcount_16u::vec8x16u_popcnt_op                        | 804ns   | 804ns    |
| 7      | popcount_16u::vec32x16u_vpermi2b                        | 490ns   | 490ns    |
| 7      | popcount_16u::vec32x16u_vpermi2b_madd                   | 386ns   | 386ns    |
| 8      | popcount_16u::vec32x16u_popcnt_op                       | 212ns   | 212ns    |


## popcount_32u
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSSE3, SSE3, SSE2, X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* POPCNT, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512BITALG

#### Config 5
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512VPOPCNTDQ

#### Config 6
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VPOPCNTDQ, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                                     | Time    | CPU Time |
| ------ | -------------------------------------------------- | ------- | -------- |
| 0      | popcount_32u::scalar_bitwise_accumulate_branchless | 11244ns | 11197ns  |
| 0      | popcount_32u::scalar_bitwise_accumulate            | 7826ns  | 7816ns   |
| 0      | popcount_32u::scalar_byte_lookup                   | 53657ns | 53597ns  |
| 0      | popcount_32u::scalar_native                        | 3224ns  | 3222ns   |
| 0      | popcount_32u::scalar_nibble_lookup                 | 16834ns | 16823ns  |
| 0      | popcount_32u::scalar_bitwise_divide_and_conquer    | 3770ns  | 3768ns   |
| 1      | popcount_32u::vec4x32u_bitwise_divide_and_conquer  | 2897ns  | 2892ns   |
| 2      | popcount_32u::vec4x32u_pshufb                      | 1864ns  | 1864ns   |
| 3      | popcount_32u::scalar_popcnt_op                     | 3194ns  | 3194ns   |
| 4      | popcount_32u::vec4x32u_popcnt16_op                 | 1215ns  | 1215ns   |
| 5      | popcount_32u::vec8x32u_popcnt_op                   | 415ns   | 415ns    |
| 5      | popcount_32u::vec4x32u_popcnt_op                   | 808ns   | 808ns    |
| 6      | popcount_32u::vec16x32u_popcnt_op                  | 212ns   | 212ns    |


## popcount_64u
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
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 5
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSSE3, SSE3, SSE2, X86

#### Config 6
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* POPCNT, X86

#### Config 7
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512BITALG

#### Config 8
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512VPOPCNTDQ

#### Config 9
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VPOPCNTDQ, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 10
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86, POPCNT

#### Config 11
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 12
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512BITALG, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 13
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512BW, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512BITALG

### Results:
| Config | Implementation                                           | Time     | CPU Time |
| ------ | -------------------------------------------------------- | -------- | -------- |
| 0      | popcount_64u::scalar_bitwise_accumulate                  | 4006ns   | 3987ns   |
| 0      | popcount_64u::scalar_native                              | 1610ns   | 1609ns   |
| 0      | popcount_64u::scalar_nibble_lookup                       | 16415ns  | 16405ns  |
| 0      | popcount_64u::scalar_byte_lookup                         | 48898ns  | 48871ns  |
| 0      | popcount_64u::scalar_bitwise_accumulate_branchless       | 101196ns | 101143ns |
| 0      | popcount_64u::scalar_bitwise_divide_and_conquer          | 7982ns   | 7977ns   |
| 1      | popcount_64u::vec8x64u_bitwise_divide_and_conquer_sad    | 646ns    | 646ns    |
| 2      | popcount_64u::vec2x64u_bitwise_divide_and_conquer        | 3212ns   | 3212ns   |
| 3      | popcount_64u::vec4x64u_pshufb_sad                        | 742ns    | 742ns    |
| 3      | popcount_64u::vec4x64u_pshufb                            | 1048ns   | 1047ns   |
| 3      | popcount_64u::vec4x64u_scalarized                        | 1434ns   | 1434ns   |
| 3      | popcount_64u::vec4x64u_bitwise_divide_and_conquer_sad    | 1003ns   | 1003ns   |
| 4      | popcount_64u::vec8x64u_bitwise_divide_and_conquer        | 1062ns   | 1061ns   |
| 4      | popcount_64u::vec8x64u_scalarized                        | 2242ns   | 2244ns   |
| 5      | popcount_64u::vec2x64u_bitwise_divide_and_conquer_sad    | 1973ns   | 1973ns   |
| 5      | popcount_64u::vec2x64u_pshufb_sad                        | 1447ns   | 1447ns   |
| 5      | popcount_64u::vec2x64u_pshufb                            | 2057ns   | 2057ns   |
| 6      | popcount_64u::scalar_popcnt_op                           | 1605ns   | 1605ns   |
| 7      | popcount_64u::vec2x64u_popcnt16_op                       | 1299ns   | 1299ns   |
| 7      | popcount_64u::vec2x64u_popcnt16_op_sad                   | 1038ns   | 1038ns   |
| 7      | popcount_64u::vec4x64u_popcnt16_op_sad                   | 530ns    | 530ns    |
| 7      | popcount_64u::vec4x64u_popcnt16_op                       | 661ns    | 661ns    |
| 8      | popcount_64u::vec4x64u_popcnt_op                         | 413ns    | 413ns    |
| 8      | popcount_64u::vec2x64u_popcnt_op                         | 812ns    | 812ns    |
| 9      | popcount_64u::vec8x64u_popcnt_op                         | 214ns    | 214ns    |
| 10     | popcount_64u::vec2x64u_scalarized_popcnt_op              | 1015ns   | 1015ns   |
| 11     | popcount_64u::vec2x64u_scalarized_popcnt_op_with_extract | 1769ns   | 1769ns   |
| 12     | popcount_64u::vec8x64u_popcnt16_op                       | 323ns    | 323ns    |
| 13     | popcount_64u::vec8x64u_popcnt16_op_sad                   | 256ns    | 256ns    |


## popcount_8u
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
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSSE3, SSE3, SSE2, X86

#### Config 5
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* POPCNT, X86

#### Config 6
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512BITALG

#### Config 7
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512BW, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512VBMI

#### Config 8
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512BW, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512BITALG

### Results:
| Config | Implementation                                    | Time     | CPU Time |
| ------ | ------------------------------------------------- | -------- | -------- |
| 0      | popcount_8u::scalar_nibble_lookup                 | 19212ns  | 19149ns  |
| 0      | popcount_8u::scalar_bitwise_divide_and_conquer    | 31296ns  | 31256ns  |
| 0      | popcount_8u::scalar_native                        | 11281ns  | 11256ns  |
| 0      | popcount_8u::scalar_bitwise_accumulate_branchless | 56953ns  | 56922ns  |
| 0      | popcount_8u::scalar_byte_lookup                   | 236482ns | 236360ns |
| 0      | popcount_8u::scalar_bitwise_accumulate            | 165956ns | 166777ns |
| 1      | popcount_8u::vec64x8u_pshufb                      | 340ns    | 340ns    |
| 1      | popcount_8u::vec64x8u_bitwise_divide_and_conquer  | 566ns    | 566ns    |
| 2      | popcount_8u::vec16x8u_bitwise_divide_and_conquer  | 1602ns   | 1602ns   |
| 3      | popcount_8u::vec32x8u_pshufb                      | 610ns    | 609ns    |
| 3      | popcount_8u::vec32x8u_bitwise_divide_and_conquer  | 813ns    | 812ns    |
| 4      | popcount_8u::vec16x8u_pshufb                      | 1213ns   | 1211ns   |
| 5      | popcount_8u::scalar_popcnt_op                     | 11170ns  | 11170ns  |
| 6      | popcount_8u::vec32x8u_popcnt_op                   | 409ns    | 409ns    |
| 6      | popcount_8u::vec16x8u_popcnt_op                   | 805ns    | 805ns    |
| 7      | popcount_8u::vec64x8u_vpermi2b                    | 331ns    | 331ns    |
| 8      | popcount_8u::vec64x8u_popcnt_op                   | 211ns    | 211ns    |


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
| Config | Implementation             | Time    | CPU Time |
| ------ | -------------------------- | ------- | -------- |
| 0      | scalbn_32f::scalar_native  | 11085ns | 11078ns  |
| 1      | scalbn_32f::vec4x32f_avel  | 2337ns  | 2337ns   |
| 2      | scalbn_32f::vec8x32f_avel  | 1209ns  | 1209ns   |
| 3      | scalbn_32f::vec16x32f_avel | 164ns   | 164ns    |


## shift_left_8u
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
* SSSE3, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512BW

### Results:
| Config | Implementation                             | Time   | CPU Time |
| ------ | ------------------------------------------ | ------ | -------- |
| 0      | shift_left_8u::scalar_native               | 8006ns | 8002ns   |
| 1      | shift_left_8u::vec16x8u_divide_and_conquer | 1226ns | 1223ns   |
| 2      | shift_left_8u::vec32x8u_pshufb             | 363ns  | 362ns    |
| 3      | shift_left_8u::vec16x8u_pshufb             | 708ns  | 708ns    |
| 4      | shift_left_8u::vec16x8u_widening           | 605ns  | 605ns    |
| 4      | shift_left_8u::vec32x8u_widening           | 414ns  | 414ns    |


## vec_from_mask_16u
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86, AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512BW

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512BW, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                                | Time    | CPU Time |
| ------ | --------------------------------------------- | ------- | -------- |
| 0      | vec_from_mask_16u::scalar_native_conversion   | 11495ns | 11487ns  |
| 1      | vec_from_mask_16u::vec16x16u_negation         | 66.3ns  | 66.4ns   |
| 1      | vec_from_mask_16u::vec8x16u_negation          | 260ns   | 261ns    |
| 2      | vec_from_mask_16u::vec16x16u_masked_broadcast | 59.9ns  | 59.9ns   |
| 2      | vec_from_mask_16u::vec16x16u_move_mask        | 59.3ns  | 59.2ns   |
| 2      | vec_from_mask_16u::vec8x16u_masked_broadcast  | 208ns   | 208ns    |
| 2      | vec_from_mask_16u::vec8x16u_move_mask         | 208ns   | 207ns    |
| 3      | vec_from_mask_16u::vec32x16u_move_mask        | 15.8ns  | 15.8ns   |
| 3      | vec_from_mask_16u::vec8x16u_negation          | 260ns   | 260ns    |
| 3      | vec_from_mask_16u::vec16x16u_negation         | 66.7ns  | 66.7ns   |
| 3      | vec_from_mask_16u::vec32x16u_masked_broadcast | 20.8ns  | 20.8ns   |


## vec_from_mask_32f
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                                | Time    | CPU Time |
| ------ | --------------------------------------------- | ------- | -------- |
| 0      | vec_from_mask_32f::scalar_native_conversion   | 18689ns | 18660ns  |
| 1      | vec_from_mask_32f::vec16x32f_masked_broadcast | 76.4ns  | 76.5ns   |
| 2      | vec_from_mask_32f::vec4x32f_bitwise_and       | 1080ns  | 1080ns   |
| 3      | vec_from_mask_32f::vec8x32f_masked_broadcast  | 301ns   | 301ns    |
| 3      | vec_from_mask_32f::vec4x32f_masked_broadcast  | 10348ns | 10349ns  |
| 4      | vec_from_mask_32f::vec8x32f_bitwise_and       | 243ns   | 243ns    |


## vec_from_mask_32u
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 5
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512DQ

#### Config 6
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512DQ, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                                | Time    | CPU Time |
| ------ | --------------------------------------------- | ------- | -------- |
| 0      | vec_from_mask_32u::scalar_native_conversion   | 11220ns | 11214ns  |
| 1      | vec_from_mask_32u::vec16x32u_masked_broadcast | 76.7ns  | 76.7ns   |
| 2      | vec_from_mask_32u::vec4x32u_negation          | 1077ns  | 1077ns   |
| 3      | vec_from_mask_32u::vec8x32u_masked_broadcast  | 301ns   | 301ns    |
| 3      | vec_from_mask_32u::vec4x32u_masked_broadcast  | 942ns   | 942ns    |
| 4      | vec_from_mask_32u::vec8x32u_negation          | 242ns   | 242ns    |
| 5      | vec_from_mask_32u::vec8x32u_move_mask         | 244ns   | 244ns    |
| 5      | vec_from_mask_32u::vec4x32u_move_mask         | 936ns   | 936ns    |
| 6      | vec_from_mask_32u::vec16x32u_move_mask        | 67.0ns  | 67.0ns   |


## vec_from_mask_64f
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                               | Time    | CPU Time |
| ------ | -------------------------------------------- | ------- | -------- |
| 0      | vec_from_mask_64f::scalar_native_conversion  | 18669ns | 18660ns  |
| 1      | vec_from_mask_64f::vec8x64f_masked_broadcast | 306ns   | 306ns    |
| 2      | vec_from_mask_64f::vec2x64f_bitwise_and      | 4811ns  | 4812ns   |
| 3      | vec_from_mask_64f::vec2x64f_masked_movedup   | 4771ns  | 4772ns   |
| 3      | vec_from_mask_64f::vec4x64f_masked_broadcast | 937ns   | 937ns    |
| 4      | vec_from_mask_64f::vec4x64f_bitwise_and      | 943ns   | 943ns    |


## vec_from_mask_64u
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 5
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512DQ

#### Config 6
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512DQ, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                               | Time    | CPU Time |
| ------ | -------------------------------------------- | ------- | -------- |
| 0      | vec_from_mask_64u::scalar_native_conversion  | 11162ns | 11153ns  |
| 1      | vec_from_mask_64u::vec8x64u_masked_broadcast | 306ns   | 306ns    |
| 2      | vec_from_mask_64u::vec2x64u_negation         | 6600ns  | 6601ns   |
| 3      | vec_from_mask_64u::vec4x64u_masked_broadcast | 938ns   | 938ns    |
| 3      | vec_from_mask_64u::vec2x64u_masked_broadcast | 4799ns  | 4799ns   |
| 4      | vec_from_mask_64u::vec4x64u_negation         | 943ns   | 943ns    |
| 5      | vec_from_mask_64u::vec2x64u_move_mask        | 4817ns  | 4817ns   |
| 5      | vec_from_mask_64u::vec4x64u_move_mask        | 953ns   | 953ns    |
| 6      | vec_from_mask_64u::vec8x64u_move_mask        | 245ns   | 245ns    |


## vec_from_mask_8u
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86, AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512BW

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512BW, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                              | Time    | CPU Time |
| ------ | ------------------------------------------- | ------- | -------- |
| 0      | vec_from_mask_8u::scalar_native_conversion  | 11163ns | 11157ns  |
| 1      | vec_from_mask_8u::vec16x8u_negation         | 57.9ns  | 58.0ns   |
| 1      | vec_from_mask_8u::vec32x8u_negation         | 15.2ns  | 15.2ns   |
| 2      | vec_from_mask_8u::vec16x8u_masked_broadcast | 57.4ns  | 57.4ns   |
| 2      | vec_from_mask_8u::vec32x8u_move_mask        | 13.6ns  | 13.6ns   |
| 2      | vec_from_mask_8u::vec32x8u_masked_broadcast | 18.9ns  | 18.9ns   |
| 2      | vec_from_mask_8u::vec16x8u_move_mask        | 60.7ns  | 60.7ns   |
| 3      | vec_from_mask_8u::vec32x8u_negation         | 15.4ns  | 15.4ns   |
| 3      | vec_from_mask_8u::vec16x8u_negation         | 58.8ns  | 58.8ns   |
| 3      | vec_from_mask_8u::vec64x8u_move_mask        | 3.67ns  | 3.67ns   |
| 3      | vec_from_mask_8u::vec64x8u_masked_broadcast | 3.65ns  | 3.65ns   |


## vec_from_scalar_16u
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
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSSE3, SSE3, SSE2, X86

### Results:
| Config | Implementation                                       | Time   | CPU Time |
| ------ | ---------------------------------------------------- | ------ | -------- |
| 0      | vec_from_scalar_16u::scalar_identity                 | 4813ns | 4798ns   |
| 1      | vec_from_scalar_16u::vec32x16u_broadcast             | 6922ns | 6926ns   |
| 1      | vec_from_scalar_16u::vec32x16u_broadcast_from_vector | 8442ns | 8443ns   |
| 2      | vec_from_scalar_16u::vec8x16u_mul_shuffle            | 9727ns | 9728ns   |
| 2      | vec_from_scalar_16u::vec8x16u_shift_shuffle          | 9641ns | 9623ns   |
| 2      | vec_from_scalar_16u::vec8x16u_unpack_shuffle         | 8025ns | 8018ns   |
| 3      | vec_from_scalar_16u::vec16x16u_broadcast             | 6382ns | 6379ns   |
| 3      | vec_from_scalar_16u::vec8x16u_broadcast              | 6432ns | 6429ns   |
| 3      | vec_from_scalar_16u::vec16x16u_broadcast_from_vector | 7991ns | 7988ns   |
| 3      | vec_from_scalar_16u::vec8x16u_broadcast_from_vector  | 7472ns | 7470ns   |
| 4      | vec_from_scalar_16u::vec8x16u_pshufb                 | 7415ns | 7414ns   |


## vec_from_scalar_32f
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
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                                       | Time   | CPU Time |
| ------ | ---------------------------------------------------- | ------ | -------- |
| 0      | vec_from_scalar_32f::scalar_identity                 | 2425ns | 2419ns   |
| 1      | vec_from_scalar_32f::vec16x32f_broadcast_from_vector | 3393ns | 3394ns   |
| 2      | vec_from_scalar_32f::vec4x32f_shuffle                | 3222ns | 3219ns   |
| 3      | vec_from_scalar_32f::vec4x32f_broadcast_from_vector  | 3211ns | 3209ns   |
| 3      | vec_from_scalar_32f::vec8x32f_broadcast              | 3186ns | 3185ns   |
| 3      | vec_from_scalar_32f::vec8x32f_broadcast_from_vector  | 3191ns | 3191ns   |
| 3      | vec_from_scalar_32f::vec4x32f_broadcast              | 3185ns | 3184ns   |
| 4      | vec_from_scalar_32f::vec16x32f_broadcast             | 3274ns | 3277ns   |


## vec_from_scalar_32u
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
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                                       | Time   | CPU Time |
| ------ | ---------------------------------------------------- | ------ | -------- |
| 0      | vec_from_scalar_32u::scalar_identity                 | 2667ns | 2665ns   |
| 1      | vec_from_scalar_32u::vec16x32u_broadcast_from_vector | 3391ns | 3392ns   |
| 2      | vec_from_scalar_32u::vec4x32u_shuffle                | 3227ns | 3220ns   |
| 3      | vec_from_scalar_32u::vec4x32u_broadcast_from_vector  | 3200ns | 3199ns   |
| 3      | vec_from_scalar_32u::vec8x32u_broadcast_from_vector  | 3223ns | 3222ns   |
| 3      | vec_from_scalar_32u::vec8x32u_broadcast              | 3214ns | 3213ns   |
| 3      | vec_from_scalar_32u::vec4x32u_broadcast              | 3187ns | 3186ns   |
| 4      | vec_from_scalar_32u::vec16x32u_broadcast             | 3271ns | 3273ns   |


## vec_from_scalar_64f
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
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                                      | Time   | CPU Time |
| ------ | --------------------------------------------------- | ------ | -------- |
| 0      | vec_from_scalar_64f::scalar_identity                | 1205ns | 1204ns   |
| 1      | vec_from_scalar_64f::vec8x64f_broadcast_from_vector | 1688ns | 1688ns   |
| 2      | vec_from_scalar_64f::vec2x64f_shuffle               | 1596ns | 1596ns   |
| 3      | vec_from_scalar_64f::vec4x64f_broadcast             | 1614ns | 1614ns   |
| 3      | vec_from_scalar_64f::vec2x64f_broadcast             | 1607ns | 1606ns   |
| 3      | vec_from_scalar_64f::vec4x64f_broadcast_from_vector | 1621ns | 1620ns   |
| 3      | vec_from_scalar_64f::vec2x64f_movedup               | 1597ns | 1597ns   |
| 4      | vec_from_scalar_64f::vec8x64f_broadcast             | 1644ns | 1644ns   |


## vec_from_scalar_64u
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
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                                      | Time   | CPU Time |
| ------ | --------------------------------------------------- | ------ | -------- |
| 0      | vec_from_scalar_64u::scalar_identity                | 1351ns | 1350ns   |
| 1      | vec_from_scalar_64u::vec8x64u_broadcast_from_vector | 1694ns | 1695ns   |
| 2      | vec_from_scalar_64u::vec2x64u_shuffle               | 1613ns | 1611ns   |
| 3      | vec_from_scalar_64u::vec2x64u_broadcast             | 1597ns | 1596ns   |
| 3      | vec_from_scalar_64u::vec4x64u_broadcast             | 1602ns | 1601ns   |
| 3      | vec_from_scalar_64u::vec4x64u_broadcast_from_vector | 1612ns | 1611ns   |
| 3      | vec_from_scalar_64u::vec2x64u_broadcast_from_vector | 1593ns | 1592ns   |
| 4      | vec_from_scalar_64u::vec8x64u_broadcast             | 1643ns | 1644ns   |


## vec_from_scalar_8u
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
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSSE3, SSE3, SSE2, X86

### Results:
| Config | Implementation                                     | Time    | CPU Time |
| ------ | -------------------------------------------------- | ------- | -------- |
| 0      | vec_from_scalar_8u::scalar_identity                | 8545ns  | 8540ns   |
| 1      | vec_from_scalar_8u::vec64x8u_broadcast_from_vector | 13733ns | 13737ns  |
| 1      | vec_from_scalar_8u::vec64x8u_broadcast             | 13676ns | 13677ns  |
| 2      | vec_from_scalar_8u::vec16x8u_unpack_shuffle        | 16222ns | 16214ns  |
| 3      | vec_from_scalar_8u::vec32x8u_broadcast             | 12981ns | 12975ns  |
| 3      | vec_from_scalar_8u::vec16x8u_broadcast_from_vector | 12758ns | 12754ns  |
| 3      | vec_from_scalar_8u::vec16x8u_broadcast             | 12691ns | 12688ns  |
| 3      | vec_from_scalar_8u::vec32x8u_broadcast_from_vector | 12920ns | 12918ns  |
| 4      | vec_from_scalar_8u::vec16x8u_pshufb                | 12821ns | 12821ns  |

