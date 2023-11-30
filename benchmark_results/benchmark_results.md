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
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512BW, AVX512CD

### Results:
| Config | Implementation                        | Time    | CPU Time |
| ------ | ------------------------------------- | ------- | -------- |
| 0      | bit_ceil_16u::scalar_bitwise          | 19064ns | 19064ns  |
| 0      | bit_ceil_16u::scalar_native           | 17244ns | 17243ns  |
| 1      | bit_ceil_16u::scalar_bsr_and_shift    | 19237ns | 19236ns  |
| 2      | bit_ceil_16u::scalar_lzcnt_and_rotate | 10499ns | 10499ns  |
| 3      | bit_ceil_16u::vec8x16u_bitwise        | 2367ns  | 2367ns   |
| 4      | bit_ceil_16u::vec16x16u_bitwise       | 991ns   | 991ns    |
| 5      | bit_ceil_16u::vec32x16u_bitwise       | 703ns   | 703ns    |
| 6      | bit_ceil_16u::vec8x16u_lzcnt32        | 2094ns  | 2094ns   |


## bit_ceil_32u
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* LZCNT, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 5
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512CD

#### Config 6
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 7
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512CD, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                                | Time    | CPU Time |
| ------ | --------------------------------------------- | ------- | -------- |
| 0      | bit_ceil_32u::scalar_native                   | 6081ns  | 6081ns   |
| 0      | bit_ceil_32u::scalar_bitwise                  | 11207ns | 11207ns  |
| 1      | bit_ceil_32u::vec16x32u_bitwise               | 822ns   | 822ns    |
| 2      | bit_ceil_32u::scalar_bsr_and_shift_branchless | 6525ns  | 6512ns   |
| 2      | bit_ceil_32u::scalar_bsr_and_shift            | 6616ns  | 6603ns   |
| 3      | bit_ceil_32u::scalar_lzcnt_and_rotate         | 5976ns  | 5975ns   |
| 3      | bit_ceil_32u::scalar_lzcnt_and_shift          | 5588ns  | 5588ns   |
| 4      | bit_ceil_32u::vec4x32u_bitwise                | 2433ns  | 2433ns   |
| 5      | bit_ceil_32u::vec8x32u_lzcnt                  | 661ns   | 661ns    |
| 5      | bit_ceil_32u::vec4x32u_lzcnt                  | 1288ns  | 1288ns   |
| 6      | bit_ceil_32u::vec8x32u_bitwise                | 1109ns  | 1110ns   |
| 7      | bit_ceil_32u::vec16x32u_lzcnt                 | 330ns   | 330ns    |


## bit_ceil_64u
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

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

### Results:
| Config | Implementation                        | Time   | CPU Time |
| ------ | ------------------------------------- | ------ | -------- |
| 0      | bit_ceil_64u::scalar_bitwise          | 6580ns | 6579ns   |
| 0      | bit_ceil_64u::scalar_native           | 3064ns | 3063ns   |
| 1      | bit_ceil_64u::vec8x64u_bitwise        | 829ns  | 829ns    |
| 2      | bit_ceil_64u::scalar_lzcnt_and_rotate | 2626ns | 2626ns   |
| 2      | bit_ceil_64u::scalar_bsr_and_shift    | 3218ns | 3218ns   |
| 3      | bit_ceil_64u::vec2x64u_bitwise        | 2812ns | 2812ns   |
| 4      | bit_ceil_64u::vec4x64u_lzcnt          | 924ns  | 924ns    |
| 4      | bit_ceil_64u::vec8x64u_lzcnt          | 402ns  | 402ns    |
| 4      | bit_ceil_64u::vec2x64u_lzcnt          | 1466ns | 1466ns   |
| 5      | bit_ceil_64u::vec4x64u_bitwise        | 1127ns | 1127ns   |


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
| Config | Implementation                       | Time    | CPU Time |
| ------ | ------------------------------------ | ------- | -------- |
| 0      | bit_ceil_8u::scalar_native           | 33967ns | 33966ns  |
| 0      | bit_ceil_8u::scalar_bitwise          | 29370ns | 29369ns  |
| 1      | bit_ceil_8u::scalar_bsr_and_shift    | 38334ns | 38300ns  |
| 2      | bit_ceil_8u::scalar_lzcnt_and_rotate | 19297ns | 19296ns  |
| 3      | bit_ceil_8u::vec16x8u_bitwise        | 2363ns  | 2363ns   |
| 4      | bit_ceil_8u::vec32x8u_pshufb         | 806ns   | 806ns    |
| 5      | bit_ceil_8u::vec64x8u_pshufb         | 600ns   | 600ns    |
| 6      | bit_ceil_8u::vec16x8u_pshufb         | 2007ns  | 2007ns   |
| 7      | bit_ceil_8u::vec64x8u_vpermi2b       | 588ns   | 588ns    |


