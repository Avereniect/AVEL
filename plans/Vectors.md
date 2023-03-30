# Vectors
* wrappers around native types
  * float, double
  * std::uint32_t, std::int32_t
  * std::uint8_t, std::int8_t
  * std::uint16_t, std::int16_t
  * std::uint64_t, std::int64_t
* perform all processing in vectorized fashion

## Signed Integer vectors
* shifting by negative amounts is currently undefined
* consider using only unsigned quantities for right shift clamping.

## Vector mask class requirements:
* implicit class alignment 
* `using primitive = `
* `Vector_mask(primitive)`
* `Vector_mask(bool)`
* defaulted default/copy/move/constructors
* defaulted destructor
* `Vector_mask& operator=(bool)`
* default copy/move assignment operators
* `Vector_mask& operator&=(Vector_mask)`
* `Vector_mask& operator|=(Vector_mask)`
* `Vector_mask& operator^=(Vector_mask)`
* `Vector_mask& operator~()`
* `Vector_mask& operator&(Vector_mask)`
* `Vector_mask& operator|(Vector_mask)`
* `Vector_mask& operator^(Vector_mask)`
* `bool operator[](int i)`
* `operator primitive()`

## Vector class requirements:

## General vector operations:
* blend
* max
* min
* abs

* load
* aligned_load
* stream_load
* gather

* store
* aligned_store
* stream_store
* scatter

## Integer vector operations:
* popcnt
* countl_zero
* countl_one
* countr_zero
* countr_one

* floor2 ()
* ceil2 ()
* mod_pow2 ()
* is_pow2
* log2()

* set_range(int from, int end)
* set_range(int n)

* rotl
* rotr

* byteswap
* 

## Floating-point vector operations:
### AVEL specific floating-point functions
* average
* epsilon_increment
* epsilon_decrement
* epsilon_offset
### cmath basic operations
* fabs
* fmod
* remainder
* remquo
* fma
* fmax
* fmin
* fdim
### Linear Interpolation
* lerp
### Exponential functions
* exp
* exp2
* expm1
* log
* log10
* log2
* log1p
### Power functions
* pow
* sqrt
* cbrt
* hypot
### Trigonometric functions
* sincos
* sin
* cos
* tan
* asin
* acos
* atan
* atan2
### Nearest integer floating-point operations
* ceil
* floor
* trunc
* round
* nearbyint
### Floating-point manipulation functions
* frexp
* ldexp
* modf
* scalbn
* ilogb
* logb
* nextafter
* nexttoward
* copysign
### Classification
* fpclassifu
* isfinite
* isinf
* isnan
* isnormal
* signbit
### Comparison
* isgreater
* isgreaterequal
* isless
* islessequal
* islessgreater
* isunordered