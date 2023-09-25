# AVEL - Another Vector Extensions Library

## Background: The SIMD Paradigm
Most CPU code is scalar. This is to say that each instruction processes a 
single set of inputs at a time. For example, a common add instruction will 
typically take in two integers, and produce a third. However, modern CPU's have
long had vector instructions that perform the same operation on multiple 
inputs. For example, any x86-64 CPU features 128-bit  registers which can 
hold four 32-bit integers at a time. A vectorized add instruction can be used on
two such registers to compute four sums in just one instruction. This paradigm 
is known as SIMD, or Single-Instruction, Multiple-Data. As should be easy to 
imagine, processing data in this vectorized fashion can greatly improve 
performance.

Unfortunately, taking advantage of these instructions is far from convenient. 
These instructions can be used directly within assembly code, but writing 
assembly is not practical at scale. Alternatively, compilers may take 
advantage of these instructions in simple cases such as summing over an array, 
but their ability to vectorize code in the general case is poor when compared to
that of a human. A third option would be to use intrinsics, functions that 
correspond to individual CPU instructions. Intriniscs are however an 
inconvenient interface with names that are difficult to parse and quite verbose.
Not to mention that the use of intriniscs is not portable and they do little 
to remedy the fact that there are many gaps in the instructions available on 
various platforms. For example, x86 does not have unsigned integer comparisons 
until AVX-512, (32i, 32i) -> (32i) multiplication is not part of the base 
x86-64 ISA, and there aren't any vectorized integer division instructions at 
all. However, more often than not, it's possible to cheaply emulate these 
operations such there the higher through still makes for an increase in 
overall performance. At the same time, it's also inconvenient and filling in 
all these gap takes a fair bit of time. Additionally, certain operations may be 
complex to emulate efficiently even if it's possible to do so, making it 
unlikely that you'll do it in practice, as you probably don't have weeks to 
ponder the best way to perform division.

An important concept in SIMD is the distinction between the 
Structure-of-Arrays(SoA) approach and the Arrays-of-Structures(AoS) approach. 
Scalar code is typically written in an AoS approach where, as the name 
implies, data is organized into arrays that contain structs. However, this 
approach is not ideal for vectorization or for performance in general since 
it's common to only make use of small portion of the data contained with 
each struct. This means that there is poor usage of the available memory 
bandwidth and valuable time is spent on shuffling data around to populate vector 
registers. Hence, under this approach, SIMD is generally used to accelerate the 
processing of each individual element (where there is generally not a lot of 
potential parallelism to exploit) instead of processing multiple structs in 
parallel. Under a SoA approach, this is not an issue. Here, there is a 
single structure which contains an array for each member of the 
corresponding structure in the AoS approach. This means that data of the 
same type and purpose is stored contiguously and may hence be placed into a 
vector register with a single load instruction and there is little to no wasted 
memory bandwidth. Granted, it's certainly more difficult to interact with 
individual elements using this approach, but will generally lead to improved 
performance when it comes to processing elements in bulk.

AVEL aims to offer a convenient general-purpose interface suited for SoA 
approaches to vectorization where the gaps are filled. where vectorized code 
is almost as easy to write as scalar code, and where there is a fair degree 
of portability.

## Overview
AVEL is a C++11 library that provides abstractions over SIMD instruction sets 
for x86 and ARM. This is primarily done through the use of classes which 
represent vector registers containing integers or floats.
* unsigned and signed integers of 8, 16, 32, and 64-bit integers are supported 
  on all targets. 
* 32 and 64-bit floats are supported on x86 and ARM with Neon and AARCH64
  * testing has not yet been done on AARCH32
* 128-bit vectors are available on ARM with Neon
* 128-bit, 256-bit and 512-bit vectors are available on x86

AVEL also offers various pieces of functionality that would be useful in the 
context of writing vectorized code.

## Features
The library aims to be convenient to use and so emulates the interface of C++'s 
native feature set and that of its standard library, offering vectorized 
versions of the same functionality including:
* all integer and floating-point operators
* misc. utilities from various headers such as `min`, `max`, `clamp`, and 
  `midpoint`, and `abs`
* most of the functions from C++ 20's `<bit>` header
* a selection of float-manipulation functions from the `<cmath>` header 
* misc. AVEL-specific functions such as `negate`, `blend`, `keep`, `clear`, 
  `set_bits` which are useful in the context of writing vectorized code

AVEL also offers scalar implementations of all of its vectorized facilities in 
the `avel/Scalar. hpp` header. These will also take advantage of scalar 
instructions native to the architecture being targeted. The behavior of 
these functions should match the behavior of each individual lane within a 
corresponding vector for all possible inputs. Note that certain inputs may 
be invalid for specific functions and results are not guaranteed to match. 

To accelerate division AVEL also offers Denominator classes which at the cost of
some upfront work, will be able to compute quotients and moduli more quickly 
after their construction.

