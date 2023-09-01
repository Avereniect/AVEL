#include <cstdint>
#include <fstream>
#include <array>
#include <iostream>
#include <cstring>
#include <cmath>
#include <cstdio>
#include <thread>
#include <csignal>
#include <mutex>
#include <queue>

//#define AVEL_AVX512VL
#define AVEL_AVX512F
#include <avel/Vector.hpp>

#include <sys/mman.h>
#include <linux/mman.h>

#include <boost/multiprecision/cpp_bin_float.hpp>
#include <fcntl.h>

void reference_generation() {
    using namespace boost::multiprecision;
    using large_float = cpp_bin_float_50;

    std::uint32_t bit_pattern = 0x00000000;
    const std::uint32_t max_pattern = 0x40490fdb;
    const std::uint32_t stride = 1;

    std::ofstream fout{"/mnt/hard_disk/Technology/sin.dat"};

    for (; bit_pattern < max_pattern; bit_pattern += stride) {
        float x;
        std::memcpy(&x, &bit_pattern, sizeof(std::uint32_t));

        large_float y = x;
        y = sin(y);

        float output = static_cast<float>(y);
        fout.write((char*)&output, sizeof(float));
    }
}

using int_vector = avel::vec16x32i;
using uint_vector = avel::vec16x32u;
using float_vector = avel::vec16x32f;

const std::string baselines_file_path{"/mnt/hard_disk/Technology/sin.dat"};

std::vector<std::array<std::int32_t, 2>> results;

float sine(float x) {
    static const std::uint32_t parameters[] = {
        0x32d2a000,
        0x3638ef1d,
        0x39500ccb,
        0x3c088887,
        0x3e2aaaab,
        0x3f800000
    };

    const float u = x * x;

    float ret = avel::bit_cast<float>(parameters[0]);
    for (unsigned i = 1; i < std::extent_v<decltype(parameters)>; ++i) {
        ret = fmaf(-u, ret, avel::bit_cast<float>(parameters[i]));
    }

    return x * ret;
}

float_vector sine(float_vector x) {
    static std::uint32_t parameters[] = {
        0x32d2b000,
        0x3638ef1e,
        0x39500ccb,
        0x3c088887,
        0x3e2aaaab,
        0x3f800000
    };

    const float_vector u = x * x;

    float_vector ret{avel::bit_cast<float>(parameters[0])};
    for (unsigned i = 1; i < std::extent_v<decltype(parameters)>; ++i) {
        ret = avel::fmaf(-u, ret, float_vector{avel::bit_cast<float>(parameters[i])});
    }

    return x * ret;
}