## comparison_equals_16u
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
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512BW

### Results:
| Config | Implementation                            | Time   | CPU Time |
| ------ | ----------------------------------------- | ------ | -------- |
| 0      | comparison_equals_16u::scalar_native      | 4278ns | 4277ns   |
| 1      | comparison_equals_16u::vec8x16u_cmpeqw    | 517ns  | 517ns    |
| 2      | comparison_equals_16u::vec16x16u_cmpeqw   | 274ns  | 274ns    |
| 3      | comparison_equals_16u::vec32x16u_cmpeqw_k | 148ns  | 148ns    |
| 4      | comparison_equals_16u::vec16x16u_cmpeqw_k | 274ns  | 274ns    |
| 4      | comparison_equals_16u::vec8x16u_cmpeqw_k  | 518ns  | 518ns    |


## comparison_equals_32f
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                           | Time   | CPU Time |
| ------ | ---------------------------------------- | ------ | -------- |
| 0      | comparison_equals_32f::scalar_native     | 2413ns | 2413ns   |
| 1      | comparison_equals_32f::vec4x32f_cmpps    | 513ns  | 513ns    |
| 2      | comparison_equals_32f::vec4x32f_cmpeqd_k | 538ns  | 538ns    |
| 2      | comparison_equals_32f::vec8x32f_vcmpps   | 277ns  | 277ns    |
| 3      | comparison_equals_32f::vec8x32f_cmpeqd   | 274ns  | 274ns    |
| 4      | comparison_equals_32f::vec16x32f_vcmpps  | 143ns  | 143ns    |


## comparison_equals_32u
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                            | Time   | CPU Time |
| ------ | ----------------------------------------- | ------ | -------- |
| 0      | comparison_equals_32u::scalar_native      | 2138ns | 2138ns   |
| 1      | comparison_equals_32u::vec4x32u_cmpeqd    | 536ns  | 536ns    |
| 2      | comparison_equals_32u::vec4x32u_cmpeqd_k  | 517ns  | 517ns    |
| 2      | comparison_equals_32u::vec8x32u_cmpeqd_k  | 274ns  | 274ns    |
| 3      | comparison_equals_32u::vec8x32u_cmpeqd    | 267ns  | 267ns    |
| 4      | comparison_equals_32u::vec16x32u_cmpeqd_k | 143ns  | 143ns    |


## comparison_equals_64f
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                           | Time   | CPU Time |
| ------ | ---------------------------------------- | ------ | -------- |
| 0      | comparison_equals_64f::scalar_native     | 1203ns | 1203ns   |
| 1      | comparison_equals_64f::vec2x64f_cmppd    | 512ns  | 512ns    |
| 2      | comparison_equals_64f::vec4x64f_vcmppd   | 274ns  | 274ns    |
| 2      | comparison_equals_64f::vec2x64f_cmpeqd_k | 612ns  | 612ns    |
| 3      | comparison_equals_64f::vec4x64f_cmpeqd   | 277ns  | 277ns    |
| 4      | comparison_equals_64f::vec8x64f_vcmppd   | 146ns  | 146ns    |


## comparison_equals_64u
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 5
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                                                | Time   | CPU Time |
| ------ | ------------------------------------------------------------- | ------ | -------- |
| 0      | comparison_equals_64u::scalar_native                          | 1077ns | 1077ns   |
| 1      | comparison_equals_64u::vec2x64u_32_bit_comparison_and_combine | 613ns  | 612ns    |
| 2      | comparison_equals_64u::vec4x64u_cmpeqq_k                      | 305ns  | 305ns    |
| 2      | comparison_equals_64u::vec2x64u_cmpeqq_k                      | 545ns  | 544ns    |
| 3      | comparison_equals_64u::vec4x64u_cmpeqq                        | 275ns  | 275ns    |
| 4      | comparison_equals_64u::vec8x64u_cmpeqq_k                      | 143ns  | 143ns    |
| 5      | comparison_equals_64u::vec2x64u_cmpeqq                        | 517ns  | 517ns    |


## comparison_equals_8u
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
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512BW

### Results:
| Config | Implementation                          | Time   | CPU Time |
| ------ | --------------------------------------- | ------ | -------- |
| 0      | comparison_equals_8u::scalar_native     | 8213ns | 8213ns   |
| 1      | comparison_equals_8u::vec16x8u_cmpeqb   | 508ns  | 508ns    |
| 2      | comparison_equals_8u::vec32x8u_cmpeqb   | 264ns  | 264ns    |
| 3      | comparison_equals_8u::vec64x8u_cmpeqb_k | 136ns  | 136ns    |
| 4      | comparison_equals_8u::vec32x8u_cmpeqb_k | 264ns  | 264ns    |
| 4      | comparison_equals_8u::vec16x8u_cmpeqb_k | 509ns  | 509ns    |


## countl_zero_64u
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

