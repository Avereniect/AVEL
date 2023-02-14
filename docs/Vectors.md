# Vectors
* wrappers around the following native types
  * `std::uint8_t`, `std::int8_t`
  * `std::uint16_t`, `std::int16_t`
  * `std::uint32_t`, `std::int32_t`
  * `std::uint64_t`, `std::int64_t`

## Vector Mask Classes:
* associated type alias `maskNxT = avel::Vector_mask<..., width`
* associated type alias `arrbxN = std::array<bool, width>`
* `std::is_trivial<Vecor_mask>` will evaluate to true

* `static constexpr std::uint32_t width = ...`
  * number of lanes in vector mask

* `using primitive = ...`
  * alias of corresponding type used by native intrinsics

* `Vector_mask(primitive p)`
  * assigns internal variable to `p`

* `Vector_mask(bool b)`
  * broadcasts value of `b` to all lanes

* `Vector_mask(arrbxN arr)`
  * sets all lanes to corresponding values from `arr`

* defaulted default/copy/move constructors

* defaulted destructor

* `Vector_mask& operator=(bool b)`
  * broadcasts value of `b` to all lanes

* default copy/move assignment operators

* `Vector_mask& operator&=(Vector_mask rhs)`
  * for each lane, sets the lane to the logical AND of `this` and `rhs`

* `Vector_mask& operator|=(Vector_mask rhs)`
  * for each lane, sets the lane to the logical OR of `this` and `rhs`

* `Vector_mask& operator^=(Vector_mask rhs)`
  * for each lane, sets the lane to the logical XOR of `this` and `rhs`

* `Vector_mask operator~()`
  *  for each lane, produces the bitwise negation of `this`

* `Vector_mask operator&(Vector_mask rhs)`
  *  for each lane, produces the bitwise AND of `this` and `rhs`

* `Vector_mask operator&&(Vector_mask rhs)`
  *  for each lane, produces the bitwise AND of `this` and `rhs`

* `Vector_mask operator|(Vector_mask rhs)`
  *  for each lane, produces the bitwise OR of `this` and `rhs`

* `Vector_mask operator||(Vector_mask rhs)`
  *  for each lane, returns a component-wise OR of `this` and `rhs`

* `Vector_mask operator^(Vector_mask rhs)`
  *  for each lane, produces the bitwise XOR of `this` and `rhs`

* `explicit operator primitive()`
  * returns a copy of the internal instance of the backing type for use with 
    native intrinsics

### Free Mask Functions
* `std::uint32_t count(mask m)`
  * return number of set lanes in the `m`

* `bool any(mask m)`
  * return true if any lanes in `m` are set to true

* `bool all(mask m)`
  * return true if all lanes in `m` are set to true

* `bool none(mask m)`
  * return true if all lane in `m` are set to false

## Vector Classes:
* associated type alias `vecNxT = avel::Vector<scalar, width>`
* associated type alias `arrNxT = std::array<scalar, width>`

* `static constexpr std::uint32_t width = ...`
  * number of lanes in vector

### Type aliases
* `using scalar`
  * the type of the vector's elements

* `using primitive = ...`
  * backing type

* `using mask = Vector_mask<scalar, width>;`
  * corresponding instantiation of `Vector_mask`

* `using rebind_type<U>`
  * corresponding instantiation of `Vector<U, width>`

* `using rebind_width<M>`
  * corresponding instantiation of `Vector<scalar, M>`

### Constructors
* `Vector(Vector<U, N> v)` where U is convertible to T
  * for each lane, converts `v` to be of type `T` in manner 
    consistent with `avel::convert` function

* `Vector(mask m)`
  * for each lane, produces `1` if `m` is set and `0` otherwise

* `explicit Vector(scalar x)`
  * set each lane to be a copy of `x`

* `explicit Vector(primitive p)`
  * assigns internal variable to be `p`

* defaulted default/copy/move constructors

* `Vector(const arrNxT arr)`
  * loads elements from `arr` into lanes of vector

### Assignment Operators 
* `Vector& operator=(primitive p)`
  * assigns internal variable to `p`

* `vector& operator=(scalar x)`
  * set each lane to be a copy of `x`

### Comparison Operators
* `mask operator==(Vector lhs, Vector rhs) const`
  * for each lane, performed an equality comparison between `lhs` and `rhs`

* `mask operator!=(Vector lhs, Vector rhs) const`
  * for each lane, performed an inequality comparison between `lhs` and `rhs`

* `mask operator<(Vector lhs, Vector rhs) const`
  * for each lane, performed a less-than comparison between `lhs` and `rhs`

* `mask operator<=(Vector lhs, Vector rhs) const`
  * for each lane, performed a less-than-or-equal comparison between `lhs` and 
    `rhs`

