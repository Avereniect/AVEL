# Denominators
AVEL offers the `Denominator` class to accelerate division in the case that 
the divisor is known ahead of time, and will be used repeatedly. It may not 
be faster than using AVEL's division operators if the quotients are consistently
very small in magnitude. 

Like for `avel::Vector` and `avel::Vector_mask`, aliases for 
`avel::Denominator` are also offered, this time with a `denom` prefix, e.g. 
`denom4x32u` or `denom32u` for the scalar counterpart.

## `avel::Denominator<T>`
### Type Aliases
* `using backing_type = T`

### -ctors
* `Denominator(T d)`
  * constructs a denominator object which can be used to accelerate division 
    by `d`. If `T` is a scalar type and dis equal to zero then the resulting 
    object is ill-formed. If `T` is a specialiation of `avel::Vector`, and any 
    lane of `d` is equal to zero, then the results for those corresponding 
    lanes are unspecified in all future operations.

* `Denominator(scalar_denominator d)`
  * exists only if `T` is a specialization of `avel::Vector`
  * Broadcasts the scalar denominator, to form a vector of denominators
* copy constructor is trivial
* move constructor is trivial
* destructor is trivial

### Assignment operators
* copy assignment is trivial
* move assignment is trivial

### Arithmetic Operations
* `avel::div_type<backing_type> div(backing_type n, Denominator d)`
  * produces the quotient and remainder of dividing `n` by the value used 
    to construct `d`

* `backing_type operator/(backing_type lhs, Denominatior d)`
  * produces the quotient of dividing `n` by the value used to construct `d`

* `backing_type operator%(backing_type lhs, Denominator d)`
  * produces the remainder of dividing `n` by the value used to construct `d`

* `backing_type& operator/=(backing_type lhs, Denominator d)`
  * assigns lhs the quotient of dividing `n` by the value used to construct `d`

* `backing_type& operator/=(backing_type lhs, Denominator d)`
  * assigns lhs the remainder of dividing `n` by the value used to construct `d`

* `Denom8u& operator<<=(T s)`
  * multiplies the denominator value by two raised to the power of `s`
  * the value of `s` is clamped such that the result does not produce an 
    inexact result
  * undefined results if `s` is negative
  * verification of the updated denominator value via the `value` method is 
    recommended when in doubt

* `Denom8u& operator>>=(T s)`
  * divides the denominator value by two raised to the power of `s`
  * the value of `s` is clamped such that the result does not produce an 
    inexact result
  * undefined results if `s` is negative
  * verification of the updated denominator value via the `value` method is 
    recommended when in doubt

* `Denom8u operator<<(T s) const`
  * multiplies the denominator value by two raised to the power of `s`
  * the value of `s` is clamped such that the result does not produce an 
    inexact result
  * undefined results if `s` is negative
  * verification of the updated denominator value via the `value` method is 
    recommended when in doubt

* `Denom8u operator>>(T s) const`
  * divides the denominator value by two raised to the power of `s`
  * the value of `s` is clamped such that the result does not produce an 
    inexact result
  * undefined results if `s` is negative
  * verification of the updated denominator value via the `value` method is 
    recommended when in doubt

### Accessors
* `T value() const`
  * returns the encapsulated denominator value