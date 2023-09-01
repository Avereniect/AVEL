#ifndef AVEL_AVEL_BENCHMARKS_HPP
#define AVEL_AVEL_BENCHMARKS_HPP

#include <benchmark/benchmark.h>

#include <random>
#include <chrono>
#include <climits>

namespace avel::benchmarks {

    constexpr std::size_t available_cache_size = 1024 * 24;

    auto random32u = std::minstd_rand{0xDEADBEEF};
    auto random64u = std::linear_congruential_engine<
        std::uint64_t,
        2862933555777941757ull,
        3037000493ull,
        9223372036854775807ull
    >{0xDEADBEEFDEADBEEFull};



    template<class T>
    T random_value();


    template<>
    bool random_value<bool>() {
        return random32u() & 0x1;
    }


    template<>
    std::uint8_t random_value<std::uint8_t>() {
        return static_cast<std::uint8_t>(random32u());
    }

    template<>
    std::uint16_t random_value<std::uint16_t>() {
        return static_cast<std::uint16_t>(random32u());
    }

    template<>
    std::uint32_t random_value<std::uint32_t>() {
        return static_cast<std::uint32_t>(random32u());
    }

    template<>
    std::uint64_t random_value<std::uint64_t>() {
        return static_cast<std::uint64_t>(random64u());
    }



    template<>
    std::int8_t random_value<std::int8_t>() {
        return static_cast<std::int8_t>(random32u());
    }

    template<>
    std::int16_t random_value<std::int16_t>() {
        return static_cast<std::int16_t>(random32u());
    }

    template<>
    std::int32_t random_value<std::int32_t>() {
        return static_cast<std::int32_t>(random32u());
    }

    template<>
    std::int64_t random_value<std::int64_t>() {
        return static_cast<std::int64_t>(random64u());
    }


    template<>
    float random_value<float>() {
        return avel::bit_cast<float>(random_value<std::uint32_t>());
    }

    template<>
    double random_value<double>() {
        return avel::bit_cast<double>(random_value<std::uint64_t>());
    }



    template<class I>
    inline I random_shift() {
        constexpr auto bound = sizeof(I) * CHAR_BIT + 1;
        return random_value<I>() % bound;
    }



    template<class T>
    std::vector<T> random_values(std::size_t n) {
        std::vector<T> ret;
        ret.resize(n);

        for (auto& x : ret) {
            x = random_value<T>();
        }

        return ret;
    }

    template<class T>
    std::vector<T> random_denominators(std::size_t n) {
        std::vector<T> ret;
        ret.resize(n);

        for (auto& x : ret) {
            x = random_value<T>();
            if (x == 0) {
                x = 1;
            }
        }

        return ret;
    }

    template<class T>
    inline std::vector<T> random_shift_array() {
        std::vector<T> ret{};

        for (int i = 0; i < ret.size(); ++i) {
            ret[i] = random_shift<T>();
        }

        return ret;
    }

    //=====================================================
    // Testbench templates
    //=====================================================

    template<class T, T(*Func)(bool)>
    void scalar_mask_bool_test_bench(benchmark::State& state) {
        typename std::enable_if_t<std::is_fundamental_v<T>, T> dummy;

        auto inputs = random_values<std::int8_t>(available_cache_size / sizeof(T));

        for (auto _ : state) {
            for (std::size_t i = 0; i < inputs.size(); ++i) {
                auto tmp = Func(inputs[i] < 0);
                benchmark::DoNotOptimize(tmp);
            }
        }
    }

    template<class T, class R, R(*Func)(T)>
    void scalar_test_bench(benchmark::State& state) {
        typename std::enable_if_t<std::is_fundamental_v<T>, T> dummy;

        auto inputs = random_values<T>(available_cache_size / sizeof(T));

        for (auto _ : state) {
            for (std::size_t i = 0; i < inputs.size(); ++i) {
                auto tmp = Func(inputs[i]);
                benchmark::DoNotOptimize(tmp);
            }
        }
    }

    template<class T, class U, T(*Func)(T, U)>
    void scalar_test_bench(benchmark::State& state) {
        typename std::enable_if_t<std::is_fundamental_v<T>, T> dummy;

        auto inputs0 = random_values<T>(available_cache_size / sizeof(T) / 2);
        auto inputs1 = random_values<U>(available_cache_size / sizeof(U) / 2);

        for (auto _ : state) {
            for (std::size_t i = 0; i < inputs0.size(); ++i) {
                auto tmp = Func(inputs0[i], inputs1[i]);
                benchmark::DoNotOptimize(tmp);
            }
        }
    }

    template<class T, T(*Func)(T)>
    void scalar_test_bench(benchmark::State& state) {
        scalar_test_bench<T, T, Func>(state);
    }

    template<class T, T(*Func)(T, T)>
    void scalar_test_bench(benchmark::State& state) {
        typename std::enable_if_t<std::is_fundamental_v<T>, T> dummy;

        using scalar = T;
        auto inputs0 = random_values<scalar>(available_cache_size / sizeof(scalar) / 2);
        auto inputs1 = random_values<scalar>(available_cache_size / sizeof(scalar) / 2);

        for (auto _ : state) {
            for (std::size_t i = 0; i < inputs0.size(); ++i) {
                auto tmp = Func(inputs0[i], inputs1[i]);
                benchmark::DoNotOptimize(tmp);
            }
        }
    }

