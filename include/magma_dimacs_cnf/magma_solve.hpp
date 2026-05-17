#ifndef MAGMA_DIMACS_CNF_MAGMA_SOLVE_HPP
#define MAGMA_DIMACS_CNF_MAGMA_SOLVE_HPP

#include <magma_dimacs_cnf/assert_and_assume.hpp>
#include <magma_dimacs_cnf/create_var.hpp>
#include <magma_dimacs_cnf/input_flags.hpp>

#include <boost/asio.hpp>

#include <cadical.hpp>

#include <cstdint>
#include <fstream>
#include <print>
#include <vector>

namespace magma_dimacs_cnf {
    static constexpr std::int32_t sum(std::vector<std::int32_t> const &v_) noexcept {
        std::int32_t res {0};
        for(auto const &val: v_) {
            ASSERT_AND_ASSUME(0 < val);
            ASSERT_AND_ASSUME(
              static_cast<std::int64_t>(val) + static_cast<std::int64_t>(res)
              < static_cast<std::int64_t>(std::numeric_limits<std::int32_t>::max()));
            res += val;
        }
        return res;
    }

    static constexpr void
    create_cnf_base(input_flags const *const flags_, std::vector<std::vector<std::int32_t>> &cnf_) {
        ASSERT_AND_ASSUME(cnf_.empty());

        auto const N = flags_->element_count;
        CREATE_VAR(N);

        for(std::int32_t i = 0; i < N; i++) {
            for(std::int32_t j = 0; j < N; j++) {
                for(std::int32_t k = 0; k < N - 1; k++) {
                    for(std::int32_t l = k + 1; l < N; l++) {
                        cnf_.emplace_back(std::initializer_list {-var(i, j, k), -var(i, j, l)});
                    }
                }
                std::vector<std::int32_t> cl;
                cl.reserve(N);
                for(std::int32_t k = 0; k < N; k++) { cl.emplace_back(var(i, j, k)); }
                cnf_.emplace_back(std::move(cl));
            }
        }

        // For any $x$ and $y$, $x=y(x((yx)y))$ holds.
        for(std::int32_t i = 0; i < N; i++) {  // x
            for(std::int32_t j = 0; j < N; j++) {  // y
                for(std::int32_t k = 0; k < N; k++) {  // yx
                    for(std::int32_t l = 0; l < N; l++) {  // (yx)y
                        for(std::int32_t m = 0; m < N; m++) {  // x((yx)y)
                            cnf_.emplace_back(
                              std::initializer_list {
                                -var(j, i, k), -var(k, j, l), -var(i, l, m), var(j, m, i)});
                        }
                    }
                }
            }
        }

        // For any $x=yz$, $z=x((yx)y)$ holds.
        for(std::int32_t i = 0; i < N; i++) {  // x
            for(std::int32_t j = 0; j < N; j++) {  // y
                for(std::int32_t k = 0; k < N; k++) {  // z
                    for(std::int32_t l = 0; l < N; l++) {  // yx
                        for(std::int32_t m = 0; m < N; m++) {  // (yx)y
                            cnf_.emplace_back(
                              std::initializer_list {
                                -var(j, k, i), -var(j, i, l), -var(l, j, m), var(i, m, k)});
                        }
                    }
                }
            }
        }

        // For any $x$ and $y$, if $x=yz$ and $z=xw$, then $w=(yx)y$ holds.
        for(std::int32_t i = 0; i < N; i++) {  // x
            for(std::int32_t j = 0; j < N; j++) {  // y
                for(std::int32_t k = 0; k < N; k++) {  // z
                    for(std::int32_t l = 0; l < N; l++) {  // w
                        for(std::int32_t m = 0; m < N; m++) {  // yx
                            cnf_.emplace_back(
                              std::initializer_list {
                                -var(j, k, i), -var(i, l, k), -var(j, i, m), var(m, j, l)});
                        }
                    }
                }
            }
        }

        // For fixed $x$ and $z$, $xy=z$ has a unique solution for $y$.
        for(std::int32_t i = 0; i < N; i++) {
            for(std::int32_t k = 0; k < N; k++) {
                for(std::int32_t j = 0; j < N - 1; j++) {
                    for(std::int32_t l = j + 1; l < N; l++) {
                        cnf_.emplace_back(std::initializer_list {-var(i, j, k), -var(i, l, k)});
                    }
                }

                std::vector<std::int32_t> cl;
                cl.reserve(N);
                for(std::int32_t j = 0; j < N; j++) { cl.emplace_back(var(i, j, k)); }
                cnf_.emplace_back(std::move(cl));
            }
        }

        // For any $x = yx$, it holds that $y = (xx)x$.
        for(std::int32_t i = 0; i < N; i++) {  // x
            for(std::int32_t j = 0; j < N; j++) {  // y
                for(std::int32_t k = 0; k < N; k++) {  // xx
                    cnf_.emplace_back(
                      std::initializer_list {-var(j, i, i), -var(i, i, k), var(k, i, j)});
                }
            }
        }

        // For any $x$, $(xx)x=(x((xx)x))(xx)$ holds.
        for(std::int32_t i = 0; i < N; i++) {  // x
            for(std::int32_t j = 0; j < N; j++) {  // xx
                for(std::int32_t k = 0; k < N; k++) {  //(xx)x
                    for(std::int32_t l = 0; l < N; l++) {  // x((xx)x)
                        cnf_.emplace_back(
                          std::initializer_list {
                            -var(i, i, j), -var(j, i, k), -var(i, k, l), var(l, j, k)});
                    }
                }
            }
        }

        // For any $x$, if $x=xy$, then $(xx)x=y(xx)$ holds.
        for(std::int32_t i = 0; i < N; i++) {  // x
            for(std::int32_t j = 0; j < N; j++) {  // xx
                for(std::int32_t k = 0; k < N; k++) {  //(xx)x
                    for(std::int32_t l = 0; l < N; l++) {  // y
                        cnf_.emplace_back(
                          std::initializer_list {
                            -var(i, i, j), -var(j, i, k), -var(i, l, i), var(l, j, k)});
                    }
                }
            }
        }

        // For any $x$, if $x=xy$ and $x\neq xx$, then $x\neq yx$.
        for(std::int32_t i = 0; i < N; i++) {  // x
            for(std::int32_t j = 0; j < N; j++) {  // xx
                if(i == j) { continue; }
                for(std::int32_t k = 0; k < N; k++) {  // y
                    cnf_.emplace_back(
                      std::initializer_list {-var(i, i, j), -var(i, k, i), -var(k, i, i)});
                }
            }
        }

        for(std::int32_t i = 0; i < N; i++) {  // x
            for(std::int32_t j = 0; j < N; j++) {  // xx
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

                for(std::int32_t k = 0; k < N; k++) {  // (xx)x
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
                }

                for(std::int32_t k = 0; k < N; k++) {  // x(xx)
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
      std::vector<std::vector<std::int32_t>> const &cnf_) {
        ASSERT_AND_ASSUME(!v_.empty());

        std::string name;
        create_name(v_, name);

        std::ofstream ofs {flags_->path / name};
        if(!ofs) {
            std::print(stderr, "Failed to create file.\n");
            std::exit(1);
        }

        print_cycles(ofs, v_);
        std::print(ofs, "\nUNSAT");

        auto const n = flags_->element_count;
        for(std::remove_cvref_t<decltype(cnf_.size())> i = 0; i < cnf_.size(); i++) {
            if(solver_.failed(1 + i + n * n * n)) {
                std::print(ofs, "\n{}:", i);
                ASSERT_AND_ASSUME(i < cnf_.size());
                for(auto const l: cnf_[i]) { std::print(ofs, " {}", l); }
            }
        }

        std::print(stderr, "{} end\n", v_);
    }

    static constexpr void solve(
      input_flags const *const flags_,
      std::vector<std::int32_t> const &v_,
      std::vector<std::vector<std::int32_t>> const &cnf_) {
        std::print(stderr, "{} start\n", v_);

        CaDiCaL::Solver solver;

        auto const n = flags_->element_count;
        auto idx = 1 + n * n * n;
        for(auto const &v: cnf_) {
            // solver_.assume(idx);
            solver.add(-idx);
            for(auto const l: v) { solver.add(l); }
            solver.add(0);
            idx++;
        }

        switch(solver.simplify()) {
        case 10: {  // SAT
            save_result_sat(flags_, solver, v_);
        } break;
        case 20: {  // UNSAT
            save_result_unsat(flags_, solver, v_, cnf_);
        } break;
        default: {  // UNKNOWN
            idx = 1 + n * n * n;
            for([[maybe_unused]]
                auto const &v: cnf_) {
                solver.assume(idx);
                idx++;
            }

            switch(solver.solve()) {
            case 10: {  // SAT
                save_result_sat(flags_, solver, v_);
            } break;
            case 20: {  // UNSAT
                save_result_unsat(flags_, solver, v_, cnf_);
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
      std::vector<std::int32_t> v_) {
        ASSERT_AND_ASSUME(0 < n_);
        ASSERT_AND_ASSUME(!v_.empty());

        auto const N = flags_->element_count;
        auto const s = sum(v_);
        ASSERT_AND_ASSUME(s <= N);

        auto const n = N - s;
        if(n < n_) { return; }

        for(std::int32_t i = n_; i < n; i++) {
            post(io_context_, [&io_context_, flags_, v_, i] {
                std::vector v {v_};
                v.push_back(i);
                magma_solve_inner(io_context_, flags_, i, std::move(v));
            });
        }

        v_.push_back(n);

        std::vector<std::vector<std::int32_t>> cnf;
        create_cnf_base(flags_, cnf);
        create_cnf_first_line_constraint(flags_, v_, cnf);

        solve(flags_, v_, cnf);
    }

    static constexpr void magma_solve(input_flags const &flags_) {
        boost::asio::io_context io_context;

        for(std::int32_t i = 1; i < flags_.element_count; i++) {
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

        post(io_context, [flags = &flags_, i = flags_.element_count] {
            std::vector<std::int32_t> const v {std::initializer_list {i}};

            std::vector<std::vector<std::int32_t>> cnf;
            create_cnf_base(flags, cnf);
            create_cnf_first_line_constraint(flags, v, cnf);

            solve(flags, v, cnf);
        });

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
