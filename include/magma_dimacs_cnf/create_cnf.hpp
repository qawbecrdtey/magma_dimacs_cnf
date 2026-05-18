#ifndef MAGMA_DIMACS_CNF_CREATE_CNF_HPP
#define MAGMA_DIMACS_CNF_CREATE_CNF_HPP

#include <magma_dimacs_cnf/create_inv_var.hpp>
#include <magma_dimacs_cnf/create_var.hpp>
#include <magma_dimacs_cnf/input_flags.hpp>
#include <magma_dimacs_cnf/sum.hpp>

#include <cstdint>
#include <vector>

namespace magma_dimacs_cnf {
    static constexpr void create_cnf_magma(
      input_flags const *const flags_,
      std::vector<std::vector<std::int32_t>> &cnf_,
      [[maybe_unused]] std::int32_t &next_) {
        auto const n = flags_->element_count;
        ASSERT_AND_ASSUME(0 < n);
        CREATE_VAR(n);

        for(std::int32_t i = 0; i < n; i++) {  // x
            for(std::int32_t j = 0; j < n; j++) {  // y
                for(std::int32_t k = 0; k < n - 1; k++) {
                    for(std::int32_t l = k + 1; l < n; l++) {
                        cnf_.emplace_back(std::initializer_list {-var(i, j, k), -var(i, j, l)});
                    }
                }

                std::vector<std::int32_t> cl;
                cl.reserve(n);
                for(std::int32_t k = 0; k < n; k++) { cl.emplace_back(var(i, j, k)); }
                cnf_.emplace_back(std::move(cl));
            }
        }
    }

    static constexpr void create_cnf_inv_magma(
      input_flags const *const flags_,
      std::vector<std::vector<std::int32_t>> &cnf_,
      [[maybe_unused]] std::int32_t &next_) {
        auto const n = flags_->element_count;
        ASSERT_AND_ASSUME(0 < n);
        CREATE_INV_VAR(n);

        for(std::int32_t i = 0; i < n; i++) {  // x
            for(std::int32_t j = 0; j < n; j++) {  // y
                for(std::int32_t k = 0; k < n - 1; k++) {
                    for(std::int32_t l = k + 1; l < n; l++) {
                        cnf_.emplace_back(
                          std::initializer_list {-inv_var(i, j, k), -inv_var(i, j, l)});
                    }
                }

                std::vector<std::int32_t> cl;
                cl.reserve(n);
                for(std::int32_t k = 0; k < n; k++) { cl.emplace_back(inv_var(i, j, k)); }
                cnf_.emplace_back(std::move(cl));
            }
        }
    }

    static constexpr void create_cnf_677_magma(
      input_flags const *const flags_,
      std::vector<std::vector<std::int32_t>> &cnf_,
      [[maybe_unused]] std::int32_t &next_) {
        auto const n = flags_->element_count;
        ASSERT_AND_ASSUME(0 < n);
        CREATE_VAR(n);

        for(std::int32_t i = 0; i < n; i++) {  // x
            for(std::int32_t j = 0; j < n; j++) {  // y
                for(std::int32_t k = 0; k < n; k++) {  // yx
                    for(std::int32_t l = 0; l < n; l++) {  // (yx)y
                        for(std::int32_t m = 0; m < n; m++) {  // x((yx)y)
                            cnf_.emplace_back(
                              std::initializer_list {
                                -var(j, i, k), -var(k, j, l), -var(i, l, m), var(j, m, i)});
                        }
                    }
                }
            }
        }
    }

    static constexpr void create_cnf_677_inv_magma(
      input_flags const *const flags_,
      std::vector<std::vector<std::int32_t>> &cnf_,
      [[maybe_unused]] std::int32_t &next_) {
        auto const n = flags_->element_count;
        ASSERT_AND_ASSUME(0 < n);
        CREATE_INV_VAR(n);

        for(std::int32_t i = 0; i < n; i++) {  // x
            for(std::int32_t j = 0; j < n; j++) {  // y
                for(std::int32_t k = 0; k < n; k++) {  // y*x
                    for(std::int32_t l = 0; l < n; l++) {  // (y*x)*x
                        for(std::int32_t m = 0; m < n; m++) {  // ((y*x)*x)*(y*x)
                            cnf_.emplace_back(
                              std::initializer_list {
                                -inv_var(j, i, k),
                                -inv_var(k, i, l),
                                -inv_var(l, k, m),
                                inv_var(m, j, i)});
                        }
                    }
                }
            }
        }
    }