### Results:
| Config | Implementation                 | Time   | CPU Time |
| ------ | ------------------------------ | ------ | -------- |
| 0      | countl_zero_64u::scalar_native | 1955ns | 1955ns   |


## countl_zero_8u
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
* SSSE3, SSE3, SSE2, X86

#### Config 5
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512BW, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512VBMI

### Results:
| Config | Implementation                                      | Time    | CPU Time |
| ------ | --------------------------------------------------- | ------- | -------- |
| 0      | countl_zero_8u::scalar_float_conversion             | 20617ns | 20617ns  |
| 0      | countl_zero_8u::scalar_bitwise_accumulate           | 78234ns | 78232ns  |
| 0      | countl_zero_8u::scalar_native                       | 15771ns | 15771ns  |
| 0      | countl_zero_8u::scalar_nibble_lookup                | 22708ns | 22708ns  |
| 0      | countl_zero_8u::scalar_byte_lookup                  | 10673ns | 10673ns  |
| 1      | countl_zero_8u::scalar_bsr                          | 17295ns | 17266ns  |
| 2      | countl_zero_8u::scalar_lzcnt                        | 12868ns | 12868ns  |
| 3      | countl_zero_8u::vec16x8u_bitwise_divide_and_conquer | 2803ns  | 2803ns   |
| 4      | countl_zero_8u::vec16x8u_pshufb                     | 1404ns  | 1404ns   |
| 5      | countl_zero_8u::vec64x8u_vpermi2b                   | 321ns   | 321ns    |


## fpclassify_32f
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512DQ

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512DQ, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 5
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 6
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                              | Time    | CPU Time |
| ------ | ------------------------------------------- | ------- | -------- |
| 0      | fpclassify_32f::scalar_branching            | 6014ns  | 6014ns   |
| 0      | fpclassify_32f::scalar_native               | 7335ns  | 7335ns   |
| 0      | fpclassify_32f::scalar_blending1            | 15329ns | 15329ns  |
| 0      | fpclassify_32f::scalar_blending0            | 21894ns | 21893ns  |
| 1      | fpclassify_32f::vec4x32f_blending_vfpclass  | 2903ns  | 2903ns   |
| 1      | fpclassify_32f::vec8x32f_blending_vfpclass  | 1458ns  | 1458ns   |
| 2      | fpclassify_32f::vec4x32f_blending_ternlog   | 2200ns  | 2200ns   |
| 2      | fpclassify_32f::vec8x32f_blending_ternlog   | 1129ns  | 1129ns   |
| 3      | fpclassify_32f::vec16x32f_blending_vfpclass | 759ns   | 759ns    |
| 4      | fpclassify_32f::vec16x32f_blending_ternlog  | 866ns   | 866ns    |
| 5      | fpclassify_32f::vec4x32f_avel               | 2477ns  | 2474ns   |
| 5      | fpclassify_32f::vec4x32f_blending           | 2461ns  | 2461ns   |
| 6      | fpclassify_32f::vec8x32f_blending           | 1179ns  | 1179ns   |


## fpclassify_64f
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512DQ, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 5
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                             | Time   | CPU Time |
| ------ | ------------------------------------------ | ------ | -------- |
| 0      | fpclassify_64f::scalar_branching           | 2855ns | 2855ns   |
| 0      | fpclassify_64f::scalar_native              | 3308ns | 3308ns   |
| 1      | fpclassify_64f::vec8x64f_blending_vfpclass | 760ns  | 760ns    |
| 2      | fpclassify_64f::vec8x64f_blend_avx512f     | 900ns  | 900ns    |
| 3      | fpclassify_64f::vec2x64f_scalarized        | 8730ns | 8730ns   |
| 3      | fpclassify_64f::vec2x64f_blend_sse2        | 2528ns | 2528ns   |
| 4      | fpclassify_64f::vec4x64f_lend_avx2         | 1178ns | 1178ns   |
| 5      | fpclassify_64f::vec2x64f_blend_sse42       | 2512ns | 2512ns   |


## mask_from_bool_8i
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
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512BW

