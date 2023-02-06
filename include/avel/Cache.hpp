#ifndef AVEL_CACHE_HPP
#define AVEL_CACHE_HPP

#include "impl/Capabilities.hpp"
#include "impl/Constants.hpp"

#include "Misc.hpp"

namespace avel {

    enum Cache_level : std::uint8_t {
        L1_CACHE = 0,
        L2_CACHE = 1,
        L3_CACHE = 2
    };

    //=====================================================
    // Prefetching
    //=====================================================

    ///
    /// Prefetch data with the anticipation that the specified data will be read
    /// in the near future.
    ///
    /// May not do anything if the target hardware does not support the
    /// specified operation.
    ///
    /// \tparam level Enum indicating cache level to prefetch into
    /// \param ptr Pointer to location in cache line to prefetch
    /// \param n Number of bytes to prefetch
    template<Cache_level level = L1_CACHE>
    void prefetch_read(const void* ptr, std::size_t n = 1) {
        #if defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICX)

        constexpr static int READ = 0;
        constexpr auto increment = cache_line_sizes[static_cast<std::uint8_t>(level)];
        constexpr int locality_hint = 3 - static_cast<std::uint8_t>(level);

        for (std::size_t i = 0; i < n; i += increment) {
            __builtin_prefetch(reinterpret_cast<const char*>(ptr) + i, READ, locality_hint);
        }
        #elif

        #if defined(AVEL_SSE)
        static constexpr int table[3] = {
            static_cast<int>(_MM_HINT_T0),
            static_cast<int>(_MM_HINT_T1),
            static_cast<int>(_MM_HINT_T2)
        };

        constexpr auto increment = cache_line_sizes[static_cast<std::uint8_t>(level)];
        constexpr auto native_enum = static_cast<int>(level);

        for (std::size_t i = 0; i < n; i += increment) {
            _mm_prefetch(reinterpret_cast<const char*>(ptr) + i, native_enum);
        }
        #endif

        #if defined(AVEL_ARM)
        //TODO: Implement
        #endif
        #endif
    }

    ///
    /// Prefetch data with the anticipation that the specified data will be read
    /// in the near future
    ///
    /// May not do anything if the target hardware does not support the
    /// specified operation.
    ///
    /// \tparam level Enum indicating cache level to prefetch into
    /// \tparam T Type of object to prefetch
    /// \param ptr Pointer to location in cache line to prefetch
    /// \param n Number of objects to prefetch
    template<Cache_level level, class T>
    void prefetch_read(const T* ptr, std::size_t n = 1) {
        prefetch_read<level>(reinterpret_cast<const void*>(ptr), sizeof(T) * n);
    }

    ///
    /// Prefetch data with the anticipation that the specified data will be
    /// written in the near future
    ///
    /// May not do anything if the target hardware does not support the
    /// specified operation.
    ///
    /// \tparam level Enum indicating cache level to prefetch into
    /// \param ptr Pointer to location in cache line to prefetch
    /// \param n Number of bytes to prefetch
    template<Cache_level level = L1_CACHE>
    void prefetch_write(const void* ptr, std::size_t n = 1) {
        #if defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICX)

        constexpr static int WRITE = 1;
        constexpr auto increment = cache_line_sizes[static_cast<std::uint8_t>(level)];
        constexpr int locality_hint = 3 - static_cast<std::uint8_t>(level);

        for (std::size_t i = 0; i < n; i += increment) {
            __builtin_prefetch(reinterpret_cast<const char*>(ptr) + i, WRITE, locality_hint);
        }
        #elif

        #if defined(AVEL_SSE)
        //TODO: Implement using _m_prefetchw?
        #endif

        #if defined(AVEL_ARM)
        //TODO: Implement
        #endif
        #endif
    }

    ///
    /// Prefetch data with the anticipation that the specified data will be
    /// written in the near future
    ///
    /// May not do anything if the target hardware does not support the
    /// specified operation.
    ///
    /// \tparam level Enum indicating cache level to prefetch into
    /// \tparam T Type of object to prefetch
    /// \param ptr Pointer to location in cache line to prefetch
    /// \param n Number of objects to prefetch
    template<Cache_level level, class T>
    void prefetch_write(const T* ptr, std::size_t n = 1) {
        prefetch_write<level>(reinterpret_cast<const void*>(ptr), sizeof(T) * n);
    }

}

#endif //AVEL_CACHE_HPP
