#ifndef MAGMA_DIMACS_CNF_MAGMA_SOLVE_HPP
#define MAGMA_DIMACS_CNF_MAGMA_SOLVE_HPP

#include <magma_dimacs_cnf/assert_and_assume.hpp>
#include <magma_dimacs_cnf/input_flags.hpp>
#include <magma_dimacs_cnf/create_cnf.hpp>
#include <magma_dimacs_cnf/sum.hpp>

#include <boost/asio.hpp>

#include <cadical.hpp>

#include <cstdint>
#include <fstream>
#include <print>
#include <vector>

namespace magma_dimacs_cnf {
    // static constexpr void
    // create_cnf_base(input_flags const *const flags_, std::vector<std::vector<std::int32_t>>
    // &cnf_) {
    //     ASSERT_AND_ASSUME(cnf_.empty());
    //
    //     auto const n = flags_->element_count;
    //     CREATE_VAR(n);
    //
    //     // For any $x$ and $y$, if $x=(yx)z$, then $z=(y(yx))y$ holds.
    //     for(std::int32_t i = 0; i < n; i++) {  // x
    //         for(std::int32_t j = 0; j < n; j++) {  // y
    //             for(std::int32_t k = 0; k < n; k++) {  // z
    //                 for(std::int32_t l = 0; l < n; l++) {  // yx
    //                     for(std::int32_t m = 0; m < n; m++) {  // y(yx)
    //                         cnf_.emplace_back(
    //                           std::initializer_list {
    //                             -var(j, i, l), -var(l, k, i), -var(j, l, m), var(m, j, k)});
    //                     }
    //                 }
    //             }
    //         }
    //     }
    //
    //     // For fixed $x$ and $z$, $xy=z$ has a unique solution for $y$.
    //     for(std::int32_t i = 0; i < n; i++) {
    //         for(std::int32_t k = 0; k < n; k++) {
    //             for(std::int32_t j = 0; j < n - 1; j++) {
    //                 for(std::int32_t l = j + 1; l < n; l++) {
    //                     cnf_.emplace_back(std::initializer_list {-var(i, j, k), -var(i, l, k)});
    //                 }
    //             }
    //
    //             std::vector<std::int32_t> cl;
    //             cl.reserve(n);
    //             for(std::int32_t j = 0; j < n; j++) { cl.emplace_back(var(i, j, k)); }
    //             cnf_.emplace_back(std::move(cl));
    //         }
    //     }
    //
    //     // For any $x = yx$, it holds that $y=(xx)x$.
    //     for(std::int32_t i = 0; i < n; i++) {  // x
    //         for(std::int32_t j = 0; j < n; j++) {  // y
    //             for(std::int32_t k = 0; k < n; k++) {  // xx
    //                 cnf_.emplace_back(
    //                   std::initializer_list {-var(j, i, i), -var(i, i, k), var(k, i, j)});
    //             }
    //         }
    //     }
    //
    //     // For any $x$, if $x=xy$, then $(xx)x=y(xx)$ holds.
    //     for(std::int32_t i = 0; i < n; i++) {  // x
    //         for(std::int32_t j = 0; j < n; j++) {  // xx
    //             for(std::int32_t k = 0; k < n; k++) {  //(xx)x
    //                 for(std::int32_t l = 0; l < n; l++) {  // y
    //                     cnf_.emplace_back(
    //                       std::initializer_list {
    //                         -var(i, i, j), -var(j, i, k), -var(i, l, i), var(l, j, k)});
    //                 }
    //             }
    //         }
    //     }
    //
    //     // For any $x$, if $x=xy$ and $y=xz$, then $z=y(xx)$ holds.
    //     for(std::int32_t i = 0; i < n; i++) {  // x
    //         for(std::int32_t j = 0; j < n; j++) {  // xx
    //             for(std::int32_t k = 0; k < n; k++) {  // y
    //                 for(std::int32_t l = 0; l < n; l++) {  // z
    //                     cnf_.emplace_back(
    //                       std::initializer_list {
    //                         -var(i, i, j), -var(i, k, i), -var(i, l, k), var(k, j, l)});
    //                 }
    //             }
    //         }
    //     }
    //
    //     // For any $x$, if $x=xy$ and $x\neq xx$, then $x\neq yx$.
    //     for(std::int32_t i = 0; i < n; i++) {  // x
    //         for(std::int32_t j = 0; j < n; j++) {  // xx
    //             if(i == j) { continue; }
    //             for(std::int32_t k = 0; k < n; k++) {  // y
    //                 cnf_.emplace_back(
    //                   std::initializer_list {-var(i, i, j), -var(i, k, i), -var(k, i, i)});
    //             }
    //         }
    //     }
    // }
    //
    // static constexpr void create_cnf_first_line_constraint(
    //   input_flags const *const flags_,
    //   std::vector<std::int32_t> const &v_,
    //   std::vector<std::vector<std::int32_t>> &cnf_) {
    //     auto const n = flags_->element_count;
    //     ASSERT_AND_ASSUME(sum(v_) == n);
    //     CREATE_VAR(n);
    //
    //     std::int32_t begin = 0;
    //     // Consider the first row, which is of the form $0x=y$.
    //     // Each value in `v_` represents a length of each circuit.
    //     for(auto const l: v_) {
    //         ASSERT_AND_ASSUME(0 < l);
    //         std::int32_t const end = begin + l;
    //         for(std::int32_t i = begin; i < end; i++) {
    //             cnf_.emplace_back(std::initializer_list {var(0, i, (i - begin + 1) % l +
    //             begin)});
    //         }
    //         begin = end;
    //     }
    // }
    //
    // // Row `lhs_` is lexicographically less than row `rhs_`.
    // [[maybe_unused]]
    // static constexpr void create_cnf_lex_less(
    //   [[maybe_unused]] input_flags const *const flags_,
    //   [[maybe_unused]] std::vector<std::vector<std::int32_t>> &cnf_,
    //   [[maybe_unused]] std::int32_t const lhs_,
    //   [[maybe_unused]] std::int32_t const rhs_,
    //   [[maybe_unused]] std::int32_t &next_) {
    //     auto const n = flags_->element_count;
    //     ASSERT_AND_ASSUME(0 <= lhs_);
    //     ASSERT_AND_ASSUME(lhs_ < n);
    //     ASSERT_AND_ASSUME(0 <= rhs_);
    //     ASSERT_AND_ASSUME(rhs_ < n);
    // }
    //
    // // The map $x\mapsto L_x$ is injective.
    // // For distinct $x$ and $y$, $xy\neq yy$ or $x(y((yy)y))\neq y$ holds.
    // // For distinct $x$ and $y$, $L^{-1}_y L_x y\neq y$ or $L_x L^{-1}_y y\neq y$ holds.
    // static constexpr void create_cnf_no_equal_rows(
    //   input_flags const *const flags_,
    //   std::vector<std::vector<std::int32_t>> &cnf_,
    //   std::int32_t &next_) {
    //     auto const n = flags_->element_count;
    //     CREATE_VAR(n);
    //
    //     for(std::int32_t i = 0; i < n; i++) {  // x
    //         for(std::int32_t j = 0; j < n; j++) {  // y
    //             if(i == j) { continue; }
    //
    //         }
    //     }
    //
    //     // for(std::int32_t i = 0; i < n - 1; i++) {  // x
    //     //     for(std::int32_t j = i + 1; j < n; j++) {  // y
    //     //         std::int32_t const begin = next_;
    //     //
    //     //         // There exists some $z$ which $xz\neq yz$.
    //     //         for(std::int32_t k = 0; k < n; k++) {
    //     //             for(std::int32_t l = 0; l < n; l++) {
    //     //                 cnf_.emplace_back(
    //     //                   std::initializer_list {next_, -var(i, k, l), -var(j, k, l)});
    //     //                 cnf_.emplace_back(
    //     //                   std::initializer_list {-next_, -var(i, k, l), var(j, k, l)});
    //     //                 cnf_.emplace_back(
    //     //                   std::initializer_list {-next_, var(i, k, l), -var(j, k, l)});
    //     //             }
    //     //             next_++;
    //     //         }
    //     //         std::vector<std::int32_t> cl;
    //     //         cl.reserve(n);
    //     //         for(std::int32_t k = 0; k < n; k++) { cl.emplace_back(-begin - k); }
    //     //         cnf_.emplace_back(std::move(cl));
    //     //     }
    //     // }
    // }

