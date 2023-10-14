# Vector Masks
* vector masks represent a vector of boolean values
* for each vector type, there is a corresponding vector mask type

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
`std::uint32_t count(mask m)`
* return the number of set lanes in the `m`

`bool any(mask m)`
* return true if any lanes in `m` are set to true

`bool all(mask m)`
* return true if all lanes in `m` are set to true

`bool none(mask m)`
* return true if all lane in `m` are set to false

```
template<std::uint32_t N>
bool extract(mask m)
```
* returns the contents of the `N`th lane of the vector mask `m`

```
template<std::uint32 N>
mask insert(mask m, bool b)
```
* replace the contents of the `N`th lane with `b` in a copy of `v`
* the contents of all other lanes are left untouched