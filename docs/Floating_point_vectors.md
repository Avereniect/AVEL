# Floating-point Vectors
* wrappers presenting a similar interface as the following native types
  * `float` assumed to be a 32-bit IEEE-754 single-precision float
  * `double` assumed to be a 64-bit IEEE-754 double-precision float
* floating-point functions will round correctly and deliver expected results 
  for all edge cases
* however exception state is not generally respected

## Vector Class
### Static Constants
`static constexpr std::uint32_t width`
* number of lanes in vector

### Type Aliases
`using scalar`
* the type fo teh vector's elements

`using primitive = ...`
* the vector's backing type

`using mask = Vector_mask<scalar, width>;`
* corresponding instantiation of `avel::Vector_mask`

`using rebind_type<U>`
* corresponding instantiation of `avel::Vector<U, width>`

`using rebind_width<M>`
* corresponding instantiation of `avel::Vector<scalar, M>`

### Constructors
`Vector(Vector<U, N> v)` where `U` is convertible to `T`
* construct a vector where each lane is a conversion of the value in `v` to type `T`
* semantics of conversion are consistent with `avel::convert`

`Vector(mask m)`
* construct a vector where each lane is `1.0` if `m` is `true` and `0.0` otherwise

`explicit Vector(scalar x)`
* constructs a vector whose lanes are all copies of `x`

`explicit Vector(primitive p)`
* constructs a vector by copying the contents of `p`

Defaulted default/copy/move constructors

`Vector(const arrNxT arr)`
* constructs a vector by loading the elements from `arr`

### Assignment Operators
`Vector& operator=(primitive p)`  
* assigns internal variable to `p`

`vector& operator=(scalar x)`
* set each lane to be a copy of `x`  

copy/move assignment operators are defaulted

### Comparison Operators
`mask operator==(Vector lhs, Vector rhs) const`
* for each lane, performs an equality comparison between `lhs` and `rhs

`mask operator!=(Vector lhs, Vector rhs) const`
* for each lane, performs an inequality comparison between `lhs` and `rhs`

`mask operator<(Vector lhs, Vector rhs) const`
* for each lane, performs a less-than comparison between `lhs` and `rhs`

`mask operator<=(Vector lhs, Vector rhs) const`
* for each lane, performs a less-than-or-equal comparison between `lhs` and 
  `rhs`

`mask operator>(Vector lhs, Vector rhs) const`
* for each lane, performs a greater-than comparison between `lhs` and `rhs`

`mask operator>=(Vector lhs, Vector rhs) const`
* for each lane, performs a greater-than-or-equal comparison between `lhs` 
  and `rhs`

### Unary Arithmetic Operators
Vector operator+() const`
* returns the input unchanged

`Vector<signed T, width> operator-() const`
* for each lane, produces the negated value of the current value
* produces `-0.0` if `v` is `+0.0`
* produces `+0.0` if `v` is `-0.0`
* produces `-INF` if `v` is `+INF`
* produces `+INF` if `v` is `-INF`
* produces `NAN` if `v` is `NAN`

### Arithmetic Assignment Operators
`Vector& operator+=(Vector rhs)`
* for each lane, adds `rhs` to the current value in `this`
* produces `NAN` if either input is `NAN`
* produces `+INF` if one input is `+INF` and the other is not NAN or `-INF`
* produces `-INF` if one input is `-INF` and the other is not NAN or `+INF`
* produces `NAN` if one input is `-INF` or `+INF`

`Vector& operator-=(Vector rhs)`
* for each lane, subtracts `rhs` from the current value in `this`
* produces `NAN` if either input is `NAN`
* produces `NAN` if both inputs are infinities

`Vector& operator*=(Vector rhs)`
* for each lane, multiplies `rhs` against the current value in `this`

`Vector& operator/=(Vector rhs)`
* for each lane, divides the current value in `this` by `rhs`
* operation in inexact on ARM without AARCH64 being enabled
* division by 0 leads to unspecified behavior

`Vector& operator%=(Vector rhs)`
* for each lane, computes the remainder of the dividing `this` by `rhs`
* consider use of denominator classes
* division by 0 leads to unspecified behavior

### Arithmetic Operators
### Increment/Decrement Operators
`Vector& operator++()`
* for each lane, increments `this` by `1.0`

`Vector operator++(int)`
* for each lane, increments `this` by `1.0` and returns a copy of `this` before 
    mutation

`Vector& operator--()`
* for each lane, decrements `this` by `1.0`

`Vector operator--(int)`
* for each lane, decrements `this` by `1.0` and returns a copy of `this` before 
  mutation

### Conversion Operators
`explicit operator primitive() const`
* returns a copy of the internal instance of the backing type for use with 
  native intrinsics

`explicit operator mask() const`
* for each lane, produces true if the element is non-zero and false otherwise

## Free Functions
### Arrangement Operations
```
template<std::uint32_t N>
scalar extract(vector v)
```
* returns the contents of the `N`th lane of the vector `v`