### Results:
| Config | Implementation                        | Time    | CPU Time |
| ------ | ------------------------------------- | ------- | -------- |
| 0      | mask_from_bool_8i::scalar_ternary     | 11237ns | 11237ns  |
| 0      | mask_from_bool_8i::scalar_negate_bool | 11259ns | 11259ns  |
| 1      | mask_from_bool_8i::mask16x8i_set1     | 1008ns  | 1008ns   |
| 1      | mask_from_bool_8i::mask16x8i_ternary  | 1286ns  | 1286ns   |
| 2      | mask_from_bool_8i::mask32x8i_ternary  | 692ns   | 692ns    |
| 2      | mask_from_bool_8i::mask32x8i_set1     | 540ns   | 540ns    |
| 3      | mask_from_bool_8i::mask64x8i_negate   | 182ns   | 182ns    |
| 4      | mask_from_bool_8i::mask32x8i_negate   | 412ns   | 412ns    |
| 4      | mask_from_bool_8i::mask16x8i_negate   | 713ns   | 713ns    |


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
| 0      | popcount_16u::scalar_bitwise_accumulate                 | 58520ns | 58519ns  |
| 0      | popcount_16u::scalar_native                             | 26086ns | 26085ns  |
| 0      | popcount_16u::scalar_byte_lookup                        | 9669ns  | 9668ns   |
| 0      | popcount_16u::scalar_bitwise_accumulate_branchless      | 58259ns | 58257ns  |
| 0      | popcount_16u::scalar_bitwise_divide_and_conquer         | 58536ns | 58535ns  |
| 0      | popcount_16u::scalar_nibble_lookup                      | 16557ns | 16557ns  |
| 1      | popcount_16u::vec8x16u_bitwise_divide_and_conquer       | 2273ns  | 2273ns   |
| 2      | popcount_16u::vec16x16u_pshufb                          | 840ns   | 840ns    |
| 2      | popcount_16u::vec16x16u_pshufb_madd                     | 710ns   | 710ns    |
| 3      | popcount_16u::vec32x16u_bitwise_divide_and_conquer_madd | 620ns   | 620ns    |
| 3      | popcount_16u::vec32x16u_pshufb                          | 483ns   | 483ns    |
| 3      | popcount_16u::vec16x16u_bitwise_divide_and_conquer_madd | 969ns   | 969ns    |
| 3      | popcount_16u::vec32x16u_pshufb_madd                     | 396ns   | 395ns    |
| 3      | popcount_16u::vec8x16u_bitwise_divide_and_conquer_madd  | 1909ns  | 1909ns   |
| 4      | popcount_16u::vec8x16u_pshufb                           | 1814ns  | 1814ns   |
| 4      | popcount_16u::vec8x16u_pshufb_madd                      | 1607ns  | 1607ns   |
| 5      | popcount_16u::scalar_popcnt_op                          | 7024ns  | 7019ns   |
| 6      | popcount_16u::vec8x16u_popcnt_op                        | 814ns   | 812ns    |
| 6      | popcount_16u::vec16x16u_popcnt_op                       | 409ns   | 409ns    |
| 7      | popcount_16u::vec32x16u_vpermi2b                        | 478ns   | 478ns    |
| 7      | popcount_16u::vec32x16u_vpermi2b_madd                   | 378ns   | 378ns    |
| 8      | popcount_16u::vec32x16u_popcnt_op                       | 222ns   | 222ns    |


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
| Config | Implementation                                     | Time     | CPU Time |
| ------ | -------------------------------------------------- | -------- | -------- |
| 0      | popcount_32u::scalar_bitwise_accumulate_branchless | 147754ns | 147750ns |
| 0      | popcount_32u::scalar_bitwise_accumulate            | 29834ns  | 29833ns  |
| 0      | popcount_32u::scalar_bitwise_divide_and_conquer    | 9682ns   | 9682ns   |
| 0      | popcount_32u::scalar_byte_lookup                   | 96760ns  | 96758ns  |
| 0      | popcount_32u::scalar_nibble_lookup                 | 16236ns  | 16235ns  |
| 0      | popcount_32u::scalar_native                        | 13342ns  | 13342ns  |
| 1      | popcount_32u::vec4x32u_bitwise_divide_and_conquer  | 2984ns   | 2984ns   |
| 2      | popcount_32u::vec4x32u_pshufb                      | 2215ns   | 2215ns   |
| 3      | popcount_32u::scalar_popcnt_op                     | 3219ns   | 3219ns   |
| 4      | popcount_32u::vec4x32u_popcnt16_op                 | 1204ns   | 1202ns   |
| 5      | popcount_32u::vec4x32u_popcnt_op                   | 812ns    | 811ns    |
| 5      | popcount_32u::vec8x32u_popcnt_op                   | 410ns    | 410ns    |
| 6      | popcount_32u::vec16x32u_popcnt_op                  | 218ns    | 218ns    |