* `mask operator>(Vector lhs, Vector rhs) const`
  * for each lane, performed a greater-than comparison between `lhs` and `rhs`

* `mask operator>=(Vector lhs, Vector rhs) const`
  * for each lane, performed a greater-than-or-equal comparison between `lhs` 
    and `rhs`

### Arithmetic Assignment Operators
* `Vector& operator+=(Vector rhs)`
  * for each lane, adds `rhs` to the current value in `this`

* `Vector& operator-=(Vector rhs)`
  * for each lane, subtracts `rhs` from the current value in `this`

* `Vector& operator*=(Vector rhs)`
  * for each lane, multiplies `rhs` against the current value in `this`

* `Vector& operator/=(Vector rhs)`
  * for each lane, divides the current value in `this` by `rhs`
  * consider use of denominator classes
  * output is unspecified for a particular lane if the denominator is 0

* `Vector& operator%=(Vector rhs)`
  * for each lane, computes the remainder of the dividing `this` by `rhs`
  * consider use of denominator classes
  * output is unspecified for a particular lane if the denominator is 0

### Arithmetic Operators
* `Vector operator+(Vector rhs) const`
  * for each lane, produces the sum of `this` and rhs

* `Vector operator-(Vector rhs) const`
  * for each lane, produces the difference of `this` and rhs

* `Vector operator*(Vector rhs) const`
  * for each lane, produces the product of `this` and rhs

* `Vector operator/(Vector rhs) const`
  * for each lane, produces the quotient of `this` and rhs
  * output is unspecified for a particular lane if the denominator is 0

* `Vector operator%(Vector rhs) const`
  * for each lane, produces the modulus of `this` and rhs
  * output is unspecified for a particular lane if the denominator is 0

### Increment/Decrement Operators
* `Vector& operator++()`
  * for each lane, increments `this` by 1

* `Vector operator++(int)`
  * for each lane, increments `this` by 1 and returns a copy of `this` before 
    mutation

* `Vector& operator--()`
  * for each lane, decrements `this` by 1

* `Vector operator--(int)`
  * for each lane, decrements `this` by 1 and returns a copy of `this` before 
    mutation

### Bitwise Assignment Operators
* `Vector operator&=(Vector rhs)`
  * for each lane, sets `this` to the bitwise AND of `this` and `rhs`

* `Vector operator|=(Vector rhs)`
  * for each lane, sets `this` to the bitwise OR of `this` and `rhs`

* `Vector operator^=(Vector rhs)`
  * for each lane, sets `this` to the bitwise XOR of `this` and `rhs`

### Bitwise Operators
* `Vector operator~() const`
  * for each lane, produces the bitwise negation of `this`

* `Vector operator&(Vector rhs) const`
  * for each lane, produces the bitwise and of `this` and `rhs`

* `Vector operator|(Vector rhs) const`
  * for each lane, produces the bitwise or of `this` and `rhs`

* `Vector operator^(Vector rhs) const`
  * for each lane, produces the bitwise xor of `this` and `rhs`

### Conversions
* `explicit operator primitive() const`
  * returns a copy of the internal instance of the backing type for use with 
    native intrinsics

* `explicit operator mask() const`
  * for each lane, produces true if the element is non-zero and false otherwise

### Unary Arithmetic Operators
* `Vector operator+() const`
  * returns the input unchanged

* `Vector<signed T, width> operator-() const`
  * for each lane, produces the negated value of the current value
  * for vectors of unsigned integers, treats input as signed integer

### Bitwise Assignment Operators
* `Vector operator>>=(long long rhs) const`
  * for each lane, shifts the value left by `rhs` bits

* `Vector operator<<=(long long rhs) const`
  * for each lane, shifts the value left by `rhs` bits
* `Vector operator>>=(vector rhs) const`
  * for each lane, shifts the value left by `rhs` bits
  * may perform poorly on x86 without AVX2 support
  
* `Vector operator<<=(vector rhs) const`
  * for each lane, shifts the value left by `rhs` bits
  * may perform poorly on x86without AVX2 support

### Bitwise Operators
* `Vector operator>>(long long rhs) const`
  * for each lane, shifts the value left by `rhs` bits

* `Vector operator<<(long long rhs) const`
  * for each lane, shifts the value left by `rhs` bits

* `Vector operator>>(vector rhs) const`
  * for each lane, shifts the value left by `rhs` bits
  * may perform poorly on x86 without AVX2 support

* `Vector operator<<(vector rhs) const`
  * for each lane, shifts the value left by `rhs` bits
  * may perform poorly on x86without AVX2 support

