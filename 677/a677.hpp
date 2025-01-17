#ifndef MAGMA_677_A677_HPP
#define MAGMA_677_A677_HPP

#include "../types.hpp"

#include <functional>
#include <vector>

namespace magma {
    /**
     * This function generates clauses for 677 to be satisfied.
     * @param clauses The container to store clauses of type `std::vector<ssize>`.
     * @param var `var(x, y, i)` is $X_{xy, i}$.
     * @param n The number of elements in the magma.
     */
    inline void a677(
      std::vector<std::vector<ssize>> &clauses,
      std::function<ssize(ssize, ssize, ssize)> const &var,
      ssize const n) {
        /**
         * THE MAIN EQUATION
         *
         * Equation 677 is $x = y(x((yx)y))$.
         * Therefore, we create a clause
         * $\neg X_{yx, a} \lor \neg X_{ay, b} \lor \neg X_{xb, c} \lor X_{yc, x}$
         * for every $a$, $b$, $c$, $x$, and $y$. */
        for(ssize x = 0; x < n; x++) {
            for(ssize y = 0; y < n; y++) {
                for(ssize a = 0; a < n; a++) {
                    for(ssize b = 0; b < n; b++) {
                        for(ssize c = 0; c < n; c++) {
                            clauses.push_back(
                              {-var(y, x, a), -var(a, y, b), -var(x, b, c), var(y, c, x)});
                        }
                    }
                }
            }
        }

        /**
         * Equation 677 implies that for any $x$ and $y$,
         * there is some $z$ which $x = yz$.
         * Also, such $z$ is unique. */
        for(ssize x = 0; x < n; x++) {
            for(ssize y = 0; y < n; y++) {
                for(ssize i = 0; i < n; i++) {
                    for(ssize j = i + 1; j < n; j++) {
                        clauses.push_back({-var(y, i, x), -var(y, j, x)});
                    }
                }

                std::vector<ssize> clause;
                clause.reserve(n);
                for(ssize i = 0; i < n; i++) { clause.push_back(var(y, i, x)); }
                clauses.push_back(std::move(clause));
            }
        }

        /**
         * Equation 677 implies that $x = xx$ if $x = (xx)x$.
         * The converse is also true, but it is already covered by the uniqueness of the true-valued
         * variable. */
        for(ssize x = 0; x < n; x++) {
            for(ssize a = 0; a < n; a++) {
                clauses.push_back({-var(x, x, a), -var(a, x, x), var(x, x, x)});
            }
        }

        /**
         * Equation 677 implies that $x = xx$ if $x = x(xx)$.
         * The converse is also true, but it is already covered by the uniqueness of the true-valued
         * variable. */
        for(ssize x = 0; x < n; x++) {
            for(ssize a = 0; a < n; a++) {
                clauses.push_back({-var(x, x, a), -var(x, a, x), var(x, x, x)});
            }
        }

        /**
         * Equation 677 implies that $x = xx$ if $x = x((xx)x)$.
         * The converse is also true, but it is already covered by the uniqueness of the true-valued
         * variable. */
        for(ssize x = 0; x < n; x++) {
            for(ssize a = 0; a < n; a++) {
                for(ssize b = 0; b < n; b++) {
                    clauses.push_back({-var(x, x, a), -var(a, x, b), -var(x, b, x), var(x, x, x)});
                }
            }
        }

        /**
         * Equation 677 implies that $x = xx$ if $x = (x(xx))x$.
         * The converse is also true, but it is already covered by the uniqueness of the true-valued
         * variable. */
        for(ssize x = 0; x < n; x++) {
            for(ssize a = 0; a < n; a++) {
                for(ssize b = 0; b < n; b++) {
                    clauses.push_back({-var(x, x, a), -var(x, a, b), -var(b, x, x), var(x, x, x)});
                }
            }
        }

        /** Equation 677 implies that $y = (xx)x$ if $x = yx$. */
        for(ssize x = 0; x < n; x++) {
            for(ssize y = 0; y < n; y++) {
                for(ssize a = 0; a < n; a++) {
                    clauses.push_back({-var(y, x, x), -var(x, x, a), var(a, x, y)});
                }
            }
        }

        /** Equation 677 implies that, for each $x$, there is at most one $y$ which $x = yx$. */
        for(ssize x = 0; x < n; x++) {
            for(ssize a = 0; a < n; a++) {
                for(ssize b = a + 1; b < n; b++) {
                    clauses.push_back({-var(a, x, x), -var(b, x, x)});
                }
            }
        }

        /**
         * Equation 677 implies that $y = (xy)(yx)$ if $xy = y*x$,
         * where $z = x*y$ is equivalent to $x = yz$. */
        for(ssize x = 0; x < n; x++) {
            for(ssize y = 0; y < n; y++) {
                for(ssize a = 0; a < n; a++) {
                    for(ssize b = 0; b < n; b++) {
                        clauses.push_back(
                          {-var(x, y, a), -var(x, a, y), -var(y, x, b), var(a, b, y)});
                    }
                }
            }
        }

        /**
         * Equation 677 implies that, for given $x$, there exists some $z$ which $zx = z*x$
         * if there exists some $y$ which $yx = x$,
         * where $z = x*y$ is equivalent to $zx = y$. */
        for(ssize x = 0; x < n; x++) {
            for(ssize y = 0; y < n; y++) {
                for(ssize z = 0; z < n; z++) {
                    for(ssize a = 0; a < n; a++) {
                        clauses.push_back({-var(y, x, x), -var(z, x, a), var(x, a, z)});
                        clauses.push_back({-var(y, x, x), -var(x, a, z), var(z, x, a)});
                    }
                }
            }
        }
    }
}  // namespace magma
#endif  // MAGMA_677_A677_HPP