## popcount_64u
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
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512BW, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

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
| 0      | popcount_64u::scalar_byte_lookup                         | 83286ns  | 83283ns  |
| 0      | popcount_64u::scalar_bitwise_accumulate_branchless       | 121582ns | 121579ns |
| 0      | popcount_64u::scalar_native                              | 7367ns   | 7367ns   |
| 0      | popcount_64u::scalar_nibble_lookup                       | 16208ns  | 16208ns  |
| 0      | popcount_64u::scalar_bitwise_divide_and_conquer          | 8068ns   | 8068ns   |
| 0      | popcount_64u::scalar_bitwise_accumulate                  | 3908ns   | 3908ns   |
| 1      | popcount_64u::vec8x64u_bitwise_divide_and_conquer        | 1054ns   | 1054ns   |
| 1      | popcount_64u::vec8x64u_scalarized                        | 2237ns   | 2237ns   |
| 2      | popcount_64u::vec2x64u_bitwise_divide_and_conquer        | 3361ns   | 3360ns   |
| 3      | popcount_64u::vec4x64u_scalarized                        | 1421ns   | 1421ns   |
| 3      | popcount_64u::vec4x64u_bitwise_divide_and_conquer_sad    | 1002ns   | 1003ns   |
| 3      | popcount_64u::vec4x64u_pshufb_sad                        | 707ns    | 707ns    |
| 3      | popcount_64u::vec4x64u_pshufb                            | 1045ns   | 1045ns   |
| 4      | popcount_64u::vec8x64u_bitwise_divide_and_conquer_sad    | 627ns    | 627ns    |
| 5      | popcount_64u::vec2x64u_bitwise_divide_and_conquer_sad    | 2014ns   | 2014ns   |
| 5      | popcount_64u::vec2x64u_pshufb_sad                        | 1607ns   | 1608ns   |
| 5      | popcount_64u::vec2x64u_pshufb                            | 2412ns   | 2413ns   |
| 6      | popcount_64u::scalar_popcnt_op                           | 1613ns   | 1611ns   |
| 7      | popcount_64u::vec2x64u_popcnt16_op                       | 1209ns   | 1209ns   |
| 7      | popcount_64u::vec4x64u_popcnt16_op                       | 621ns    | 621ns    |
| 7      | popcount_64u::vec4x64u_popcnt16_op_sad                   | 536ns    | 536ns    |
| 7      | popcount_64u::vec2x64u_popcnt16_op_sad                   | 1039ns   | 1040ns   |
| 8      | popcount_64u::vec2x64u_popcnt_op                         | 809ns    | 809ns    |
| 8      | popcount_64u::vec4x64u_popcnt_op                         | 408ns    | 408ns    |
| 9      | popcount_64u::vec8x64u_popcnt_op                         | 221ns    | 221ns    |
| 10     | popcount_64u::vec2x64u_scalarized_popcnt_op              | 1721ns   | 1721ns   |
| 11     | popcount_64u::vec2x64u_scalarized_popcnt_op_with_extract | 1707ns   | 1707ns   |
| 12     | popcount_64u::vec8x64u_popcnt16_op                       | 334ns    | 334ns    |
| 13     | popcount_64u::vec8x64u_popcnt16_op_sad                   | 287ns    | 287ns    |


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
| 0      | popcount_8u::scalar_native                        | 49995ns  | 49994ns  |
| 0      | popcount_8u::scalar_nibble_lookup                 | 19319ns  | 19318ns  |
| 0      | popcount_8u::scalar_bitwise_accumulate            | 153864ns | 153861ns |
| 0      | popcount_8u::scalar_bitwise_accumulate_branchless | 55651ns  | 55650ns  |
| 0      | popcount_8u::scalar_bitwise_divide_and_conquer    | 31056ns  | 31055ns  |
| 0      | popcount_8u::scalar_byte_lookup                   | 103975ns | 103973ns |
| 1      | popcount_8u::vec16x8u_bitwise_divide_and_conquer  | 1803ns   | 1803ns   |
| 2      | popcount_8u::vec32x8u_pshufb                      | 624ns    | 623ns    |
| 2      | popcount_8u::vec32x8u_bitwise_divide_and_conquer  | 809ns    | 810ns    |
| 3      | popcount_8u::vec64x8u_bitwise_divide_and_conquer  | 552ns    | 552ns    |
| 3      | popcount_8u::vec64x8u_pshufb                      | 333ns    | 333ns    |
| 4      | popcount_8u::vec16x8u_pshufb                      | 1412ns   | 1413ns   |
| 5      | popcount_8u::scalar_popcnt_op                     | 11126ns  | 11127ns  |
| 6      | popcount_8u::vec32x8u_popcnt_op                   | 408ns    | 407ns    |
| 6      | popcount_8u::vec16x8u_popcnt_op                   | 811ns    | 811ns    |
| 7      | popcount_8u::vec64x8u_vpermi2b                    | 322ns    | 322ns    |
| 8      | popcount_8u::vec64x8u_popcnt_op                   | 218ns    | 218ns    |


## scalbn_32f
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
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation             | Time    | CPU Time |
| ------ | -------------------------- | ------- | -------- |
| 0      | scalbn_32f::scalar_native  | 11478ns | 11478ns  |
| 1      | scalbn_32f::vec16x32f_avel | 168ns   | 168ns    |
| 2      | scalbn_32f::vec4x32f_avel  | 3228ns  | 3228ns   |
| 3      | scalbn_32f::vec8x32f_avel  | 1378ns  | 1376ns   |


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
| 0      | shift_left_8u::scalar_native               | 8275ns | 8275ns   |
| 1      | shift_left_8u::vec16x8u_divide_and_conquer | 1486ns | 1486ns   |
| 2      | shift_left_8u::vec32x8u_pshufb             | 409ns  | 409ns    |
| 3      | shift_left_8u::vec16x8u_pshufb             | 808ns  | 809ns    |
| 4      | shift_left_8u::vec32x8u_widening           | 421ns  | 421ns    |
| 4      | shift_left_8u::vec16x8u_widening           | 614ns  | 614ns    |