## General Vector Operations:
* `vector` refers to a specialization of `avel::Vector`
* `mask` refers to `vector::mask`
* `scalar` refers to `vector::scalar`

### Arrangement Operations
* `template<std::uint32_t N>`
  `scalar extract(vector v)`
  * returns the contents of the `N`th lane of the vector `v`

* `template<std::uint32_t N`
  `vector insert(vector v, scalar x)`
  * replace the contents of the `N`th lane with `x` in the vector `v`
  * the contents of all other lanes are left untouched

### Logical/Arithmetic
* `vector broadcast_mask(mask m)`
  * for each lane, sets all bits to 0 if `m` is false or to 1 if `m` is true

* `vector blend(mask m, vector a, vector b)`
  * for each lane, selects `a` if `m` is true and `b` if `m` is false
  * note that this is the same order as the parameters to the ternary operator

* `vector byteswap(vector x)`
  * for each lane reverses the ordering of the bytes in each element

* `vector max(vector a, vector b)`
  * for each lane, if `a < b`, returns `b`, otherwise returns `a`

* `vector min(vector a, vector b)`
  * for each lane, if `a < b`, returns `a`, otherwise returns `b`

* `std::array<vector, 2> minmax(vector x, vector y)`
  * returns a pair containing {min(x, y), max(x, y)}

* `vector clamp(vector x, vector lo, vector hi)`
  * for each lane, clamps the value in `x` to the range of [lo, hi]
  * behavior is unspecified if `lo` is not less than `hi`

* `vector negate(mask m, vector x)`
  * conditionally negate values in `x` if `m` is set
  * not available for vectors of unsigned integers
  * if the vector contains signed integers and the negated value cannot be 
    represented, this function produces a bit pattern which, if reinterpreted 
    as an unsigned integer, has the correct value

* `vector abs(vector x)`
  * for each lane, computes the absolute value of `x`
  * if the vector contains signed integers whose negated value cannot be 
    represented, this function produces a bit pattern which, if 
    reinterpreted as an unsigned integer, has the correct value

* `vector neg_abs(vector x)`
  * for each lane, computes the negative absolute value of `x`

* `vector average(vector a, vector b)`
  * computes the average of two values without overflow
  * rounds in manner consistent with `(a + b) / 2` i.e. towards zero

* `vector midpoint(vector a, vector b)`
  * for each lane, computes the value in the middle of `a` and `b` in a manner 
    consistent with C++ 20's `std::midpoint`
  * for each lane, computes the average value of `a` and `b`.
  * if the true midpoint lies exactly between two representable values, 
    chooses the one which is closest to `a`
  * on vectors of floating-point types, the output for a particular lane is 
    unspecified if either input is `NAN`, `+INF` or `-INF`



### Load/Store
* `template<class V, N = V::width>`  
  `vector load(const scalar* p)`
  * loads the first `N` values from the address `p` into corresponding lanes
  * likely to perform best when `N == V::width`

* `template<class V>`  
  `vector load(const scalar* p, std::uint32_t n)`
  * loads the first `n` values from the address `p` into corresponding lanes
  * if `n` is greater than the width of the vector, loads `V::width` elements
  * likely to perform worse than an equivalent call to `load<n>(p)`

* `template<class V, N = V::width>`  
  `vector aligned_load(const scalar* p)`
  * loads the first `N` values from the address `p` into corresponding lanes
  * `p` must be aligned to `alignof(vector)`
  * likely to perform best when `N == V::width`
  * may perform better than an equivalent call to `load(p, N)`

* `template<class V>`  
  `vector aligned_load(const scalar* p, std::uint32_t n)`
  * loads the first `n` values from the address `p` into corresponding lanes
  * `p` must be aligned to `alignof(vector)`
  * may perform better than an equivalent call to `load`

* `template<class V, N = V::width>`
  `vector gather(const scalar* p, ivector indices)`
  * for the first `N` lanes, loads the value located at `p[indices]`
  * may perform better than individual loads on machine with AVX2
  * only available for vectors of 32-bit and 64-bit  elements

* `template<class V>`
  `vector gather(const scalar* p, ivector indices, std::uint32_t n)`
  * for the first `n` lanes, loads the value located at `p[indices]`
  * may perform better than individual loads on machine with AVX2
  * only available for vectors of 32-bit and 64-bit elements

* `template<class N = vector::width>`
  `void store(scalar* p, vector x)`
  * stores `x`'s elements beginning at `p`
  * likely to perform best when `N == V::width`

* `void store(scalar* p, vector x, std::uint32_t n)`
  * stores `x`'s first `n` elements beginning at `p`

