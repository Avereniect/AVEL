# Denominators
* AVEL offers denominator classes to accelerate division in the case that 
  the divisor is known ahead of time, and will be used repeatedly
* may not be faster than the alternative if the quotients are consistently very 
  small in magnitude

## Denominator Classes
### Type Aliases
* `using backing_type = ...`

### -ctors
* `Denominator(scalar d)`
  * constructs a denominator object which can be used to accelerate division 
    by `d`
* `Denominator(scalar_denominator d)`
  * exists only if `backing_type` is a specialization of `avel::Vector`
  * constructs a denominator object which can be used to accelerate division 
    by the values in `d`

### Arithmetic Operations
* `avel::div_type<backing_type> div(backing_type n, Denominator d)`
  * produces the quotient and remainder of dividing `n` by the value tused 
    to construct `d`

* `backing_type operator/(backing_type lhs, Denominatior d)`
  * produces the quotient of dividing `n` by the value used to construct `d`

* `backing_type operator%(backing_type lhs, Denominator d)`
  * produces the remainder of dividing `n` by the value used to construct `d`

* `backing_type& operator/=(backing_type lhs, Denominator d)`
  * assigns lhs the quotient of dividing `n` by the value used to construct `d`

* `backing_type& operator/=(backing_type lhs, Denominator d)`
  * assigns lhs the remainder of dividing `n` by the value used to construct `d`
