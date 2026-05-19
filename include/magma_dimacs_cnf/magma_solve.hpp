#ifndef MAGMA_DIMACS_CNF_MAGMA_SOLVE_HPP
#define MAGMA_DIMACS_CNF_MAGMA_SOLVE_HPP

#include <magma_dimacs_cnf/assert_and_assume.hpp>
#include <magma_dimacs_cnf/create_cnf.hpp>
#include <magma_dimacs_cnf/input_flags.hpp>
#include <magma_dimacs_cnf/sum.hpp>

#include <boost/asio.hpp>

#include <cadical.hpp>

#include <cstdint>
#include <fstream>
#include <print>
#include <vector>

namespace magma_dimacs_cnf {
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
      [[maybe_unused]] CaDiCaL::Solver &solver_,
      std::vector<std::int32_t> const &v_,
      [[maybe_unused]] std::vector<std::vector<std::int32_t>> const &cnf_,
      [[maybe_unused]] std::int32_t &next_) {
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

        switch(solver.simplify(10)) {
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
