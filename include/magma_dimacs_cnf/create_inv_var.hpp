#ifndef MAGMA_DIMACS_CNF_CREATE_INV_VAR_HPP
#define MAGMA_DIMACS_CNF_CREATE_INV_VAR_HPP

/**
 * Creates a inverse variable function based on three indices.
 * `inv_var(i, j, k)` indicates an equality $i=jk$ in the original magma, or equivalently, $i*j=k$.
 * @param n_ Number of elements in the magma.
 */
#define CREATE_INV_VAR(n_)                                                                            \
    auto const inv_var =                                                                              \
      [n = n_](std::int32_t const i_, std::int32_t const j_, std::int32_t const k_) -> std::int32_t { \
        ASSERT_AND_ASSUME(i_ < n);                                                                    \
        ASSERT_AND_ASSUME(j_ < n);                                                                    \
        ASSERT_AND_ASSUME(k_ < n);                                                                    \
        return 1 + n * n * n + (i_ * n + j_) * n + k_;                                                \
    }

#endif