* `template<class N = vector::width>`
  `void aligned_store(scalar* p, vector x)`
  * stores `x`'s elements beginning at `p`
  * `p` must be aligned to `alignof(vector)`
  * may perform better than an equivalent call to `store`

* `void aligned_store(scalar* p, vector x, std::uint32_t n)`
  * stores `x`'s first `n` elements beginning at `p`
  * `p` must be aligned to `alignof(vector)`
  * may perform better than an equivalent call to `store`

* `template<class N = vector::width>`
  `void scatter(scalar* p, ivector indices, vector x)`
  * for the first `N` lanes, store the value to `p[indices]`
  * may perform better than individual loads on machine with AVX2
  * only available for vectors of 32-bit and 64-bit  elements

* `void scatter(scalar* p, ivector indices, vector x, std::uint32_t n)`
  * for the first `n` lanes, store the value to `p[indices]`
  * may perform better than individual loads on machine with AVX2
  * only available for vectors of 32-bit and 64-bit  elements

## Conversions
* `arrNxT to_array(vector x) const`
  * stores elements within analogous `std::array` object

* `std::array<V0, ...> convert<V0, V1>(V1 v)`
  * returns and array of vectors containing the converted elements contained 
    within `v`
  * currently, only conversions between vectors of integers of equal width 
    and size are supported
    * e.g. conversions to self type or conversions to unsigned/signed 
      counterparts

## Integer Vector Operations
* `avel::div_type<vector> div(vector x, vector y)`
  * performs an integer division of `x` by `y` yielding both quotient and 
    remainder
  * it is unspecified what happens to a particular lane if `y` equals zero
    * otherwise equivalent to C++11's `std::div`
  * note that the algorithm is implemented in software due to lack of hardware 
    integer division instructions
  * elements in each input vector should be of a similar magnitude to each 
    other to minimize the work done
  * elements `x` should not be substantially larger magnitudes than 
    corresponding elements in `y` to maximize performance
  * consider use of denominator classes for improved performance

* `vector popcount(vector v)`
  * for each lane, computes the number of set bits in `v`

* `vector countl_zero(vector v)`
  * for each lane, computes the number of contiguous high bits which aren't set

* `vector countl_one(vector v)`
  * for each lane, computes the number of contiguous high bits which are set

* `vector countr_zero(vector v)`
  * for each lane, computes the number of contiguous low bits which aren't set

* `vector countr_one(vector v)`
  * for each lane, computes the number of contiguous low bits which are set

* `vector bit_floor(vector v)`
  * for each lane, rounds `v` down to the previous power of two
  * rounds 0 to 0
  * for signed integer vectors containing negative values, the behavior is 
    undefined

* `vector bit_width(vector v)`
  * for each lane, computes the number of bits necessary to represent the 
    value in `v`

* `vector bit_ceil(vector b)`
  * for each lane, rounds `v` up to the next power of two
  * for unsigned integer vectors, values greater than 
    `std::numeric_limits<scalar>::max() / 2`, produce 0
  * for signed integer vectors, values greater than 
    `std::numeric_limits<scalar>::max() / 2`, produce 
    `std::numeric_limits<scalar>::min()`

* `mask has_single_bit(vector x)`
  * for each lane, produces true if `x` has a single bit set. False otherwise

### Bit Manipulation Instructions
* `vector bit_shift_left<S>(vector x)`
  * performs a bitwise left shift on all lanes in `x` by `S` bits
  * may perform marginally better than shifting by a runtime scalar

* `vector bit_shift_right<S>(vector x)`
  * performs a bitwise right shift on all lanes in `x` by `S` bits
  * may perform marginally better than shifting by a runtime scalar

* `vector rotl<S>(vector x)`
  * performs a bitwise left rotation on all lanes in `x` by `S` bits
  * may perform marginally better than rotating by a runtime scalar

* `vector rotl(vector x, scalar s)`
  * performs a bitwise left rotation on all lanes by s bits
  * likely to perform suboptimally on x86 on 8-bit elements

* `vector rotl(vector x, vector s)`
  * for each lane, performs a bitwise left rotation by s bits
  * likely to perform suboptimally on x86 on 8-bit elements
  * likely to perform poorly without AVX2 support

* `vector rotr<S>(vector x)``
  * performs a bitwise right rotation on all lanes in `x` by `S` bits
  * may perform marginally better than rotating by a runtime scalar

* `vector rotr(vector x, scalar s)`
  * performs a bitwise right rotation on all lanes in `x` by `s` bits
  * likely to perform suboptimally on x86 on 8-bit elements

* `vector rotr(vector x, vector s)`
  * for each lane, performs a bitwise right rotation on `x` by `s` bits
  * likely to perform suboptimally on x86 on 8-bit elements
  * likely to perform poorly without AVX2 support