    static constexpr void create_cnf_677_equivalence(
      input_flags const *const flags_,
      std::vector<std::vector<std::int32_t>> &cnf_,
      [[maybe_unused]] std::int32_t &next_) {
        auto const n = flags_->element_count;
        ASSERT_AND_ASSUME(0 < n);
        CREATE_VAR(n);
        CREATE_INV_VAR(n);

        for(std::int32_t i = 0; i < n; i++) {  // x
            for(std::int32_t j = 0; j < n; j++) {  // y
                for(std::int32_t k = 0; k < n; k++) {  // z
                    cnf_.emplace_back(std::initializer_list {-var(i, j, k), inv_var(k, i, j)});
                    cnf_.emplace_back(std::initializer_list {var(i, j, k), -inv_var(k, i, j)});
                }
            }
        }

        // For every $x$ and $y$, $(yx)y=(x*y)*x$ holds.
        for(std::int32_t i = 0; i < n; i++) {  // x
            for(std::int32_t j = 0; j < n; j++) {  // y
                for(std::int32_t k = 0; k < n; k++) {  // yx
                    for(std::int32_t l = 0; l < n; l++) {  // (1) (yx)y  // (2) x*y
                        for(std::int32_t m = 0; m < n; m++) {  // (1) x*y  // (2) (x*y)*x
                            // (1)
                            cnf_.emplace_back(
                              std::initializer_list {
                                -var(j, i, k), -var(k, j, l), -inv_var(i, j, m), inv_var(m, i, l)});
                            // (2)
                            cnf_.emplace_back(
                              std::initializer_list {
                                -var(j, i, k), -inv_var(i, j, l), -inv_var(l, i, m), var(k, j, m)});
                        }
                    }
                }
            }
        }
    }

    static constexpr void create_cnf_677_general_properties(
      input_flags const *const flags_,
      std::vector<std::vector<std::int32_t>> &cnf_,
      [[maybe_unused]] std::int32_t &next_) {
        auto const n = flags_->element_count;
        ASSERT_AND_ASSUME(0 < n);
        CREATE_VAR(n);
        CREATE_INV_VAR(n);

        // For any $x = yx$, it holds that $y=(xx)x=(x*x)*x$.
        for(std::int32_t i = 0; i < n; i++) {  // x
            for(std::int32_t j = 0; j < n; j++) {  // y
                for(std::int32_t k = 0; k < n; k++) {  // (1) xx  // (2) x*x
                    // (1)
                    cnf_.emplace_back(
                      std::initializer_list {-var(j, i, i), -var(i, i, k), var(k, i, j)});
                    // (2)
                    cnf_.emplace_back(
                      std::initializer_list {-var(j, i, i), -inv_var(i, i, k), inv_var(k, i, j)});
                }
            }
        }

        // For any $x$, $(xx)x=(x((xx)x))(xx)=(x*x)(xx)$ holds.
        for(std::int32_t i = 0; i < n; i++) {  // x
            for(std::int32_t j = 0; j < n; j++) {  // xx
                for(std::int32_t k = 0; k < n; k++) {  //(xx)x
                    for(std::int32_t l = 0; l < n; l++) {  // (1) x((xx)x)  // (2) x*x
                        // (1)
                        cnf_.emplace_back(
                          std::initializer_list {
                            -var(i, i, j), -var(j, i, k), -var(i, k, l), var(l, j, k)});
                        // (2)
                        cnf_.emplace_back(
                          std::initializer_list {
                            -var(i, i, j), -var(j, i, k), -inv_var(i, i, l), var(l, j, k)});
                    }
                }
            }
        }

        // For any $x$, if $x\neq x*x$, then
        // (1) $x\neq (x*x)*x
        // (2) $x*x\neq (x*x)*x$
        // (3) $x\neq x*(x*x)$
        // (4) $x*x\neq x*(x*x)$
        for(std::int32_t i = 0; i < n; i++) {  // x
            for(std::int32_t j = 0; j < n; j++) {  // x*x
                if(i == j) { continue; }
                // (1)
                cnf_.emplace_back(std::initializer_list {-inv_var(i, i, j), -inv_var(j, i, i)});
                // (2)
                cnf_.emplace_back(std::initializer_list {-inv_var(i, i, j), -inv_var(j, i, j)});
                // (3)
                cnf_.emplace_back(std::initializer_list {-inv_var(i, i, j), -inv_var(i, j, i)});
                // (4)
                cnf_.emplace_back(std::initializer_list {-inv_var(i, i, j), -inv_var(i, j, j)});
            }
        }

        // For any distinct $x$ and $y$, $yx\neq xx$ or $y(x*x)\neq x$ holds.
        for(std::int32_t i = 0; i < n; i++) {  // x
            for(std::int32_t j = 0; j < n; j++) {  // y
                if(i == j) { continue; }
                for(std::int32_t k = 0; k < n; k++) {
                    for(std::int32_t l = 0; l < n; l++) {
                        cnf_.emplace_back(
                          std::initializer_list {
                            -var(i, i, k), -var(j, i, k), -var(i, l, i), -var(j, l, i)});
                        cnf_.emplace_back(
                          std::initializer_list {
                            -inv_var(k, i, i), -inv_var(k, j, i), -inv_var(i, i, l), -inv_var(i, j, l)});
                    }
                }
            }
        }
    }

