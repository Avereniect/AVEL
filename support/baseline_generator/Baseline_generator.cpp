#include <cstdlib>

#include <vector>
#include <thread>
#include <queue>

#define __USE_FILE_OFFSET64

#include <avel/Misc.hpp>

#include <sys/mman.h>
#include <linux/mman.h>
#include <fcntl.h>
#include <string.h>

#include <boost/multiprecision/cpp_bin_float.hpp>

using hp_float = boost::multiprecision::cpp_bin_float_50;

const std::string output_path = "/mnt/hard_disk/Technology/Baselines/f32/recip_sqrt.bin";
const std::uint64_t range_start =  0x00000000ull;
const std::uint64_t range_end   = 0x100000000ull;
const std::uint64_t file_size = (range_end - range_start) * sizeof(float);
const std::uint64_t thread_count = 8;


std::uint32_t* output_array = nullptr;

hp_float f(hp_float x) {
    return hp_float{1.0f} / boost::multiprecision::sqrt(x);
}

void thread_worker(std::uint32_t id, std::uint64_t begin, std::uint64_t end) {
    std::stringstream initial_report;
    initial_report << "Thread " << id << " working on 0x" << std::hex << begin << " to 0x" << std::hex << end << std::endl;
    std::cout << initial_report.str();

    auto report_threshold = (end - begin) / 100;

    for (std::uint64_t i = begin; i < end; ++i) {
        auto x_bits = static_cast<std::uint32_t>(i);
        auto lp_x = avel::bit_cast<float>(x_bits);
        hp_float x{lp_x};

        hp_float f_of_x = f(x);
        auto lp_f_of_x = static_cast<float>(f_of_x);
        auto f_of_x_bits = avel::bit_cast<std::uint32_t>(lp_f_of_x);

        output_array[i - range_start] = f_of_x_bits;

        if ((i - begin) % report_threshold == 0) {
            std::string output = "Thread ";
            output += std::to_string(id);
            output += " at ";
            output += std::to_string(double(i - begin) / double(end - begin));
            output += "\n";
            std::cout << output;
            std::cout.flush();
        }
    }

    std::string output = "Thread ";
    output += std::to_string(id);
    output += " completed\n";
    std::cout << output;
    std::cout.flush();
}

void generate_baselines() {
    //Acquire file descriptor
    int fd = open(output_path.c_str(), O_CREAT | O_RDWR, S_IRWXU | S_IWUSR);
    if (fd == -1) {
        std::cerr << "Failed to open file descriptor. Error: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    //Resize file to appropriate capacity
    if (ftruncate(fd, file_size)) {
        std::cerr << "Failed to resize file. Error: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    //Map file
    output_array = reinterpret_cast<std::uint32_t*>(
        mmap(
            nullptr,
            file_size,
            PROT_WRITE,
            MAP_SHARED | MAP_HUGE_1GB,
            fd,
            sizeof(std::uint32_t) * 0x00 //Mapping offset
        )
    );

    if (output_array == MAP_FAILED) {
        std::cerr << "Failed to map file contents. Error: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    // Dispatch threads
    std::uint64_t work_per_thread = (range_end - range_start) / thread_count;
    std::uint64_t remainder_work = (range_end - range_start) % thread_count;

    std::vector<std::thread> threads;

    std::uint64_t curr_begin = 0;
    std::uint64_t curr_end = 0;
    for (std::uint32_t i = 0; i < thread_count; ++i) {
        curr_begin = curr_end;
        curr_end += work_per_thread + (i < remainder_work);
        threads.emplace_back(thread_worker, i, curr_begin, curr_end);
    }

    // Wait on threads
    for (auto& t : threads) {
        t.join();
    }

    msync(output_array, file_size, MS_SYNC);
    munmap(output_array, file_size);
    close(fd);
}

int main() {
    generate_baselines();

    return EXIT_SUCCESS;
}
