//
// Created by avereniect on 2/26/22.
//

#ifndef AVEL_IMPL_CACHE_HPP
#define AVEL_IMPL_CACHE_HPP

#include "../Capabilities.hpp"
#include "../Misc.hpp"

namespace avel {

    AVEL_FINL void prefetch(void* ptr, Cache_level level = Cache_level::NULL_CACHE_LEVEL) {
        #if defined(AVEL_PREFETCH)
        _mm_prefetch(ptr, _mm_hint(int(level)));
        #endif
    }

    void prefetch(const void* ptr, std::size_t n, unsigned level) {

    }

    template<class T>
    void prefetch(const T* ptr, unsigned level) {
        //TODO: Eliminate need for branch
        if (alignof(T) > sizeof(T)) {
            //Object cannot cross a cache line
            prefetch(reinterpret_cast<const void*>(ptr), level);
        } else {
            prefetch(reinterpret_cast<const void*>(ptr), sizeof(T), level);
        }
    }

    template<class T>
    void prefetch(const T* ptr, std::size_t n, unsigned level) {
        prefetch(reinterpret_cast<const void*>(ptr), n * sizeof(T), level);
    }

    void flush(const void* ptr) {
        #if defined(AVEL_CLFLUSHOPT)
        _mm_clflushopt(const_cast<void*>(ptr));
        #elif defined(AVEL_CLFLUSH)
        _mm_clflush(const_cast<void*>(ptr));
        #endif
    }

    template<class T>
    void flush(const T* ptr, std::size_t n) {
        flush((const void*)ptr, n * sizeof(T));
    }

    template<>
    void flush<void>(const void* ptr, std::size_t n) {
        const char* p = reinterpret_cast<const char*>(ptr);

        auto m = (n / AVEL_CACHE_SIZE) * AVEL_CACHE_SIZE;

        for (std::size_t i = 0 ; i < m; i += AVEL_CACHE_SIZE) {
            flush(p + i);
        }
    }

}

#endif //AVEL_IMPL_CACHE_HPP