```
template<std::uint32_t N>
vector insert(vector v, scalar x)
```
* replace the contents of the `N`th lane with `x` in a copy of `v`
* the contents of all other lanes are left untouched

### General Vector Operations
`vector keep(mask m, vector v)`
* for each lane, return `v` if `m` is true and `0.0` otherwise

`vector clear(mask m, vector v)`
* for each lane, return `0.0` if `m` is true and `v` otherwise

`vector blend(mask m, vector a, vector b)`
* for each lane, return `a` if `m` is true and `b` otherwise

`vector byteswap(vector v)`
* for each lane, reverses the order of bytes within `v`

`vector max(vector a, vector b)`
* for each lane, returns `a` if `a > b` and `b` otherwise

`vector min(vector a, vector b)`
* for each lane, returns `a` if `a < b` and `b` otherwise

`std::array<vector, 2> minmax(vector a, vector b)`
* produces an array containing the less and greater of `a` and `b` 
  respectively
* delivers the same results as `min(a, b)` and `max(a, b)`

`vector clamp(vector x, vector lo, vector hi)`
* for each lane, clamps the values `x` to the range [lo, hi]
* if `x` is NAN, produces `x`
* output is unspecified if `hi` is less than `lo`
* output is unspecified if either `lo` or `hi` are `NAN`

`vector negate(mask m, vector v)`
* for each lane, negates the value in `v` if `m` is set
* produces `NAN` if `v` is `NAN`
* the signs of zeros may not be negated properly

`vector abs(vector v)`
* for each lane, computes the absolute value of `v`
* produces `+0.0` if `v` is `+0.0` or `-0.0f`
* produces `+INF` if `v` is `+INF` or `-INF`
* produces `NAN` if `v` is `NAN`

`vector neg_abs(vector v)`
* for each lane, computes the negative absolute value of `v`
* produces `-0.0` if `v` is `+0.0` or `-0.0f`
* produces `-INF` if `v` is `+INF` or `-INF`
* produces `NAN` if `v` is `NAN`



### Load/Store operations
```
template<class V, N = V::width>  
vector load(const scalar* p)
```
* loads the first `N` values from the address `p` into corresponding lanes
* likely to perform best when `N == V::width`

```
template<class V>
vector load(const scalar* p, std::uint32_t n)
```
* loads the first `n` values from the address `p` into corresponding lanes
* if `n` is greater than the width of the vector, loads `V::width` elements
* likely to perform worse than an equivalent call to `load<n>(p)`

```
template<class V, N = V::width>
vector aligned_load(const scalar* p)
```
* loads the first `N` values from the address `p` into corresponding lanes
* `p` must be aligned to `alignof(vector)`
* likely to perform best when `N == V::width`
* may perform better than an equivalent call to `load(p, N)`

```
template<class V>
vector aligned_load(const scalar* p, std::uint32_t n)
```
* loads the first `n` values from the address `p` into corresponding lanes
* `p` must be aligned to `alignof(vector)`
* may perform better than an equivalent call to `load`

`template<class V, N = V::width>`  
`vector gather(const scalar* p, ivector indices)`
* for the first `N` lanes, loads the value located at `p[indices]`
* may perform better than individual loads on machine with AVX2
* only available for vectors of 32-bit and 64-bit  elements

`template<class V>`  
`vector gather(const scalar* p, ivector indices, std::uint32_t n)`
* for the first `n` lanes, loads the value located at `p[indices]`
* may perform better than individual loads on machine with AVX2
* only available for vectors of 32-bit and 64-bit elements

`template<class N = vector::width>`  
`void store(scalar* p, vector x)`
* stores `x`'s elements beginning at `p`
* likely to perform best when `N == V::width`

`void store(scalar* p, vector x, std::uint32_t n)`
* stores `x`'s first `n` elements beginning at `p`

`template<class N = vector::width>`  
`void aligned_store(scalar* p, vector x)`
* stores `x`'s elements beginning at `p`
* `p` must be aligned to `alignof(vector)`
* may perform better than an equivalent call to `store`

`void aligned_store(scalar* p, vector x, std::uint32_t n)`
* stores `x`'s first `n` elements beginning at `p`
* `p` must be aligned to `alignof(vector)`
* may perform better than an equivalent call to `store`

`template<class N = vector::width>`
`void scatter(scalar* p, ivector indices, vector x)`
* for the first `N` lanes, store the value to `p[indices]`
* may perform better than individual loads on machine with AVX512F

`void scatter(scalar* p, ivector indices, vector x, std::uint32_t n)`
* for the first `n` lanes, store the value to `p[indices]`
* may perform better than individual loads on machine with AVX512F



## Conversions
`std::array<V0, ...> convert<V0, V1>(V1 v)`
* returns and array of vectors containing the converted elements contained 
  within `v`
* currently, only conversions between vectors of integers of equal width 
  and size are supported
  * e.g. conversions to self type or conversions to unsigned/signed 
    counterparts

### CMath Functions
#### General Floating-Point Operations
`vector fmax(vector a, vector b)`
* for each lane, produces the lesser of `a` and `b`
* if one of `a` or `b` is `NAN` then produces the other