## vec_from_mask_16u
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512BW, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86, AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512BW

### Results:
| Config | Implementation                                | Time    | CPU Time |
| ------ | --------------------------------------------- | ------- | -------- |
| 0      | vec_from_mask_16u::scalar_native_conversion   | 11277ns | 11277ns  |
| 1      | vec_from_mask_16u::vec32x16u_move_mask        | 18.8ns  | 18.8ns   |
| 1      | vec_from_mask_16u::vec16x16u_negation         | 65.8ns  | 65.8ns   |
| 1      | vec_from_mask_16u::vec8x16u_negation          | 240ns   | 240ns    |
| 1      | vec_from_mask_16u::vec32x16u_masked_broadcast | 16.7ns  | 16.7ns   |
| 2      | vec_from_mask_16u::vec8x16u_negation          | 261ns   | 261ns    |
| 2      | vec_from_mask_16u::vec16x16u_negation         | 67.9ns  | 67.9ns   |
| 3      | vec_from_mask_16u::vec16x16u_move_mask        | 58.3ns  | 58.3ns   |
| 3      | vec_from_mask_16u::vec8x16u_move_mask         | 212ns   | 212ns    |
| 3      | vec_from_mask_16u::vec16x16u_masked_broadcast | 62.5ns  | 62.5ns   |
| 3      | vec_from_mask_16u::vec8x16u_masked_broadcast  | 206ns   | 206ns    |


## vec_from_mask_32f
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                                | Time    | CPU Time |
| ------ | --------------------------------------------- | ------- | -------- |
| 0      | vec_from_mask_32f::scalar_native_conversion   | 19297ns | 19297ns  |
| 1      | vec_from_mask_32f::vec4x32f_bitwise_and       | 1066ns  | 1066ns   |
| 2      | vec_from_mask_32f::vec4x32f_masked_broadcast  | 10392ns | 10392ns  |
| 2      | vec_from_mask_32f::vec8x32f_masked_broadcast  | 299ns   | 299ns    |
| 3      | vec_from_mask_32f::vec8x32f_bitwise_and       | 243ns   | 243ns    |
| 4      | vec_from_mask_32f::vec16x32f_masked_broadcast | 81.2ns  | 81.2ns   |


## vec_from_mask_32u
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512DQ

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 5
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512DQ, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 6
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                                | Time    | CPU Time |
| ------ | --------------------------------------------- | ------- | -------- |
| 0      | vec_from_mask_32u::scalar_native_conversion   | 11217ns | 11217ns  |
| 1      | vec_from_mask_32u::vec4x32u_negation          | 1199ns  | 1199ns   |
| 2      | vec_from_mask_32u::vec8x32u_move_mask         | 243ns   | 243ns    |
| 2      | vec_from_mask_32u::vec4x32u_move_mask         | 955ns   | 955ns    |
| 3      | vec_from_mask_32u::vec4x32u_masked_broadcast  | 937ns   | 937ns    |
| 3      | vec_from_mask_32u::vec8x32u_masked_broadcast  | 299ns   | 299ns    |
| 4      | vec_from_mask_32u::vec8x32u_negation          | 243ns   | 243ns    |
| 5      | vec_from_mask_32u::vec16x32u_move_mask        | 70.5ns  | 70.5ns   |
| 6      | vec_from_mask_32u::vec16x32u_masked_broadcast | 80.6ns  | 80.6ns   |


## vec_from_mask_64f
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                               | Time    | CPU Time |
| ------ | -------------------------------------------- | ------- | -------- |
| 0      | vec_from_mask_64f::scalar_native_conversion  | 19342ns | 19341ns  |
| 1      | vec_from_mask_64f::vec2x64f_bitwise_and      | 5010ns  | 5010ns   |
| 2      | vec_from_mask_64f::vec2x64f_masked_movedup   | 4782ns  | 4782ns   |
| 2      | vec_from_mask_64f::vec4x64f_masked_broadcast | 943ns   | 943ns    |
| 3      | vec_from_mask_64f::vec4x64f_bitwise_and      | 948ns   | 948ns    |
| 4      | vec_from_mask_64f::vec8x64f_masked_broadcast | 311ns   | 311ns    |


