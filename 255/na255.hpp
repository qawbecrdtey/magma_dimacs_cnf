#ifndef MAGMA_255_NA255_HPP
#define MAGMA_255_NA255_HPP

#include "../types.hpp"

#include <functional>
#include <vector>

namespace magma {
    /**
     * This function generates clauses to find a case with at least one variable
     * which doesn't satisfy 255.
     * @param clauses The container to store clauses of type `std::vector<ssize>`.
     * @param var1 `var1(x, y, i)` is $X_{xy, i}$.
     * @param var2 `var2(x)` is $Y_{x} = X_{xx, a} \to X_{ax, b} \to X_{bx, x}$ for all $a$ and $b$.
     * @param n The number of elements in the magma.
     */
    inline void na255(
      std::vector<std::vector<ssize>> &clauses,
      std::function<ssize(ssize, ssize, ssize)> const &var1,
      std::function<ssize(ssize)> const &var2,
      ssize const n) {
        /** $Y_{x}$ is equivalent to $X_{xx, a} \to X_{ax, b} \to X_{bx, x}$ for all $a$ and $b$. */
        for(ssize x = 0; x < n; x++) {
            for(ssize a = 0; a < n; a++) {
                for(ssize b = 0; b < n; b++) {
                    clauses.push_back({-var1(x, x, a), -var1(a, x, b), -var1(b, x, x), var2(x)});
                    clauses.push_back({-var2(x), -var1(x, x, a), -var1(a, x, b), var1(b, x, x)});
                }
            }
        }

        /**
         * If not all elements of magma satisfy equation 255,
         * then there should be some $Y_{x}$ which is false. */
        {
            std::vector<ssize> clause;
            clause.reserve(n);
            for(ssize x = 0; x < n; x++) { clause.push_back(-var2(x)); }
            clauses.push_back(std::move(clause));
        }

        /** Symmetry breaking - USE WITH CAUTION */

        /**
         * If equation 255 is false, then there should be at least one $x$ with $xx \neq x$.
         * Let such $x$ be 0. */
        { clauses.push_back({-var1(0, 0, 0)}); }

        /** Symmetry breaking end */
    }
}  // namespace magma

#endif  // MAGMA_255_NA255_HPP