    static constexpr void
    pass_cnf(CaDiCaL::Solver &solver_, std::vector<std::vector<std::int32_t>> const &cnf_) {
        for(auto const &cl: cnf_) {
            for(auto const l: cl) {
                ASSERT_AND_ASSUME(l != 0);
                solver_.add(l);
            }
            solver_.add(0);
        }
    }

    static constexpr void create_name(std::vector<std::int32_t> const &v_, std::string &name_) {
        ASSERT_AND_ASSUME(name_.empty());

        char s[] {'\0', '\0'};
        for(auto const l: v_) {
            name_ += s;
            name_ += std::to_string(l);
            s[0] = '_';
        }
    }

    static constexpr void print_cycles(std::ostream &os_, std::vector<std::int32_t> const &v_) {
        ASSERT_AND_ASSUME(!v_.empty());
        char s[] {'\0', '\0'};
        for(auto const l: v_) {
            std::print(os_, "{}{}", s, l);
            s[0] = ' ';
        }
    }

    static constexpr void
    print_assignments(std::ostream &os_, std::int32_t const n_, CaDiCaL::Solver &solver_) {
        ASSERT_AND_ASSUME(0 < n_);
        CREATE_VAR(n_);

        char s[] {'\0', '\0'};
        for(std::int32_t i = 0; i < n_; i++) {
            for(std::int32_t j = 0; j < n_; j++) {
                for(std::int32_t k = 0; k < n_; k++) {
                    std::print(os_, "{}{}", s, solver_.val(var(i, j, k)));
                    s[0] = ' ';
                }
            }
        }
    }

