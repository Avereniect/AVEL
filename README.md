# AVEL - Another Vector Extensions Library
## Feature Overview
* AVEL is a C++11 library that provides abstractions over SIMD instruction 
  sets for x86 and ARM
* this is primarily done through the use of classes which represent vector 
  registers
  * currently unsigned and signed integers of 8, 16, 32, and 64-bit integers 
    are supported
  * plans for supporting floats, doubles in the future
  * currently only 128-bit vectors are supported
  * plans to support 256-bit and 512-bit vectors on x86
* the library aims to be convenient and so emulates the interface of C++'s 
  native feature set and its standard library, offering vectorized versions of 
  the same functionality including:
  * operator overloads
  * misc. utilities from various headers such as `min`, `max`, `clamp`, and 
    `midpoint`, and `abs`
  * C++ 20's `<bit>` header
  * misc. AVEL-specific functions such as `negate`, `blend`, `minmax`, and 
    `broadcast_mask`
* AVEL also offers scalar implementations of functions offering the same 
  behavior as their vectorized counterparts in the `avel/Scalar.hpp` header 
* to accelerate division AVEL also offers Denominator classes which at the 
  cost of some upfront work, will be able to compute quotients and moduli 
  more quickly
* `avel::Aligned_allocator` can be used to ensure that allocations meet an 
  alignment requirement. This allows for the use of aligned load instructions 
  which may perform better than non-aligned load instructions. Ideally, if 
  performing any non-trivial amount of allocation you'd use another 
  allocator for performance reasons however

## Usage
* requires C++ 11 and a supported version of GCC, Clang, or ICX
  * see compiler support section at the end of this file for further details
* AVEL offers various headers in the `include/avel/` directory
  * all of these headers may be included by including `avel/Avel.hpp`
  * before including any of AVEL's header files, a list of macros should be 
    defined in order to indicate which SIMD ISA extensions you wish to use
  * see [Capabilities](./docs/Capabilities.md) for a list of macros
  * Instead of specifying each individual SIMD ISA extensions,
    `AVEL_AUTO_DETECT` may be defined and the library will identify which ISA 
    extensions the compiler's target architecture supports
* depending on which ISA extensions are enabled, specializations of the 
  `avel::Vector` and `avel::Vector_mask` classes will be provided
* operations on these vectors are implemented in a way that leverages 
  the SIMD capabilities of the target architecture
* since these vector classes are meant to represent vector registers, they 
  should be used in a manner that is different from most other types
  * it is discouraged to use these types for anything other than 
    manipulating data. e.g. Don't make a `std::vector<avel::vec4x32i>` object
  * data should be explicitly loaded and stored
  * using too many objects of these types may cause register spillover and 
    cause the compiler to emit undesired stores and loads
* the library is built using SIMD intrinsics which unfortunately have poor 
  performance without optimization but compile to near optimal code even at 
  low optimization levels.
  * consider using the `-O1` or `-Og` optimization flags during non-release 
    builds 
  * the debug assembly code is generally more readable with these optimizations 
    enabled than not
* it's encouraged to take a look at the underlying implementations to garner 
  intuition behind the cost of various functions
  * while some implementations while not necessarily ideal in 
    their instruction count, will still generally offer better performance than 
    scalar code due to the sheer throughput of the vectorized approach

## SIMD Vectors Aliases
* a different set of specializations of `avel::Vector` will be available 
  depending on which feature set macros were defined
* although the direct use of `avel::Vector` is possible, it is not recommended
* AVEL defines aliases for vectors containing a specific quantity of 
  specific element types e.g. if `AVEL_SSE2` or `AVEL_NEON` have been defined
  then AVEL will offer:
  * `vec16x8u`
  * `vec16x8i`
  * `vec8x16u`
  * `vec8x16i`
  * `vec4x32u`
  * `vec4x32i`
  * `vec2x64u`
  * `vec2x64i`
* AVEL also defines aliases that don't feature a specific length but instead 
  use `N` or `M`
  * `vecNx8u`  / `vecMx8u`
  * `vecNx8i`  / `vecMx8i`
  * `vecNx16u` / `vecMx16u`
  * `vecNx16i` / `vecMx16i`
  * `vecNx32u` / `vecMx32u`
  * `vecNx32i` / `vecMx32i`
  * `vecNx64u` / `vecMx64u`
  * `vecNx64i` / `vecMx64i`
* `N` indicates the natural width of a vector given the specified CPU features. 
  These vectors will offer AVEL's full feature set. 
* `M` indicates the maximum width of a vector given the specified CPU 
  features. Only loading, storing, and vector conversions are guaranteed 
  to be supported for these vectors
* corresponding aliases for `avel::Vector_mask` and `std::array` are also 
  provided with the `vec` prefix replaced with the `mask` and `arr` prefixes 
  respectively

## Documentation
* the library's documentation simply consists of Markdown files provided in the 
  docs directory of this repo
* since most of the functionality that the library provides is simply a 
  vectorized version of features provided by the C++ core language and the 
  C++'s standard library, it should largely be easy to extrapolate how to 
  utilize the library
