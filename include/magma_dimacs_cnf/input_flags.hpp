#ifndef MAGMA_DIMACS_CNF_INPUT_FLAGS_HPP
#define MAGMA_DIMACS_CNF_INPUT_FLAGS_HPP

#include <cstdint>
#include <filesystem>

namespace magma_dimacs_cnf {
    struct input_flags {
        std::filesystem::path path;
        std::int32_t equation_number;
        std::int32_t element_count;
        std::int32_t thread_count;
    };
}  // namespace magma_dimacs_cnf

#endif
