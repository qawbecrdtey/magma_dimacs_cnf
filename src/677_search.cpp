#include "magma_dimacs_cnf/magma_solve.hpp"

#include <magma_dimacs_cnf/assert_and_assume.hpp>
#include <magma_dimacs_cnf/input_flags.hpp>

#include <cstdint>
#include <print>
#include <string_view>
#include <thread>

static constexpr std::int32_t lo = 5;
static constexpr std::int32_t hi = 70;

static constexpr void print_help(char const *const argv0_) {
    std::print(
      "usage: {} [/path/to/dimacs/output] [# of elements] [# of threads]\n"
      "  Number of elements shouldn't be smaller than {}.\n"
      "  Number of elements shouldn't be greater than {}.\n"
      "  Number of threads should be positive, and shouldn't be greater than {}",
      argv0_,
      lo,
      hi,
      std::thread::hardware_concurrency());
}

static constexpr bool
get_output_path(std::string_view const str_, magma_dimacs_cnf::input_flags &flags_) {
    flags_.path = std::filesystem::path {str_};
    if(!is_directory(flags_.path)) { return false; }
    if(!exists(flags_.path)) { return false; }
    flags_.path /= std::to_string(flags_.element_count);
    if(!exists(flags_.path)) { std::filesystem::create_directory(flags_.path); }
    if(!exists(flags_.path / "drat")) { std::filesystem::create_directory(flags_.path / "drat"); }
    return true;
}

static constexpr bool
get_element_count(std::string_view const str_, magma_dimacs_cnf::input_flags &flags_) {
    auto const [ptr, ec] =
      std::from_chars(str_.data(), str_.data() + str_.size(), flags_.element_count);
    if(ec != std::errc {}) { return false; }
    if(flags_.element_count < lo) { return false; }
    if(hi < flags_.element_count) { return false; }
    return true;
}

static constexpr bool
get_thread_count(std::string_view const str_, magma_dimacs_cnf::input_flags &flags_) {
    auto const [ptr, ec] =
      std::from_chars(str_.data(), str_.data() + str_.size(), flags_.thread_count);
    if(ec != std::errc {}) { return false; }
    if(flags_.thread_count < 1) { return false; }
    if(std::thread::hardware_concurrency() < static_cast<unsigned int>(flags_.thread_count)) {
        return false;
    }
    return true;
}

static constexpr bool
get_input_flags(int const argc_, char const *const *const argv_, magma_dimacs_cnf::input_flags &flags_) {
    if(argc_ != 4) { return false; }
    if(!get_element_count(argv_[2], flags_)) { return false; }
    if(!get_output_path(argv_[1], flags_)) { return false; }
    if(!get_thread_count(argv_[3], flags_)) { return false; }
    return true;
}

int main(int argc, char *argv[]) {
    ASSERT_AND_ASSUME(0 < argc);
    ASSERT_AND_ASSUME(!argv[argc]);

    magma_dimacs_cnf::input_flags flags {};

    if(!get_input_flags(argc, argv, flags)) {
        print_help(argv[0]);
        return 1;
    }

    magma_solve(flags);
}