* Hence, the documentation for the standard library should suffice in most 
  cases with the understanding this library will simply perform the specified 
  operation for as many lanes as there are in a vector.
* Most operations on `avel::Vector<T, x>` should have the same semantics as the 
  corresponding operations on `T` with some notable exceptions:
  * explicit conversions must be made to convert a `avel::Vector<T, N>` 
    object to a `avel::Vector<U, N>` object even if there is an implicit 
    conversion from `T` to `U`.
  * signed integer overflow and underflow is well-defined to exhibit the 
    behavior expected of integers with a twos-complement representation
  * shifting integers by amounts equal to the number of bits they contain is 
    well-defined to continue to shift in zeros or ones
    * the behavior shifting by larger amounts or by negative amounts is 
      unspecified for performance reasons
  * comparison operators on vectors perform element-wise comparisons and hence 
    return vector mask objects. The functions `avel::all`, `avel::any`, 
    `avel::count`, and `avel::none` can be used on masks to produce a scalar 
    boolean value

## Usage Examples

### Populating Vectors
```
#define AVEL_AUTO_DETECT
#include <avel/Vectors.hpp>

#include <cstdint>

using avel::vec4x32u;
using avel::load;

int main() {
    // Fills v0 with 4 copies of 1024
    vec4x32u v0{1024}
    
    // Load contents from a previously declared std::array object
    std::array<std::uint32_t, 4> arr1{256, 512, 1024, 2048};
    vec4x32u v1{{arr1}};

    // Load contents from anonymous std::array object
    vec4x32u v2{{256, 512, 1024, 2048}};
  
    // Load data from arbitrary memory location
    auto v3 = avel::load<vec4x32u>(arr1.data());
  
    // Load data from memory address aligned to alignof(vec4x32u)
    alignas(16) std::int32_t raw_array[4] = {256, 512, 1024, 2048};
    auto v4 = avel::load_aligned<vec4x32u>(arr1.data());
}
```

### Utilizing a General Purpose Operation
```c++
#define AVEL_AUTO_DETECT
#include <avel/Vectors.hpp>

#include <cstdint>
#include <vector>

using avel::vec4x32u;

void clamp_vector(const std::vector<std::uint32_t>& values) {
    for (std::size_t i = 0; i < values.size(); i += vec4x32u::width) {
        auto values = avel::load<vec4x32u>(values.data() + i);
        auto clamped_values = avel::clamp(values, 0, 10);
        avel::store(clamped_values, ptr);
    }

    //Handle partials
    auto remainder = values.size() % vec4x32u::width;
    auto value = avel::load<vec4x32u>(ptr, remainder);
    auto clamped_values = avel::clamp(values, 0, 10);
    avel::store(clamped_values, ptr, remainder);
}
```

### Use of Reciprocal Classes
```c++
#include AVEL_AUTO_DETECT
#include <avel/Vectors.hpp>

#include <cstdint>

using avel::vec4x32u;

int main() {
    std::uint32_t x = 10;
    Denom32u d{x};
    
    std::uint32_t z = 467;
    
    // Div can be used to compute both quotients and remainders in one call
    auto qr = div(z, d);
    std::uint32_t quotient = qr[0]; //Equal to 46
    std::uint32_t remainder = qr[1]; // Equal to 7
    
    
    return 0;
}

```

### Use of Vector Reciprocal Classes
```c++
#include AVEL_AUTODETECT

int main() {
    Denom32u d{10};
    // Vector denominators can be constructed from scalar denominators
    Denom4x32u denom_vector{d};
    
    vec4x32u v0{{10, 20, 30, 40}};

    // Can be used to divide each lane by 10, 20, 30, and 40 respectively
    Denom4x32u dv{v0}; 
    
    vec4x32u v1{{194, 994, 395, 973}};
    
    auto quotients = v1 / dv; //Equal to (19, 49, 13, 24)

    return 0;
}
```

### Use of Aligned Allocator
```c++
// Use of the allocator by itself doesn't require specifying any ISA extensions
#include <avel/Allocator>

int main() {
    avel::aligned_allocator<std::uint32_t, 64> allocator;
    std::vector<std::uint32_t, decltype(allocator)> vec{allocator};
    
    return 0;
}


```

### Usage Warnings
* If the target machine architecture doesn't support a particular operation 
  via a dedicated instruction, it will be emulated using other instructions, 
  which may lead to certain operations being disproportionately expensive 
  compared to the equivalent operations on scalars, or the same operations 
  performed if using a more modern instruction set

## Compiler Support
* the library's tests have been run and passed on:
  * x86
    * GCC 9.5.0, 10.4.0, 11.3.0, and 12.1.0
    * Clang 11.1.0-6,  12.0.1, 13.0.1, 14.0.0
    * ICX 2023.0.0
  * ARM
    * GCC 9.3.0-22, 10.2.1-6
    * Clang 9.0.10-16.1, 11.0.1-2, 13.0.1-6
* presumably newer/intermediate versions of these compilers will also work 
  however it has not been tested which are the oldest versions on which this 
  library will function
* support for MSVC is planned
