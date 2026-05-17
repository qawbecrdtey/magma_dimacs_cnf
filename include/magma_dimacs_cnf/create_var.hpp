#ifndef MAGMA_DIMACS_CNF_CREATE_VAR_HPP
#define MAGMA_DIMACS_CNF_CREATE_VAR_HPP

#include <cstdint>

#define CREATE_VAR(n_)                                                                             \
    auto const var = [n = n_](std::int32_t const i_, std::int32_t const j_, std::int32_t const k_) \
      -> std::int32_t {                                                                            \
        ASSERT_AND_ASSUME(i_ < n);                                                                 \
        ASSERT_AND_ASSUME(j_ < n);                                                                 \
        ASSERT_AND_ASSUME(k_ < n);                                                                 \
        return 1 + (i_ * n + j_) * n + k_;                                                         \
    }

#endif
