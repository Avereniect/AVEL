//
// Created by avereniect on 2/26/22.
//

#ifndef AVEL_IMPL_CACHE_HPP
#define AVEL_IMPL_CACHE_HPP

#include "../Capabilities.hpp"
#include "../Utils.hpp"

namespace avel {

    AVEL_FINL void prefetch(void* ptr, unsigned level) {
        _MM_HINT_T0,
        _MM_HINT_T1,
        _MM_HINT_T2,
        _MM_HINT_NTA;

        #if defined(AVEL_PREFETCH)
        _mm_prefetch(ptr, 0); //TODO: Use Cache level
        #endif
    }

    template<class T>
    void prefetch(const T* ptr, unsigned level) {
        //Optimizer should make this equivalent to constexpr
        if (alignof(T) > sizeof(T)) {
            //Object cannot cross a cache line
            prefetch(reinterpret_cast<const void*>(ptr), level);
        } else {
            prefetch<(reinterpret_cast<const void*>(ptr), sizeof(T), level);
        }
    }

    template<class T>
    void prefetch(const T* ptr, std::size_t n, unsigned level) {
        prefetch(reinterpret_cast<const void*>(ptr), n * sizeof(T), level);
    }

    template<>
    void prefetch<void>(const void* ptr, std::size_t n, unsigned level) {

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