void comparison(std::int32_t lo_pattern, std::int32_t hi_pattern, unsigned thread_index = -1) {
    static const std::array<std::int32_t, 16> offset_values {
        0x00, 0x01, 0x02, 0x03,
        0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B,
        0x0C, 0x0D, 0x0E, 0x0F
    };

    static const int_vector offsets{offset_values};

    std::FILE* fin = fopen(baselines_file_path.c_str(), "r");
    if (!fin) {
        std::cout << "Failed to open baselines file." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::int32_t max_error = 0x00;
    std::int32_t max_error_instances = 0x00;

    int_vector max_errors{0x00};
    int_vector max_errors_instances{0x00};

    std::int32_t bit_pattern = lo_pattern;
    while (bit_pattern < hi_pattern) {
        std::array<std::int32_t, 4 * 1024> baselines{};

        std::size_t read_size = std::min(std::size_t(hi_pattern - bit_pattern), baselines.size());

        fseek(fin, bit_pattern * sizeof(std::int32_t), SEEK_SET);
        std::int32_t num_read = fread(baselines.data(), sizeof(std::int32_t), read_size, fin);

        const auto vectorized_elements = (num_read / float_vector::width) * float_vector::width;

        std::int32_t i = 0;
        for (; i < vectorized_elements; i += float_vector::width) {
            int_vector bit_patterns = int_vector{bit_pattern} + offsets;

            auto sine_vector = avel::sin(avel::bit_cast<float_vector>(bit_patterns));

            auto sine_bit_patterns = avel::bit_cast<int_vector>(sine_vector);

            auto baseline_patterns = int_vector{(baselines.data() + i)};
            auto errors = avel::abs(baseline_patterns - sine_bit_patterns);

            max_errors = avel::max(max_errors, errors);

            auto mask = (max_errors < errors);
            max_errors_instances = avel::blend(max_errors_instances + int_vector{0x01}, int_vector{0x01}, mask);

            bit_pattern += float_vector::width;
        }

        for (; i < num_read; ++i) {
            std::int32_t baseline = baselines[i];

            auto x = avel::bit_cast<float>(bit_pattern);

            auto s = avel::sin(avel::vec16x32f{x});

            std::int32_t estimate_bits = avel::bit_cast<std::int32_t>(s.as_array()[0]);

            std::int32_t error = std::abs(estimate_bits - baseline);

            max_error = std::max(max_error, error);
            if (max_error < error) {
                max_error_instances = 1;
            } else {
                max_error_instances += 1;
            }
            bit_pattern += 1;
        }
    }

    auto max_error_array = max_errors.as_array();
    auto max_error_instances_array = max_errors.as_array();
    for (int i = 0; i < int_vector::width; ++i) {
        max_error = std::max(max_error_array[i], max_error);

        if (max_error == max_error_array[i]) {
            max_error_instances += max_error_instances_array[i];
        }

        if (max_error < max_error_array[i]) {
            max_error_instances = max_error_instances_array[i];
        }

        if (max_error > max_error_array[i]) {
            max_error_instances = max_error_instances;
        }
    }

    if (thread_index == -1) {
        std::cout << "Max error: " << max_error << std::endl;
        std::cout << "Instances: " << max_error_instances << std::endl;
        std::cout << "Bit patterns searched: " << (hi_pattern - lo_pattern) << std::endl;
    } else {
        results[thread_index][0] = max_error;
        results[thread_index][1] = max_error_instances;
    }
}

void parallel_comparison(std::int32_t first, std::int32_t last) {
    std::int32_t num_threads = std::thread::hardware_concurrency();
    results.resize(num_threads);

    std::int32_t elements_per_thread = ((last - first) / num_threads / float_vector::width) * float_vector::width;

    //Create ranges for threads to operate on
    std::vector<std::array<std::int32_t, 2>> ranges{};
    for (std::int32_t i = 0; (i + 1) < num_threads; ++i) {
        std::array<std::int32_t, 2> tmp{first + i * elements_per_thread, first + (i + 1) * elements_per_thread};
        ranges.emplace_back(tmp);
    }
    std::array<std::int32_t, 2> tmp{first + (num_threads - 1) * elements_per_thread, first + last};
    ranges.emplace_back(tmp);


    std::vector<std::thread> threads;
    for (std::size_t i = 0; i < num_threads; ++i) {
        threads.emplace_back(comparison, ranges[i][0], ranges[i][1], i);
    }

    for (auto& th : threads) {
        th.join();
    }

    auto& r = *std::max_element(results.begin(), results.end());
    std::int32_t max_error = r[0];
    std::int32_t max_error_instances = r[1];

    std::cout << "Max error: " << max_error << std::endl;
    std::cout << "Instances: " << max_error_instances << std::endl;
    std::cout << "Bit patterns searched: " << (last - first) << std::endl;
}

void vector_comparison(std::int32_t begin, std::int32_t end) {
    int fd = open(baselines_file_path.c_str(), O_RDONLY);
    if (fd == -1) {
        std::cerr << "Failed to open file descriptors. Errno: " << errno << std::endl;
        errno = 0;
        return;
    }

    auto* baselines = (std::int32_t*)mmap(
        nullptr,
        sizeof(std::uint32_t) * (end - begin),
        PROT_READ,
        MAP_PRIVATE | MAP_HUGE_256MB,
        fd,
        sizeof(std::uint32_t) * begin
    );

    if (baselines == MAP_FAILED) {
        std::cerr << "Failed to map file contents. Errno: " << errno << std::endl;
        errno = 0;
        return;
    }

    static const int_vector pattern_offset{{
        0x00, 0x01, 0x02, 0x03,
        0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b,
        0x0c, 0x0d, 0x0e, 0x0f
    }};

    int_vector cumulative_errors{0};
    int_vector max_errors{0};
    int_vector max_errors_instances{0};

    std::uint32_t i = begin;
    std::uint32_t vector_end = (end / float_vector::width) * float_vector::width;

    for (; i < vector_end; i += float_vector::width) {
        int_vector baseline{baselines + i - begin};

        auto bit_patterns = int_vector{std::int32_t(i)} + pattern_offset;
        auto x = avel::bit_cast<float_vector>(bit_patterns);

        float_vector s = sine(x);
        auto sine_bits = avel::bit_cast<int_vector>(s);

        auto errors = avel::abs(baseline - sine_bits);

        cumulative_errors += errors;

        max_errors_instances = avel::blend(max_errors_instances, int_vector{0x00}, max_errors < errors);
        max_errors = avel::max(max_errors, errors);
        max_errors_instances += int_vector(errors == max_errors);
    }

    std::uint32_t cumulative_error = 0;
    std::uint32_t max_error = 0;
    std::uint32_t max_error_instances = 0;

    for (; i < end; ++i) {
        std::int32_t baseline = baselines[i - begin];

        auto x = avel::bit_cast<float>(i);
        float s = sine(float_vector{x}).as_array()[0];
        auto sine_bits = avel::bit_cast<std::int32_t>(s);

        std::int32_t error = std::abs(baseline - sine_bits);

        cumulative_error += error;

        if (max_error < error) {
            max_error = error;
            max_error_instances = 0;
        }

        max_error_instances += (error == max_error);
    }

    auto cumulative_error_array = cumulative_errors.as_array();
    auto max_error_array = max_errors.as_array();
    auto max_error_instance_array = max_errors_instances.as_array();
    for (int j = 0; j < float_vector::width; ++j) {
        cumulative_error += cumulative_error_array[j];

        if (max_error < max_error_array[j]) {
            max_error = max_error_array[j];
            max_error_instances = 0;
        }

        max_error_instances += max_error_instance_array[j];
    }

    std::cout << "Cumulative error: " << cumulative_error << std::endl;
    std::cout << "Max error: " << max_error << std::endl;
    std::cout << "Instances: " << max_error_instances << std::endl;
    std::cout << "Proportion: " << std::fixed << std::setprecision(12) << double(max_error_instances) / double(end - begin) << std::endl;
    std::cout << "Bit patterns searched: " << (i - begin) << std::endl;

    std::cout << std::endl;

    munmap(baselines, sizeof(std::uint32_t) * (end - begin));
    close(fd);
}

void scalar_comparison(std::uint32_t begin, std::uint32_t end) {
    int fd = open(baselines_file_path.c_str(), O_RDONLY);
    if (fd == -1) {
        std::cerr << "Failed to open file descriptors. Errno: " << errno << std::endl;
        errno = 0;
        return;
    }

    auto* baselines = (std::int32_t*)mmap(
        nullptr,
        sizeof(std::uint32_t) * (end - begin),
        PROT_READ,
        MAP_PRIVATE | MAP_HUGE_256MB,
        fd,
        sizeof(std::uint32_t) * begin
    );
    if (baselines == MAP_FAILED) {
        std::cerr << "Failed to map file contents. Errno: " << errno << std::endl;
        errno = 0;
        return;
    }

    std::uint32_t cumulative_error = 0;
    std::uint32_t max_error = 0;
    std::uint32_t max_error_instances = 0;

    std::uint32_t i = begin;
    for (; i < end; ++i) {
        std::int32_t baseline = baselines[i - begin];

        auto x = avel::bit_cast<float>(i);
        float s = std::sin(x);
        auto sine_bits = avel::bit_cast<std::int32_t>(s);

        std::int32_t error = std::abs(baseline - sine_bits);

        cumulative_error += error;

        if (max_error < error) {
            max_error = error;
            max_error_instances = 0;
        }

        max_error_instances += (error == max_error);
    }

    std::cout << "Cumulative error: " << cumulative_error << std::endl;
    std::cout << "Max error: " << max_error << std::endl;
    std::cout << "Instances: " << max_error_instances << std::endl;
    std::cout << "Proportion: " << std::fixed << std::setprecision(12) << double(max_error_instances) / double(end - begin) << std::endl;
    std::cout << "Bit patterns searched: " << (i - begin) << std::endl;

    std::cout << std::endl;

    munmap(baselines, sizeof(std::uint32_t) * (end - begin));
    close(fd);
}

int main() {
    //parallel_comparison(0x00, 0x3fc90fdc);

    scalar_comparison(0x0000'0000, 0x3FC9'0FDC);
    //vector_comparison(0x0000'0000, 0x3FC9'0FDC);

    return EXIT_SUCCESS;
}