Facilities to manage caches through prefetching are also available.

AVEL's `Aligned_allocator` class can be used to ensure that allocations meet an 
alignment requirement. This allows for the use of aligned load instructions 
which may perform better than non-aligned load instructions. However, it is 
recommended to use more specialized allocators if performing any non-trivial 
number of allocations. The performance characteristics of this allocator are not
guaranteed to be consistent. It's primarily offered as a simple convenience.

AVEL's simplicity of implementation is considered to be one of its features.
It's recognized that using a library is not suitable for all situations. 
AVEL was deliberately written with the intent of being easy to read so that 
AVEL may serve as a set of reference implementations for functions which may 
take a long time to create implementations for, or for which there may simply 
not be enough interest to implement. In this way, those who stick to using 
intrinsics or writing assembly may also benefit from the library as well.

## Usage
Requires C++ 11 and a supported version of GCC, Clang, or ICX. See the 
[Compiler Support](#compiler-support) section at the end of this file for 
further details

AVEL is a header-only library so it's facilities will be accessed by 
including the appropriate headers offers various headers in the `include/avel/`
directory. As an convenience, all of these headers may be included by including 
the `avel/Avel.hpp` header.

For each CPU feature set you wish to use, you must first pass the appropriate 
flags to your compiler. 
* See [GCC's x86 flags](https://gcc.gnu.org/onlinedocs/gcc/x86-Options.html) 
  and [GCC's ARM flags](https://gcc.gnu.org/onlinedocs/gcc/ARM-Options.html)
  * The relevant flags should also work if targeting Clang or ICPX
* As a matter of convenience, the `-march=native` flag may be passed to 
  GCC, Clang, or ICPX to indicate that the compiler should target the CPU 
  on which the compiler is running.
* On Linux systems you may inspect the `/proc/cpuinfo` and inspect the flags 
  listings to see which features your CPU supports
  * Alternatively, the `lscpu` command can present you with the same 
    information in a slightly more convenient format 

Before including any of AVEL's header files, a list of macros should be defined
to indicate which ISA extensions you wish to use. See 
[Capabilities](./docs/Capabilities.md) for a list of these macros.

Alternatively, instead of defining the macros individually, 
`AVEL_AUTO_DETECT` may be defined instead and the library will automatically 
use the ISA extensions specified via the command line.

Depending on which ISA extensions are enabled, specializations of the 
`avel::Vector` and `avel::Vector_mask` classes will be provided. Operations 
on these vectors will be implemented in a way that leverages the SIMD 
capabilities of the target machine. 

Since these vector classes are meant to represent vector registers, they should 
ideally be used in a manner that is different from most other types:
* It is discouraged to use these types for representing data.
* It is encouraged to use these to manipulate data.
* Vector's contents should be explicitly loaded and stored
* Using too many objects of these types may cause register spillover and cause
  the compiler to emit undesired stores and loads
* It's discouraged to use these in interfaces that may be called from 
  external code. These classes should be a hidden implementation detail

The library is built using SIMD intrinsics which unfortunately have poor 
performance without optimization but compile to near optimal code even at low 
optimization levels.
* Consider using the `-O1` or `-Og` optimization flags in debug builds
* The resulting assembly code is actually more readable with these optimizations 
  enabled so if this is a relevant factor, their use is highly encouraged

It's encouraged to take a look at the underlying implementations to garner 
intuition behind the cost of various functions. 

### SIMD Vector Aliases
Different sets of specializations of `avel::Vector` will be available depending 
on which feature macros were defined. Although the direct use of 
`avel::Vector` and `avel::Vector_mask` is possible, it is not recommended. 
Instead, AVEL defines aliases for vectors containing a specific quantity of 
elements of a particular type. For example, if you've enabled `AVEL_SSE2` then 
AVEL will offer the following aliases:

| Native Width | 128 bits wide |
|--------------|---------------|
| `vec1x8u`    | `vec16x8u`    |
| `vec1x8i`    | `vec16x8i`    |
| `vec1x16u`   | `vec8x16u`    |
| `vec1x16i`   | `vec8x16i`    |
| `vec1x32u`   | `vec4x32u`    |
| `vec1x32i`   | `vec4x32i`    |
| `vec1x64u`   | `vec2x64u`    |
| `vec1x64i`   | `vec2x64i`    |

Those in the left column are always offered by AVEL and correspond to 
processing data in a vector fashion. Those in the right colum correspond to 
populating an 128-bit vector with as many elements of the specified type as 
possible.

AVEL also defines aliases that don't feature a specific length but instead have
`N` or `M`.

| Natural width | Max width  |
|---------------|------------|
| `vecNx8u`     | `vecMx8u`  |
| `vecNx8i`     | `vecMx8i`  |
| `vecNx16u`    | `vecMx16u` |
| `vecNx16i`    | `vecMx16i` |
| `vecNx32u`    | `vecMx32u` |
| `vecNx32i`    | `vecMx32i` |
| `vecNx64u`    | `vecMx64u` |
| `vecNx64i`    | `vecMx64i` |

`N` indicates the natural width of a vector given the specified CPU features. 
These vectors will offer AVEL's full feature set. `M` indicates the maximum 
width of a vector given the specified CPU features. Only loading, storing, and 
conversions are guaranteed to be supported for max-width vectors (note that 
only signed/unsigned and identity conversions are currently implemented).

Corresponding aliases for `avel::Vector_mask` and `std::array` are also provided
with the `vec` prefix replaced with the `mask` and `arr` prefixes respectively.

## Performance Warnings
If the target machine architecture doesn't support a particular operation via a
dedicated instruction, it will be emulated using an instruction sequence 
instead., This may lead to certain operations being disproportionately 
expensive compared to the equivalent scalar operation, or the same operations 
performed if using a more modern instruction set.

This means that your intuitions for the cost of various operations may not 
translate from scalar code. It is encouraged that you read through the 
underlying implementations to get a better idea of the cost of the various 
functions. 

Overall, most operations, even when emulated, will exhibit better throughputs
than their scalar counterparts. While some emulations will have a non-trivial 
cost, they will still generally perform better than scalar code due to the sheer
throughput of vectorization. A select few implementations may perform worse than
scalar code depending on use, however, unless these operations comprise a 
significant portion of the data transforms, the performance benefits from other
operations will still yield a net performance gain.

## Falling Back to Intrinsics
AVEL relies on intrinsics, but it's fundamentally recognized that AVEL will not 
always be able to offer the best way to perform a particular operation. Hence, 
AVEL makes it easy to fall back to using platform-specific intrinsics.

Every vector class has a member type alias `primitive` which corresponds to 
the native type used for that vector by the underlying intrinsics. Vectors 
may be constructed from, assigned from, and converted to this type. For 
convenience, `avel::decay` may be called on a vector to get a copy of the 
underlying `primitive` object contained within the class.

The hope is that this will make it possible for users of the library to 
write the majority of their code in a convenient way, and only fall back to 
intrinsics when they need to take advantage of more specialized instructions.

## Documentation
The library's documentation consists of simple Markdown files provided in the 
docs directory of this repo. Since most of the functionality that the library 
provides is simply a vectorized version of features provided by the C++ core 
language and the C++'s standard library, it should be straightforward for 
any C++ developer to infer how to utilize most of the library.

The documentation for the standard library should suffice in most cases with the
understanding this library will simply perform the specified operation for as 
many lanes as there are the vector being processed.

Most operations on `avel::Vector<T, x>` should have the same semantics as the 
corresponding operations on `T` with some notable exceptions:
* Explicit conversions must be made to convert a `avel::Vector<T, N>` 
  object to a `avel::Vector<U, N>` object even if there is an implicit 
  conversion from `T` to `U` (Note that only signed/unsigned and identity 
  conversions are currently implemented).
* Signed integer overflow and underflow in the case of addition, subtraction,
  and multiplication is well-defined to exhibit the behavior expected of 
  integers with a twos-complement representation.
* Shifting integers by amounts equal to the number of bits they contain is 
  well-defined to continue to shift in zeros or ones. The behavior of shifting 
  by larger amounts or by negative amounts is unspecified for performance 
  reasons.
* Comparisons on vectors perform element-wise comparisons and hence return 
  vector mask objects. The functions `avel::all`, `avel::any`, `avel::count`,
  and `avel::none` may be used on mask object to produce a scalar boolean value.
* Conversions to/from bool are replaced with equivalent conversions to/from 
  vector masks

## Usage Examples

### Populating Vectors
```c++
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
    vec4x32u v1{arr1};

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
    //Handle complete vectors
    for (std::size_t i = 0; i < values.size(); i += vec4x32u::width) {
        auto values = avel::load<vec4x32u>(values.data() + i);
        auto clamped_values = avel::clamp(values, 0, 10);
        avel::store(clamped_values, ptr);
    }

    //Handle remaining elements
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
    
    //Use vec as you otherwise would
    
    return 0;
}


```

## Compiler Support
The library's tests have been run and passed on:
* x86
  * GCC 9.5.0, 10.4.0, 11.3.0, and 12.1.0
  * Clang 11.1.0-6,  12.0.1, 13.0.1, 14.0.0, 15.0.7
  * ICX 2023.2.0
* ARM
  * GCC 9.3.0-22, 10.2.1-6
* Clang 9.0.10-16.1, 11.0.1-2, 13.0.1-6
* MSVC 19.37.32822

Presumably newer/intermediate versions of these compilers will also work 
however it has not been tested which are the oldest versions on which this 
library will function
