//
// Created by avereniect on 2/26/22.
//

#ifndef AVEL_CACHE_HPP
#define AVEL_CACHE_HPP

#include "impl/Capabilities.hpp"
#include "impl/Utils.hpp"

namespace avel {

    ///
    /// Prefetches the cache line to which corresponds to the memory address
    /// specified by the pointer passed in
    ///
    /// \param ptr Arbitrary pointer
    /// \param level Cache level to prefetch into
    AVEL_FINL void prefetch(const void* ptr, unsigned level);

    ///
    /// Prefetches all the cache lines which correspond to the n bytes pointed
    /// to by the specified pointer
    ///
    /// \param ptr Arbitrary pointer
    /// \param n Number of bytes to prefetch
    /// \param level Cache level to prefetch into
    AVEL_FINL void prefetch(const void* ptr, std::size_t n, unsigned level);

}

namespace avel::impl {

    template<std::size_t Object_size, std::size_t Object_alignment, bool C = (Object_size > Object_alignment)>
    AVEL_FINL void prefetch_(const void* ptr, std::size_t n, unsigned level);

    template<std::size_t Object_size, std::size_t Object_alignment>
    AVEL_FINL void prefetch_crossing(const void* ptr, std::size_t n, unsigned level) {

    }

}

namespace avel {

    ///
    /// Prefetches all the cache lines which are occupied by the pointed to
    /// object
    ///
    /// \tparam T Object type
    /// \param ptr Pointer to object
    /// \param level Cache level to prefetch into
    template<class T>
    void prefetch(const T* ptr, unsigned level) {
        impl::prefetch<sizeof(T), alignof(T), sizeof(T) > alignof(T)>(ptr, level);
        //prefetch(reinterpret_cast<const void*>(ptr), sizeof(T), level);
    }

    ///
    /// Prefetches all the cache lines which are occupied by the pointed to
    /// objects
    ///
    /// \tparam T Object type
    /// \param ptr Pointer to object
    /// \param n Number of objects in array
    /// \param level Cache level to prefetch into
    template<class T>
    void prefetch(const T* ptr, std::size_t n, unsigned level) {
        //prefetch(reinterpret_cast<const void*>(ptr), sizeof(T), level);
    }

}

#include "impl/cache/Cache.hpp"

#endif //AVEL_CACHE_HPP