    static constexpr void create_cnf_677_idempotent_properties(
      input_flags const *const flags_,
      std::vector<std::vector<std::int32_t>> &cnf_,
      [[maybe_unused]] std::int32_t &next_) {
        auto const n = flags_->element_count;
        ASSERT_AND_ASSUME(0 < n);
        CREATE_VAR(n);
        CREATE_INV_VAR(n);

        for(std::int32_t i = 0; i < n; i++) {  // x
            for(std::int32_t j = 0; j < n; j++) {  // xx
                // In this scope, we add `-var(i, i, j)` to represent that $x\neq xx$.
                if(i == j) { continue; }
                // For any $x$ which $x\neq xx$, $x\neq (xx)x$ holds.
                cnf_.emplace_back(std::initializer_list {-var(i, i, j), -var(j, i, i)});
                // For any $x$ which $x\neq xx$, $x\neq x(xx)$ holds.
                cnf_.emplace_back(std::initializer_list {-var(i, i, j), -var(i, j, i)});
                // For any $x$ which $x\neq xx$, $x\neq (xx)(xx)$ holds.
                cnf_.emplace_back(std::initializer_list {-var(i, i, j), -var(j, j, i)});
                // For any $x$ which $x\neq xx$, $xx\neq (xx)x$ holds.
                cnf_.emplace_back(std::initializer_list {-var(i, i, j), -var(j, i, j)});
                // For any $x$ which $x\neq xx$, $xx\neq x(xx)$ holds.
                cnf_.emplace_back(std::initializer_list {-var(i, i, j), -var(i, j, j)});

                // For any $x$ which $x\neq xx$, $x\neq x*x$ holds.
                cnf_.emplace_back(std::initializer_list {-var(i, i, j), -inv_var(i, i, i)});
                // For any $x$ which $x\neq xx$, $xx\neq x*x$ holds.
                cnf_.emplace_back(std::initializer_list {-var(i, i, j), -inv_var(i, i, j)});

                for(std::int32_t k = 0; k < n; k++) {  // (xx)x
                    // For any $x$ which $x\neq xx$, $x\neq x((xx)x)$ holds.
                    cnf_.emplace_back(
                      std::initializer_list {-var(i, i, j), -var(j, i, k), -var(i, k, i)});
                    // For any $x$ which $x\neq xx$, $xx\neq x((xx)x)$ holds.
                    cnf_.emplace_back(
                      std::initializer_list {-var(i, i, j), -var(j, i, k), -var(i, k, j)});
                    // For any $x$ which $x\neq xx$, $(xx)x\neq x((xx)x)$ holds.
                    cnf_.emplace_back(
                      std::initializer_list {-var(i, i, j), -var(j, i, k), -var(i, k, k)});
                    // For any $x$ which $x\neq xx$, $x\neq ((xx)x)(xx)$ holds.
                    cnf_.emplace_back(
                      std::initializer_list {-var(i, i, j), -var(j, i, k), -var(k, j, i)});
                    // For any $x$ which $x\neq xx$, $(xx)x\neq (xx)((xx)x)$ holds.
                    cnf_.emplace_back(
                      std::initializer_list {-var(i, i, j), -var(j, i, k), -var(j, k, k)});

                    // For any $x$ which $x\neq xx$, $(xx)x\neq x*x$ holds.
                    cnf_.emplace_back(
                      std::initializer_list {-var(i, i, j), -var(j, i, k), -inv_var(i, i, k)});
                }

                for(std::int32_t k = 0; k < n; k++) {  // x(xx)
                    // For any $x$ which $x\neq xx$, $x\neq x(x(xx))$ holds.
                    cnf_.emplace_back(
                      std::initializer_list {-var(i, i, j), -var(i, j, k), -var(i, k, i)});
                    // For any $x$ which $x\neq xx$, $x\neq (x(xx))x$ holds.
                    cnf_.emplace_back(
                      std::initializer_list {-var(i, i, j), -var(i, j, k), -var(k, i, i)});
                    // For any $x$ which $x\neq xx$, $xx\neq (x(xx))x$ holds.
                    cnf_.emplace_back(
                      std::initializer_list {-var(i, i, j), -var(i, j, k), -var(k, i, j)});
                }
            }
        }
    }