    template<class T, T(*Func)(T, T)>
    void scalar_division_test_bench(benchmark::State& state) {
        typename std::enable_if_t<std::is_fundamental_v<T>, T> dummy;

        using scalar = T;
        auto inputs0 = random_values<scalar>(available_cache_size / sizeof(scalar) / 2);
        auto inputs1 = random_denominators<scalar>(available_cache_size / sizeof(scalar) / 2);

        for (auto _ : state) {
            for (std::size_t i = 0; i < inputs0.size(); ++i) {
                auto tmp = Func(inputs0[i], inputs1[i]);
                benchmark::DoNotOptimize(tmp);
            }
        }
    }



    template<class Vec_mask, Vec_mask(*Func)(bool)>
    void vector_mask_bool_test_bench(benchmark::State& state) {
        typename std::enable_if_t<avel::is_vector_mask_v<Vec_mask>, Vec_mask> dummy;

        using Vec = typename avel::to_vector<Vec_mask>::type;

        auto inputs = random_values<std::int8_t>(available_cache_size / sizeof(Vec));

        for (auto _ : state) {
            for (std::size_t i = 0; i < inputs.size(); ++i) {
                auto tmp = Func(inputs[i] < 0);
                benchmark::DoNotOptimize(tmp);
            }
        }
    }

    template<class Vector, class Ret, Ret(*Func)(Vector)>
    void vector_test_bench(benchmark::State& state) {
        typename std::enable_if_t<avel::is_vector_v<Vector>, Vector> dummy;

        using scalar = typename Vector::scalar;
        auto inputs0 = random_values<scalar>(available_cache_size / sizeof(scalar));

        for (auto _ : state) {
            for (std::size_t i = 0; i < inputs0.size(); i += Vector::width) {
                Ret tmp = Func(avel::load<Vector>(inputs0.data() + i));
                benchmark::DoNotOptimize(tmp);
            }
        }
    }

    template<class Vector0, class Vector1, Vector0(*Func)(Vector0, Vector1)>
    void vector_test_bench(benchmark::State& state) {
        typename std::enable_if_t<avel::is_vector_v<Vector0>, Vector0> dummy0;
        typename std::enable_if_t<avel::is_vector_v<Vector1>, Vector1> dummy1;

        using scalar0 = typename Vector0::scalar;
        using scalar1 = typename Vector1::scalar;

        auto inputs0 = random_values<scalar0>(available_cache_size / sizeof(scalar0) / 2);
        auto inputs1 = random_values<scalar1>(available_cache_size / sizeof(scalar1) / 2);

        for (auto _ : state) {
            for (std::size_t i = 0; i < inputs0.size(); i += Vector0::width) {
                auto values0 = avel::load<Vector0>(inputs0.data() + i);
                auto values1 = avel::load<Vector1>(inputs1.data() + i);
                auto tmp = Func(values0, values1);
                benchmark::DoNotOptimize(tmp);
            }
        }
    }

    template<class Vector, Vector(*Func)(Vector)>
    void vector_test_bench(benchmark::State& state) {
        vector_test_bench<Vector, Vector, Func>(state);
    }

    template<class Vector, Vector(*Func)(Vector, Vector)>
    void vector_test_bench(benchmark::State& state) {
        typename std::enable_if_t<avel::is_vector_v<Vector>, Vector> dummy;

        using scalar = typename Vector::scalar;
        auto inputs0 = random_values<scalar>(available_cache_size / sizeof(scalar) / 2);
        auto inputs1 = random_values<scalar>(available_cache_size / sizeof(scalar) / 2);

        for (auto _ : state) {
            for (std::size_t i = 0; i < inputs0.size(); i += Vector::width) {
                auto values0 = avel::load<Vector>(inputs0.data() + i);
                auto values1 = avel::load<Vector>(inputs1.data() + i);
                auto tmp = Func(values0, values1);
                benchmark::DoNotOptimize(tmp);
            }
        }
    }

    template<class Vector, Vector(*Func)(Vector, Vector)>
    void vector_division_test_bench(benchmark::State& state) {
        typename std::enable_if_t<avel::is_vector_v<Vector>, Vector> dummy;

        using scalar = typename Vector::scalar;
        auto inputs0 = random_values<scalar>(available_cache_size / sizeof(scalar) / 2);
        auto inputs1 = random_denominators<scalar>(available_cache_size / sizeof(scalar) / 2);

        for (auto _ : state) {
            for (std::size_t i = 0; i < inputs0.size(); i += Vector::width) {
                auto values0 = avel::load<Vector>(inputs0.data() + i);
                auto values1 = avel::load<Vector>(inputs1.data() + i);
                auto ret = Func(values0, values1);
                benchmark::DoNotOptimize(ret);
            }
        }
    }

}

#endif //AVEL_AVEL_BENCHMARKS_HPP
