#ifndef MAGMA_2504_A2504_HPP
#define MAGMA_2504_A2504_HPP

#include "../types.hpp"

#include <functional>
#include <vector>

namespace magma {
    /**
     * This function generates clauses for 2504 to be satisfied.
     * @param clauses The container to store clauses of type `std::vector<ssize>`.
     * @param var `var(x, y, i)` is $X_{xy, i}$.
     * @param n The number of elements in the magma.
     */
    inline void a2504(
      std::vector<std::vector<ssize>> &clauses,
      std::function<ssize(ssize, ssize, ssize)> const &var,
      ssize const n) {
        /**
         * THE MAIN EQUATION
         *
         * Equation 2504 is $x = (y((xy)x))y$.
         * Therefore, we create a clause
         * $\neg X_{xy, a} \lor \neg X_{ax, b} \lor \neg X_{yb, c} \lor X_{cy, x}$
         * for every $x$, $y$, $a$, $b$, and $c$. */
        for(ssize x = 0; x < n; x++) {
            for(ssize y = 0; y < n; y++) {
                for(ssize a = 0; a < n; a++) {
                    for(ssize b = 0; b < n; b++) {
                        for(ssize c = 0; c < n; c++) {
                            clauses.push_back({-var(x, y, a), -var(a, x, b), -var(y, b, c), var(c, y, x)});
                        }
                    }
                }
            }
        }
    }
}  // namespace magma

#endif  // MAGMA_2504_A2504_HPP
