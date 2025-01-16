#include "../255/na255.hpp"
#include "a677.hpp"

#include <iostream>
#include <vector>

/**
 * This program generates a DIMACS CNF format which is satisfiable iff
 * there is a magma of order `N` satisfying the equation 677.
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
    /** `var2(x)` is $Y_{x} = \forall a b, X_{xx, a} \to X_{ax, b} \lor X_{bx, x}$. */
    auto const var2 = [n](ssize const x) noexcept { return 1 + n * n * n + x; };
    /** `var3(i)` is $Z_{x} = X_{0 \cdot x, x} \lor \ldots \lor X_{(n - 1) \cdot x, x}$. */
    auto const var3 = [n](ssize const i) noexcept { return 1 + n * n * n + n + i; };

    auto const variables_count = n * n * n + 2 * n;

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

    /** Not all elements of the magma satisfy equation 255. */
    na255(clauses, var1, var2, n);

    /** Additional conditions */

    /**
     * Equation 677 implies, for given $x$, there exists some $y$ which $yx = x$
     * if and only if equation 255 for $x$ holds.
     * To refute equation 255 under equation 677, we may give additional constraints that,
     * there is some $x$ which for all $y$, $yx \neq x$. */
    for(ssize x = 0; x < n; x++) {
        std::vector<ssize> clause;
        clause.reserve(n + 1);
        clause.push_back(-var3(x));
        for(ssize i = 0; i < n; i++) {
            clauses.push_back({-var1(i, x, x), var3(i)});
            clause.push_back(var1(i, x, x));
        }
        clauses.push_back(std::move(clause));
    }

    {
        std::vector<ssize> clause;
        clause.reserve(n);
        for(ssize x = 0; x < n; x++) { clause.push_back(-var3(x)); }
        clauses.push_back(std::move(clause));
    }

    /** Additional conditions end*/

    // DIMACS CNF Generation start
    std::cout << "p cnf " << variables_count << ' ' << clauses.size() << '\n';
    for(auto const &clause: clauses) {
        for(auto const lit: clause) { std::cout << lit << ' '; }
        std::cout << "0\n";
    }

    return 0;
}
