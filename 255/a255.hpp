#ifndef MAGMA_255_A255_HPP
#define MAGMA_255_A255_HPP

#include "../types.hpp"

#include <functional>
#include <vector>

namespace magma {
    /**
     * This function generates clauses for 255 to be satisfied.
     * @param clauses The container to store clauses of type `std::vector<ssize>`.
     * @param var `var(x, y, i)` is $X_{xy, i}$.
     * @param n The number of elements in the magma.
     */
    inline void a255(
      std::vector<std::vector<ssize>> &clauses,
      std::function<ssize(ssize, ssize, ssize)> const &var,
      ssize const n) {
        /**
         * THE MAIN EQUATION
         *
         * Equation 255 is $x = ((xx)x)x$.
         * Therefore, we create a clause
         * $\neg X_{xx, a} \lor \neg X_{ax, b} \lor X_{bx, x}$
         * for every $x$, $a$, and $b$. */
        for(ssize x = 0; x < n; x++) {
            for(ssize a = 0; a < n; a++) {
                for(ssize b = 0; b < n; b++) {
                    clauses.push_back({-var(x, x, a), -var(a, x, b), var(b, x, x)});
                }
            }
        }
    }
}  // namespace magma

#endif  // MAGMA_255_A255_HPP