    static constexpr void save_result_sat(
      input_flags const *const flags_,
      CaDiCaL::Solver &solver_,
      std::vector<std::int32_t> const &v_) {
        ASSERT_AND_ASSUME(!v_.empty());

        std::string name;
        create_name(v_, name);

        std::ofstream ofs {flags_->path / name};
        if(!ofs) {
            std::print(stderr, "Failed to create file.\n");
            std::exit(1);
        }

        print_cycles(ofs, v_);
        std::print(ofs, "\nSAT\n");
        print_assignments(ofs, flags_->element_count, solver_);

        std::print(stderr, "{} end\n", v_);
    }

    static constexpr void save_result_unsat(
      input_flags const *const flags_,
      CaDiCaL::Solver &solver_,
      std::vector<std::int32_t> const &v_,
      std::vector<std::vector<std::int32_t>> const &cnf_,
      std::int32_t &next_) {
        ASSERT_AND_ASSUME(!v_.empty());

        std::string name;
        create_name(v_, name);

        std::ofstream ofs {flags_->path / name};
        if(!ofs) {
            std::print(stderr, "Failed to create file.\n");
            std::exit(1);
        }

        print_cycles(ofs, v_);
        std::print(ofs, "\nUNSAT\n");

        std::print(stderr, "{} end\n", v_);
    }

    static constexpr void
    save_result_unknown(input_flags const *const flags_, std::vector<std::int32_t> const &v_) {
        std::string name;
        create_name(v_, name);

        std::ofstream ofs {flags_->path / name};
        if(!ofs) {
            std::print(stderr, "Failed to create file.\n");
            std::exit(1);
        }

        print_cycles(ofs, v_);
        std::print(ofs, "\nUNKNOWN\n");

        std::print(stderr, "{} end\n", v_);
    }

    static constexpr void solve(
      input_flags const *const flags_,
      std::vector<std::int32_t> const &v_,
      std::vector<std::vector<std::int32_t>> const &cnf_,
      std::int32_t &next_) {
        std::string name;
        create_name(v_, name);

        if(!exists(flags_->path / "drat")) {
            std::print(stderr, "drat directory does not exist.\n");
            std::exit(1);
        }

        CaDiCaL::Solver solver;
        solver.trace_proof((flags_->path / "drat" / (name + ".drat")).c_str());
        solver.resize(next_);

        pass_cnf(solver, cnf_);

        switch(solver.simplify()) {
        case 10: {  // SAT
            save_result_sat(flags_, solver, v_);
        } break;
        case 20: {  // UNSAT
            save_result_unsat(flags_, solver, v_, cnf_, next_);
        } break;
        default: {  // UNKNOWN
            switch(solver.solve()) {
            case 10: {  // SAT
                save_result_sat(flags_, solver, v_);
            } break;
            case 20: {  // UNSAT
                save_result_unsat(flags_, solver, v_, cnf_, next_);
            } break;
            default: {  // UNKNOWN
                save_result_unknown(flags_, v_);
            } break;
            }
        } break;
        }
    }

    static constexpr void magma_solve_inner(
      boost::asio::io_context &io_context_,
      input_flags const *const flags_,
      std::int32_t const n_,
      std::vector<std::int32_t> const v_) {
        ASSERT_AND_ASSUME(!v_.empty());

        auto const s = sum(v_);
        ASSERT_AND_ASSUME(0 < s);

        auto const n = flags_->element_count;
        ASSERT_AND_ASSUME(s <= n);

        if(s < n) {
            for(std::int32_t i = n_; i <= n - s; i++) {
                post(io_context_, [&io_context_, flags_, i, v_] {
                    std::vector v {v_};
                    v.push_back(i);
                    magma_solve_inner(io_context_, flags_, i, std::move(v));
                });
            }
            return;
        }

        ASSERT_AND_ASSUME(s == n);

        std::vector<std::vector<std::int32_t>> cnf;
        std::int32_t next = 1 + 2 * n * n * n;
        create_cnf(flags_, v_, cnf, next);
        solve(flags_, v_, cnf, next);
    }

    static constexpr void magma_solve(input_flags const &flags_) {
        boost::asio::io_context io_context;

        for(std::int32_t i = 1; i <= flags_.element_count; i++) {
            if(i == 2 || i == 3) {
                // There is no magma which contains 2- or 3-cycle with $0\times 0$. 2-cycle implies
                // $0\times (0\times 0)=0$, 3-cycle implies $0\times (0\times (0\times 0)) = 0$, and
                // both directly implies $0\times 0 = 0$.
                continue;
            }
            post(io_context, [&io_context, &flags_, i] {
                magma_solve_inner(io_context, &flags_, 1, std::vector {i});
            });
        }

        std::vector<std::thread> threads;
        threads.reserve(flags_.thread_count);
        for(std::remove_cvref_t<decltype(flags_.thread_count)> i = 0; i < flags_.thread_count; i++) {
            threads.emplace_back([&io_context] { io_context.run(); });
        }
        for(std::remove_cvref_t<decltype(flags_.thread_count)> i = 0; i < flags_.thread_count; i++) {
            threads[i].join();
        }
    }
}  // namespace magma_dimacs_cnf

#endif