`vector fmin(vector a, vector b)`
* for each lane, produces the greater of `a` and `b`
* if one of `a` or `b` is `NAN` then produces the other

#### Nearest Integer Operations
`vector ceil(vector v)`
* for each lane, rounds `v` to the nearest integer not less than `v`

`vector floor(vector v)`
* for each lane, rounds `v` to the nearest integer not greater than `v`

`vector trunc(vector v)`
* for each lane, rounds `v` to the nearest integer not greater in 
    magnitude than `v`

`vector round(vector v)`
* for each lane, rounds `v` to the nearest integer
* halfway cases are rounded away from zero

`vector nearbytint(vector v)`
* for each lane, rounds `v` to the nearest integer as consistent with the 
  current rounding mode
* if the current rounding mode is `FE_TONEAREST`, then halfway cases will 
  round to even

`vector rint(vector v)`
* for each lane, rounds `v` to the nearest integer as consistent with the 
  current rounding mode
* if the current rounding mode is `FE_TONEAREST`, then halfway cases will 
  round to even

#### Floating-Point Manipulation
`vector frexp(vector num, ivector* exp)`
* for each lane, returns the normalized  significand of `num` and writes the 
  exponent of `num` to `exp`
* for positive `num`, the significand is normalized to the `[+0.5, +1.0)` 
  range
* for negative `num`, the significand is normalized to the `(-1.0, -0.5]` 
  range
* produces `num` if `num` is `+0.0` or `-0.0` and writes `0` to `exp`
* produces `num` if `num` is `+INFINITY` or `-INFINITY` and an 
  unspecified value is written to `exp` 
* produces `NAN` if `num` is `NAN` and an unspecified value is written to `exp`

`vector ldexp(vector num, ivector exp)`
* for each lane, returns `v` times `2` raised to the power of `num`
* produces `v` if `v` is `+0.0` or `-0.0`
* produces `v` if `num` is `0`
* produces `v` if `v` is `+INFINITY` or `-INFINITY`
* produces `NAN` if `v` is `NAN`

`vector scalbn(vector v, ivector num)`
* for each lane, returns `v` times `FLT_RADIX` raised to the power of `num`
* produces `v` if `v` is `+0.0` or `-0.0`
* produces `v` if `num` is `0`
* produces `v` if `v` is `+INFINITY` or `-INFINITY`
* produces `NAN` if `v` is `NAN`

`ivector ilogb(vector v)`
* for each lane, returns `v`'s unbiased exponent as an integral value
* produces `FP_ILOGB0` if `v` is `0.0f`
* produces `FP_ILOGB0` if `v` is `NAN`
* produces `INT_MAX` if `v` is `+INF`
* produces `INT_MAX` if `v` is `-INF`

`vector logb(vector v)`
* for each lane, returns `v`'s unbiased exponent as a floating-point value
* produces `NAN` if `v` is `0.0f`
* produces `NAN` if `v` is `NAN`
* produces `+INF` if `v` is `+INF`
* produces `+INF` if `v` is `-INF`

`vector copysign(vector mag, vector sign)`
* for each lane, returns a value equal in magnitude to `mag` and with the 
  same sign as `sign`

#### Floating-Point Classification
`ivector fpclassify(vector v)`
* for each lane, returns an implementation-defined integer value identifying 
  the category to which `v` belongs
* integer values may be `FP_NORMAL`, `FP_SUBNORMAL`, `FP_ZERO`, `FP_INFINITE`,
  `FP_NAN`

`mask isfinite(vector v)`
* for each lane, returns `true` if `v` is a finite value, `false` otherwise

`mask isinf(vector v)`
* for each lane, returns `true` if `v` is an infinite value, `false` otherwise

`mask isnan(vector v)`
* for each lane, returns `true` if `v` is a NAN value, `false` otherwise

`mask isnormal(vector v)`
* for each lane, returns `true` if `v` is a normal value

`mask signbit(vector v)`
* for each lane, returns `true` if the sign bit of `v` is set, `false` 
  otherwise
* checks the sign bit of `+infinity`, `-infinity`, and `NAN`

#### Floating-point Comparisons
`mask isgreater(vector x, vector y)`
* for each lane, returns `true` if `x` is greater than `y`
* if either `x` or `y` is `NAN`, returns `false`

`mask isgreaterequal(vector x, vector y)`
* for each lane, returns `true` if `x` is greater than or equal to `y`
* if either `x` or `y` is `NAN`, returns `false`

* `mask isless(vector x, vector y)`
* for each lane, returns `true` if `x` is less than `y`
* if either `x` or `y` is `NAN`, returns `false`

`mask islessequal(vector x, vector y)`
* for each lane, returns `true` if `x` is less than or equal to `y`
* if either `x` or `y` is `NAN`, returns `false`

`mask islessgreater(vector x, vector y)`
* for each lane, returns `true` if `x` is less than or greater than `y`
* if either `x` or `y` is `NAN`, returns `false`

`mask isunordered(vector x, vector y)`
* for each lane, return `true` if either `x` or `y` is `NAN`