## vec_from_mask_64u
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512DQ

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 5
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512DQ, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 6
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                               | Time    | CPU Time |
| ------ | -------------------------------------------- | ------- | -------- |
| 0      | vec_from_mask_64u::scalar_native_conversion  | 10735ns | 10735ns  |
| 1      | vec_from_mask_64u::vec2x64u_negation         | 6609ns  | 6609ns   |
| 2      | vec_from_mask_64u::vec2x64u_move_mask        | 4763ns  | 4763ns   |
| 2      | vec_from_mask_64u::vec4x64u_move_mask        | 947ns   | 947ns    |
| 3      | vec_from_mask_64u::vec2x64u_masked_broadcast | 4808ns  | 4807ns   |
| 3      | vec_from_mask_64u::vec4x64u_masked_broadcast | 942ns   | 942ns    |
| 4      | vec_from_mask_64u::vec4x64u_negation         | 951ns   | 951ns    |
| 5      | vec_from_mask_64u::vec8x64u_move_mask        | 271ns   | 271ns    |
| 6      | vec_from_mask_64u::vec8x64u_masked_broadcast | 310ns   | 310ns    |


## vec_from_mask_8u
### Configurations:
#### Config 0
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0

#### Config 1
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512BW, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 2
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* SSE2, X86, AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3

#### Config 3
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512VL, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86, AVX512BW

### Results:
| Config | Implementation                              | Time    | CPU Time |
| ------ | ------------------------------------------- | ------- | -------- |
| 0      | vec_from_mask_8u::scalar_native_conversion  | 11232ns | 11231ns  |
| 1      | vec_from_mask_8u::vec16x8u_negation         | 60.8ns  | 60.8ns   |
| 1      | vec_from_mask_8u::vec64x8u_move_mask        | 3.60ns  | 3.60ns   |
| 1      | vec_from_mask_8u::vec64x8u_masked_broadcast | 3.57ns  | 3.57ns   |
| 1      | vec_from_mask_8u::vec32x8u_negation         | 17.1ns  | 17.1ns   |
| 2      | vec_from_mask_8u::vec32x8u_negation         | 13.7ns  | 13.7ns   |
| 2      | vec_from_mask_8u::vec16x8u_negation         | 59.0ns  | 59.1ns   |
| 3      | vec_from_mask_8u::vec16x8u_move_mask        | 58.3ns  | 58.3ns   |
| 3      | vec_from_mask_8u::vec32x8u_move_mask        | 14.0ns  | 14.0ns   |
| 3      | vec_from_mask_8u::vec32x8u_masked_broadcast | 15.3ns  | 15.3ns   |
| 3      | vec_from_mask_8u::vec16x8u_masked_broadcast | 60.6ns  | 60.6ns   |


## vec_from_scalar_16u
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
* SSSE3, SSE3, SSE2, X86

### Results:
| Config | Implementation                                       | Time   | CPU Time |
| ------ | ---------------------------------------------------- | ------ | -------- |
| 0      | vec_from_scalar_16u::scalar_identity                 | 4815ns | 4815ns   |
| 1      | vec_from_scalar_16u::vec8x16u_mul_shuffle            | 9637ns | 9637ns   |
| 1      | vec_from_scalar_16u::vec8x16u_unpack_shuffle         | 8558ns | 8557ns   |
| 1      | vec_from_scalar_16u::vec8x16u_shift_shuffle          | 9656ns | 9655ns   |
| 2      | vec_from_scalar_16u::vec16x16u_broadcast             | 6414ns | 6410ns   |
| 2      | vec_from_scalar_16u::vec8x16u_broadcast_from_vector  | 7958ns | 7958ns   |
| 2      | vec_from_scalar_16u::vec16x16u_broadcast_from_vector | 7482ns | 7483ns   |
| 2      | vec_from_scalar_16u::vec8x16u_broadcast              | 6376ns | 6377ns   |
| 3      | vec_from_scalar_16u::vec32x16u_broadcast             | 6778ns | 6779ns   |
| 3      | vec_from_scalar_16u::vec32x16u_broadcast_from_vector | 8245ns | 8246ns   |
| 4      | vec_from_scalar_16u::vec8x16u_pshufb                 | 7986ns | 7987ns   |


## vec_from_scalar_32f
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
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512BW, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                                       | Time   | CPU Time |
| ------ | ---------------------------------------------------- | ------ | -------- |
| 0      | vec_from_scalar_32f::scalar_identity                 | 2434ns | 2434ns   |
| 1      | vec_from_scalar_32f::vec16x32f_broadcast             | 3353ns | 3353ns   |
| 2      | vec_from_scalar_32f::vec4x32f_shuffle                | 3211ns | 3211ns   |
| 3      | vec_from_scalar_32f::vec4x32f_broadcast              | 3203ns | 3204ns   |
| 3      | vec_from_scalar_32f::vec8x32f_broadcast_from_vector  | 3179ns | 3180ns   |
| 3      | vec_from_scalar_32f::vec4x32f_broadcast_from_vector  | 3204ns | 3204ns   |
| 3      | vec_from_scalar_32f::vec8x32f_broadcast              | 3183ns | 3183ns   |
| 4      | vec_from_scalar_32f::vec16x32f_broadcast_from_vector | 3321ns | 3321ns   |