    static constexpr void create_cnf_677_properties(
      input_flags const *const flags_,
      std::vector<std::vector<std::int32_t>> &cnf_,
      [[maybe_unused]] std::int32_t &next_) {
        auto const n = flags_->element_count;
        ASSERT_AND_ASSUME(0 < n);
        // CREATE_VAR(n);
        // CREATE_INV_VAR(n);

        create_cnf_677_general_properties(flags_, cnf_, next_);
        create_cnf_677_idempotent_properties(flags_, cnf_, next_);
    }

    static constexpr void create_cnf_first_line_constraint(
      input_flags const *const flags_,
      std::vector<std::int32_t> const &v_,
      std::vector<std::vector<std::int32_t>> &cnf_) {
        auto const n = flags_->element_count;
        ASSERT_AND_ASSUME(sum(v_) == n);
        CREATE_VAR(n);

        std::int32_t begin = 0;
        // Consider the first row, which is of the form $0x=y$.
        // Each value in `v_` represents a length of each circuit.
        for(auto const l: v_) {
            ASSERT_AND_ASSUME(0 < l);
            std::int32_t const end = begin + l;
            for(std::int32_t i = begin; i < end; i++) {
                cnf_.emplace_back(std::initializer_list {var(0, i, (i - begin + 1) % l + begin)});
            }
            begin = end;
        }

        if(1 != v_[0]) {
            for(std::int32_t i = 1; i < n; i++) {
                cnf_.emplace_back(std::initializer_list {-var(i, i, i)});
                for(std::int32_t j = 0; j < n; j++) {
                    if(i == j) { continue; }
                    cnf_.emplace_back(std::initializer_list {-var(i, i, j), -var(i, j, i)});
                    for(std::int32_t k = 0; k < n; k++) {
                        if(i == k) { continue; }
                        if(j == k) { continue; }
                        cnf_.emplace_back(
                          std::initializer_list {-var(i, i, j), -var(i, j, k), -var(i, k, i)});
                    }
                }
            }
        }
    }

    static constexpr void create_cnf_677_symmetry_breaker(
      input_flags const *const flags_,
      [[maybe_unused]] std::vector<std::int32_t> const &v_,
      std::vector<std::vector<std::int32_t>> &cnf_,
      [[maybe_unused]] std::int32_t &next_) {
        create_cnf_first_line_constraint(flags_, v_, cnf_);
    }

    static constexpr void create_cnf(
      input_flags const *const flags_,
      std::vector<std::int32_t> const &v_,
      std::vector<std::vector<std::int32_t>> &cnf_,
      [[maybe_unused]] std::int32_t &next_) {
        create_cnf_magma(flags_, cnf_, next_);
        create_cnf_inv_magma(flags_, cnf_, next_);
        create_cnf_677_magma(flags_, cnf_, next_);
        create_cnf_677_inv_magma(flags_, cnf_, next_);
        create_cnf_677_equivalence(flags_, cnf_, next_);
        create_cnf_677_properties(flags_, cnf_, next_);
        create_cnf_677_symmetry_breaker(flags_, v_, cnf_, next_);
    }
}  // namespace magma_dimacs_cnf

#endif
