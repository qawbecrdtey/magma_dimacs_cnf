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
     * @param var `var1(x, y, i)` is $X_{xy, i}$.
     * @param n The number of elements in the magma.
     */
    inline void na255(
      std::vector<std::vector<ssize>> &clauses,
      std::function<ssize(ssize, ssize, ssize)> const &var,
      ssize const n) {
        /** Symmetry breaking - USE WITH CAUTION */

        /**
         * If equation 255 is false, then there should be at least one $x$ with $xx \neq x$.
         * Let such $x$ be 0, and set $0 \cdot 0 = 1$.
         * If $1 \cdot 0 \neq 0$, then we may let $1 \cdot 0 = 2$.
         * It follows that $2 \cdot 0$ is one of $1$, $2$, or $3$, where $3$ is arbitrarily chosen.
         */
        if(4 <= n) {
            clauses.push_back({var(0, 0, 1)});
            clauses.push_back({var(1, 0, 0), var(1, 0, 2)});
            clauses.push_back({var(1, 0, 0), var(2, 0, 1), var(2, 0, 2), var(2, 0, 3)});
        }

        /** Symmetry breaking end */
    }
}  // namespace magma

#endif  // MAGMA_255_NA255_HPP