## vec_from_scalar_32u
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
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512BW, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                                       | Time   | CPU Time |
| ------ | ---------------------------------------------------- | ------ | -------- |
| 0      | vec_from_scalar_32u::scalar_identity                 | 2402ns | 2401ns   |
| 1      | vec_from_scalar_32u::vec16x32u_broadcast             | 3351ns | 3351ns   |
| 2      | vec_from_scalar_32u::vec4x32u_shuffle                | 3184ns | 3184ns   |
| 3      | vec_from_scalar_32u::vec8x32u_broadcast              | 3186ns | 3186ns   |
| 3      | vec_from_scalar_32u::vec4x32u_broadcast_from_vector  | 3207ns | 3207ns   |
| 3      | vec_from_scalar_32u::vec8x32u_broadcast_from_vector  | 3205ns | 3205ns   |
| 3      | vec_from_scalar_32u::vec4x32u_broadcast              | 3207ns | 3207ns   |
| 4      | vec_from_scalar_32u::vec16x32u_broadcast_from_vector | 3334ns | 3334ns   |


## vec_from_scalar_64f
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
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512BW, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                                      | Time   | CPU Time |
| ------ | --------------------------------------------------- | ------ | -------- |
| 0      | vec_from_scalar_64f::scalar_identity                | 1227ns | 1227ns   |
| 1      | vec_from_scalar_64f::vec8x64f_broadcast             | 1673ns | 1673ns   |
| 2      | vec_from_scalar_64f::vec2x64f_shuffle               | 1594ns | 1594ns   |
| 3      | vec_from_scalar_64f::vec2x64f_broadcast             | 1610ns | 1608ns   |
| 3      | vec_from_scalar_64f::vec4x64f_broadcast             | 1603ns | 1603ns   |
| 3      | vec_from_scalar_64f::vec2x64f_movedup               | 1611ns | 1611ns   |
| 3      | vec_from_scalar_64f::vec4x64f_broadcast_from_vector | 1603ns | 1604ns   |
| 4      | vec_from_scalar_64f::vec8x64f_broadcast_from_vector | 1662ns | 1662ns   |


## vec_from_scalar_64u
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
* AVX2, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

#### Config 4
* g++-12 (Ubuntu 12.3.0-1ubuntu1~22.04) 12.3.0
* AVX512BW, AVX512F, AVX2, FMA, AVX, SSE42, SSE41, SSSE3, POPCNT, SSE3, SSE2, X86

### Results:
| Config | Implementation                                      | Time   | CPU Time |
| ------ | --------------------------------------------------- | ------ | -------- |
| 0      | vec_from_scalar_64u::scalar_identity                | 1343ns | 1342ns   |
| 1      | vec_from_scalar_64u::vec8x64u_broadcast             | 1675ns | 1675ns   |
| 2      | vec_from_scalar_64u::vec2x64u_shuffle               | 1604ns | 1604ns   |
| 3      | vec_from_scalar_64u::vec2x64u_broadcast             | 1601ns | 1601ns   |
| 3      | vec_from_scalar_64u::vec2x64u_broadcast_from_vector | 1592ns | 1593ns   |
| 3      | vec_from_scalar_64u::vec4x64u_broadcast_from_vector | 1596ns | 1596ns   |
| 3      | vec_from_scalar_64u::vec4x64u_broadcast             | 1599ns | 1599ns   |
| 4      | vec_from_scalar_64u::vec8x64u_broadcast_from_vector | 1668ns | 1668ns   |


## vec_from_scalar_8u
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
* SSSE3, SSE3, SSE2, X86

### Results:
| Config | Implementation                                     | Time    | CPU Time |
| ------ | -------------------------------------------------- | ------- | -------- |
| 0      | vec_from_scalar_8u::scalar_identity                | 8564ns  | 8564ns   |
| 1      | vec_from_scalar_8u::vec16x8u_unpack_shuffle        | 16131ns | 16131ns  |
| 2      | vec_from_scalar_8u::vec16x8u_broadcast_from_vector | 12878ns | 12863ns  |
| 2      | vec_from_scalar_8u::vec32x8u_broadcast             | 12849ns | 12852ns  |
| 2      | vec_from_scalar_8u::vec32x8u_broadcast_from_vector | 12932ns | 12934ns  |
| 2      | vec_from_scalar_8u::vec16x8u_broadcast             | 12805ns | 12807ns  |
| 3      | vec_from_scalar_8u::vec64x8u_broadcast             | 13491ns | 13493ns  |
| 3      | vec_from_scalar_8u::vec64x8u_broadcast_from_vector | 13572ns | 13573ns  |
| 4      | vec_from_scalar_8u::vec16x8u_pshufb                | 12783ns | 12784ns  |

