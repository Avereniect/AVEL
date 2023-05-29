# Floating-point Vectors

## Vector Class
### Static Constants
* `static constexpr std::uint32_t width`
  * number of lanes in vector

### Type Aliases
* `using scalar`
  * the type fo teh vector's elements

* `using primitive = ...`
  * the vector's backing type

* `using mask = Vector_mask<scalar, width>;`
  * corresponding instantiation of `avel::Vector_mask`

* `using rebind_type<U>`
  * corresponding instantiation of `avel::Vector<U, width>`

* `using rebind_width<M>`
  * corresponding instantiation of `avel::Vector<scalar, M>`

### Constructors
* `Vector(Vector<U, N> v)` where `U` is convertible to `T`
  * construct a vector where each lane is a conversion of the value in `v` to type `T`
  * semantics of conversion are consistent with `avel::convert`

* `Vector(mask m)`
  * construct a vector where each lane is `1.0` if `m` is `true` and `0.0` otherwise

* `explicit Vector(scalar x)`
  * constructs a vector whose lanes are all copies of `x`

* `explicit Vector(primitive p)`
  * constructs a vector by copying the contents of `p`

* default default, copy, move constructors

* `Vector(const arrNxT arr)`
  * constructs a vector by loading the elements from `arr`

### Assignment Operators
* `Vector& operator=(primitive p)`
  * assigns internal variable to `p`

* `vector& operator=(scalar x)`
  * set each lane to be a copy of `x`

* defaulted copy/move assignment operators

### Comparison Operators
* `mask operator==(Vector lhs, Vector rhs) const`
  * for each lane, performs an equality comparison between `lhs` and `rhs`

* `mask operator!=(Vector lhs, Vector rhs) const`
  * for each lane, performs an inequality comparison between `lhs` and `rhs`

* `mask operator<(Vector lhs, Vector rhs) const`
  * for each lane, performs a less-than comparison between `lhs` and `rhs`

* `mask operator<=(Vector lhs, Vector rhs) const`
  * for each lane, performs a less-than-or-equal comparison between `lhs` and 
    `rhs`

* `mask operator>(Vector lhs, Vector rhs) const`
  * for each lane, performs a greater-than comparison between `lhs` and `rhs`

* `mask operator>=(Vector lhs, Vector rhs) const`
  * for each lane, performs a greater-than-or-equal comparison between `lhs` 
    and `rhs`

### Unary Arithmetic Operators
* `Vector operator+() const`
  * returns the input unchanged

* `Vector<signed T, width> operator-() const`
  * for each lane, produces the negated value of the current value
  * produces `-0.0` if `v` is `+0.0`
  * produces `+0.0` if `v` is `-0.0`
  * produces `-INF` if `v` is `+INF`
  * produces `+INF` if `v` is `-INF`
  * produces `NAN` if `v` is `NAN`

### Arithmetic Assignment Operators
* `Vector& operator+=(Vector rhs)`
  * for each lane, adds `rhs` to the current value in `this`
  * produces `NAN` if either input is `NAN`
  * produces `+INF` if one input is `+INF` and the other is not NAN or `-INF`
  * produces `-INF` if one input is `-INF` and the other is not NAN or `+INF`
  * produces `NAN` if one input is `-INF` or `+INF`

* `Vector& operator-=(Vector rhs)`
  * for each lane, subtracts `rhs` from the current value in `this`
  * produces `NAN` if either input is `NAN`
  * produces `NAN` if both inputs are infinities

* `Vector& operator*=(Vector rhs)`
  * for each lane, multiplies `rhs` against the current value in `this`

* `Vector& operator/=(Vector rhs)`
  * for each lane, divides the current value in `this` by `rhs`
  * operation in inexact on ARM without AARCH64 being enabled
  * division by 0 leads to unspecified behavior

* `Vector& operator%=(Vector rhs)`
  * for each lane, computes the remainder of the dividing `this` by `rhs`
  * consider use of denominator classes
  * division by 0 leads to unspecified behavior

### Arithmetic Operators
### Increment/Decrement Operators
* ``
### Conversion Operators
* `explicit operator primitive() const`
* `explicit operator mask() const`

