#include "../tools.hpp"

#include "../2504/a2504.hpp"
#include "a677.hpp"

#include <iostream>
#include <vector>

/**
 * This program generates a DIMACS CNF format which is satisfiable iff
 * there is a magma of order `N` satisfying equation 677 and 2504.
 *
 * Number of variables: `O(N^3)`
 * Number of clauses: `O(N^5)`
 */

int main(int argc, char *argv[]) {
    using namespace magma;

    if(argc != 2) {
        std::cerr << "Usage: " << argv[0] << " [Number of elements]" << std::endl;
        return 1;
    }

    ssize n;
    try {
        n = std::stoll(argv[1]);
        if(n < 5) { throw std::invalid_argument("Number of elements must be at least 5."); }
        if(n > 1'000'000) { throw std::invalid_argument("Number of elements is too large."); }
    } catch(std::exception const &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    std::vector<std::vector<ssize>> clauses;

    /** `var1(x, y, i)` is $X_{xy, i}$. */
    auto const var1 = [n](ssize const x, ssize const y, ssize const i) noexcept {
        return 1 + (x * n + y) * n + i;
    };

    auto const variables_count = n * n * n;

    /** Condition for variables */

    /** For every $x$ and $y$, exactly one of $X_{yx, i}$ for $0 \leq i < N$ is true. */
    for(ssize x = 0; x < n; x++) {
        for(ssize y = 0; y < n; y++) {
            /**
             * For each distinct pair $X_{yx, i}$ and $X_{yx, j}$ of variables,
             * there is at least one false variable. */
            for(ssize i = 0; i < n; i++) {
                for(ssize j = i + 1; j < n; j++) {
                    clauses.push_back({-var1(y, x, i), -var1(y, x, j)});
                }
            }

            /** There is at least one true $X_{yx, i}$. */
            std::vector<ssize> clause;
            clause.reserve(n);
            for(ssize i = 0; i < n; i++) { clause.push_back(var1(y, x, i)); }
            clauses.push_back(std::move(clause));
        }
    }

    /** Condition for variables end */

    /** All elements of the magma satisfy equation 677. */
    a677(clauses, var1, n);

    /** All elements of the magma satisfy equation 2504. */
    a2504(clauses, var1, n);

    // DIMACS CNF Generation start
    remove_duplicates(clauses);

    // Faster I/O
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    std::cout << "p cnf " << variables_count << ' ' << clauses.size() << '\n';
    for(auto const &clause: clauses) {
        for(auto const lit: clause) { std::cout << lit << ' '; }
        std::cout << "0\n";
    }

    return 0;
}
