#ifndef MAGMA_255_NE255_HPP
#define MAGMA_255_NE255_HPP

#include "../types.hpp"

#include <functional>
#include <vector>

namespace magma {
    /**
     * This function generates clauses so that no elements of the magma satisfies 255.
     * @param clauses The container to store clauses of type `std::vector<ssize>`.
     * @param var `var(x, y, i)` is $X_{xy, i}$.
     * @param n The number of elements in the magma.
     */
    inline void ne255(
      std::vector<std::vector<ssize>> &clauses,
      std::function<ssize(ssize, ssize, ssize)> const &var,
      ssize const n) {
        for(ssize x = 0; x < n; x++) {
            for(ssize a = 0; a < n; a++) {
                for(ssize b = 0; b < n; b++) {
                    clauses.push_back({-var(x, x, a), -var(a, x, b), -var(b, x, x)});
                }
            }
        }
    }
}  // namespace magma

#endif  // MAGMA_255_NE255_HPP