## Free Functions
### Arrangement Operations
* `template<std::uint32_t N>``
  `scalar extract(vector v)`

* `template<std::uint32_t N>`
  `vector insert(vector v, scalar x)`

### General Vector Operations
* `vector keep(mask m, vector v)`
  * for each lane, return `v` if `m` is true and `0.0` otherwise

* `vector clear(mask m, vector v)`
  * for each lane, return `0.0` if `m` is true and `v` otherwise

* `vector blend(mask m, vector a, vector b)`
  * for each lane, return `a` if `m` is true and `b` otherwise

* `vector byteswap(vector v)`
  * for each lane, reverses the order of bytes within `v`

* `vector max(vector a, vector b)`
  * for each lane, returns `a` if 

* `vector min(vector a, vector b)`
  * for each lane, returns `b` if 

* `std::array<vector, 2> minmax(vector a, vector b)`
  * produces a 

* `vector clamp(vector x, vector lo, vector hi)`
  * for each lane, clamps the values `x` to the range [lo, hi]
  * if `x` is NAN, produces `x`
  * output is unspecified if `hi` is less than `lo`
  * output is unspecified if either `lo` or `hi` are NAN

* `vector negate(mask m, vector v)`
  * for each lane, negates the value in `v` if `m` is set
  * produces `NAN` if `v` is `NAN`
  * produces `+0.0` if `m` is false and `v` is `+0.0` 
  * produces `-0.0` if `m` is true and `v` is `+0.0` 
  * produces `-0.0` if `m` is false and `v` is `-0.0` 
  * produces `+0.0` if `m` is true and `v` is `-0.0` 

* `vector abs(vector v)`
  * for each lane, computes the absolute value of `v`
  * produces `+0.0` if `v` is `+0.0` and `-0.0f`
  * produces `+INF` if `v` is `+INF` and `-INF`
  * produces `NAN` if `v` if `NAN`

* `vector neg_abs(vector v)`
  * for each lane, computes the negative absolute value of `v`
  * produces `-0.0` if `v` is `+0.0` and `-0.0f`
  * produces `-INF` if `v` is `+INF` and `-INF`
  * produces `NAN` if `v` if `NAN`

* `vector average(vector a, vector b)`
* `vector midpoint(vector a, vector b)`

### Load/Store Operations
*

## Conversions
* `std::array<V0, ...> convert<V0, V1>(V1 v)`
  * returns and array of vectors containing the converted elements contained 
    within `v`
  * currently, only conversions between vectors of integers of equal width 
    and size are supported
    * e.g. conversions to self type or conversions to unsigned/signed 
      counterparts

### CMath Functions
#### General Floating-Point Operations
* `vector fmax(vector a, vector b)`
* `vector fmin(vector a, vector b)`

#### Rounding Operations
* `vector ceil(vector v)`
* `vector floor(vector v)`
* `vector trunc(vector v)`
* `vector round(vector v)`
* `vector nearbytint()`
  * a 

* `vector rint()`
  * a

#### Floating-Point Manipulation
* `vector frexp(vector num, ivector* exp)`
  * for each lane, returns a normalized fraction and write the exponent of `num` into `exp`

* `vector ldexp(vector num, ivector exp)`
  * for each lane, returns

* `vector scalbn(vector v, ivector num)`
  * a

  * `ivector ilogb(vector v)`
    * a

* `vector logb(vector v)`
  * a

* `vector copysign(vector mag, vector sign)`
  * for each lane, returns a value equal in magnitude to `mag` and with the 
    same sign as `sign`

#### Floating-Point Classification
* `ivector fpclassify(vector v)`
  * for each lane, returns an implementation-defined integer value identifying 
    the category to which `v` belongs
  * integer values may be `FP_NORMAL`, `FP_SUBNORMAL`, `FP_ZERO`, 
    `FP_INFINITE`, `FP_NAN`

* `mask isfinite(vector v)`
  * for each lane, returns `true` if `v` is a finite value, `false` otherwise

* `mask isinf(vector v)`
  * for each lane, returns `true` if `v` is an infinite value, `false` otherwise

* `mask isnan(vector v)`
  * for each lane, returns `true` if `v` is a NAN value, `false` otherwise
* `mask isnormal(vector v)`
  * for each lane, returns `true` if `v` is a normal value

* `mask signbit(vector v)`
  * for each lane, returns `true` if the signbit of `v` is set, `false` otherwise
