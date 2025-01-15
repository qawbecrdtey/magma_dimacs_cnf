#include <iostream>
#include <vector>

/**
 * This program generates a DIMACS CNF format which is satisfiable iff
 * there is a magma of order `N` satisfying the equation 677.
 *
 * Number of variables: `N^3`
 * Number of clauses: `O(N^5)`
 */

int main(int argc, char *argv[]) {
    using ssize_t = long long;

    if(argc != 2) {
        std::cerr << "Usage: " << argv[0] << " [Number of elements]" << std::endl;
        return 1;
    }

    ssize_t n;
    try {
        n = std::stoll(argv[1]);
        if(n < 5) { throw std::invalid_argument("Number of elements must be at least 5."); }
        if(n > 1'000'000) { throw std::invalid_argument("Number of elements is too large."); }
    } catch(std::exception const &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    std::vector<std::vector<ssize_t>> clauses;

    /** `var1(x, y, i)` is $X_{xy, i}$. */
    auto const var1 = [n](ssize_t const x, ssize_t const y, ssize_t const i) noexcept {
        return 1 + (x * n + y) * n + i;
    };

    auto const variables_count = n * n * n;

    /** Condition for variables */

    /** For every $x$ and $y$, exactly one of $X_{yx, i}$ for $0 \leq i < N$ is true. */
    for(ssize_t x = 0; x < n; x++) {
        for(ssize_t y = 0; y < n; y++) {
            /**
             * For each distinct pair $X_{yx, i}$ and $X_{yx, j}$ of variables,
             * there is at least one false variable. */
            for(ssize_t i = 0; i < n; i++) {
                for(ssize_t j = i + 1; j < n; j++) {
                    clauses.push_back({-var1(y, x, i), -var1(y, x, j)});
                }
            }

            /** There is at least one true $X_{yx, i}$. */
            std::vector<ssize_t> clause;
            clause.reserve(n);
            for(ssize_t i = 0; i < n; i++) { clause.push_back(var1(y, x, i)); }
            clauses.push_back(std::move(clause));
        }
    }

    /** Condition for variables end */

    /** Condition for 677 magma */

    /**
     * THE MAIN EQUATION
     *
     * Equation 677 is $x = y(x((yx)y))$.
     * Therefore, we create a clause
     * $\neg X_{yx, a} \lor \neg X_{ay, b} \lor \neg X_{xb, c} \lor X_{yc, x}$
     * for every $a$, $b$, $c$, $x$, and $y$. */
    for(ssize_t x = 0; x < n; x++) {
        for(ssize_t y = 0; y < n; y++) {
            for(ssize_t a = 0; a < n; a++) {
                for(ssize_t b = 0; b < n; b++) {
                    for(ssize_t c = 0; c < n; c++) {
                        clauses.push_back(
                          {-var1(y, x, a), -var1(a, y, b), -var1(x, b, c), var1(y, c, x)});
                    }
                }
            }
        }
    }

    /**
     * Equation 677 implies that for any $x$ and $y$,
     * there is some $z$ which $x = yz$.
     * Also, such $z$ is unique. */
    for(ssize_t x = 0; x < n; x++) {
        for(ssize_t y = 0; y < n; y++) {
            for(ssize_t i = 0; i < n; i++) {
                for(ssize_t j = i + 1; j < n; j++) {
                    clauses.push_back({-var1(y, i, x), -var1(y, j, x)});
                }
            }

            std::vector<ssize_t> clause;
            clause.reserve(n);
            for(ssize_t i = 0; i < n; i++) { clause.push_back(var1(y, i, x)); }
            clauses.push_back(std::move(clause));
        }
    }

    /**
     * Equation 677 implies that $x = xx$ if $x = (xx)x$.
     * The converse is also true, but it is already covered by the uniqueness of the true-valued
     * variable. */
    for(ssize_t x = 0; x < n; x++) {
        for(ssize_t a = 0; a < n; a++) {
            clauses.push_back({-var1(x, x, a), -var1(a, x, x), var1(x, x, x)});
        }
    }

    /**
     * Equation 677 implies that $x = xx$ if $x = x(xx)$.
     * The converse is also true, but it is already covered by the uniqueness of the true-valued
     * variable. */
    for(ssize_t x = 0; x < n; x++) {
        for(ssize_t a = 0; a < n; a++) {
            clauses.push_back({-var1(x, x, a), -var1(x, a, x), var1(x, x, x)});
        }
    }

    /**
     * Equation 677 implies that $x = xx$ if $x = x((xx)x)$.
     * The converse is also true, but it is already covered by the uniqueness of the true-valued
     * variable. */
    for(ssize_t x = 0; x < n; x++) {
        for(ssize_t a = 0; a < n; a++) {
            for(ssize_t b = 0; b < n; b++) {
                clauses.push_back({-var1(x, x, a), -var1(a, x, b), -var1(x, b, x), var1(x, x, x)});
            }
        }
    }

    /**
     * Equation 677 implies that $x = xx$ if $x = (x(xx))x$.
     * The converse is also true, but it is already covered by the uniqueness of the true-valued
     * variable. */
    for(ssize_t x = 0; x < n; x++) {
        for(ssize_t a = 0; a < n; a++) {
            for(ssize_t b = 0; b < n; b++) {
                clauses.push_back({-var1(x, x, a), -var1(x, a, b), -var1(b, x, x), var1(x, x, x)});
            }
        }
    }

    /** Equation 677 implies that $y = (xx)x$ if $x = yx$. */
    for(ssize_t x = 0; x < n; x++) {
        for(ssize_t y = 0; y < n; y++) {
            for(ssize_t a = 0; a < n; a++) {
                clauses.push_back({-var1(y, x, x), -var1(x, x, a), var1(a, x, y)});
            }
        }
    }

    /** Equation 677 implies that, for each $x$, there is at most one $y$ which $x = yx$. */
    for(ssize_t x = 0; x < n; x++) {
        for(ssize_t a = 0; a < n; a++) {
            for(ssize_t b = a + 1; b < n; b++) {
                clauses.push_back({-var1(a, x, x), -var1(b, x, x)});
            }
        }
    }

    /**
     * Equation 677 implies that $y = (xy)(yx)$ if $xy = y*x$,
     * where $z = x*y$ is equivalent to $zx = y$. */
    for(ssize_t x = 0; x < n; x++) {
        for(ssize_t y = 0; y < n; y++) {
            for(ssize_t a = 0; a < n; a++) {
                for(ssize_t b = 0; b < n; b++) {
                    clauses.push_back(
                      {-var1(x, y, a), -var1(x, a, y), -var1(y, x, b), var1(a, b, y)});
                }
            }
        }
    }

    /** Condition for 677 magma end */

    // DIMACS CNF Generation start
    std::cout << "p cnf " << variables_count << ' ' << clauses.size() << '\n';
    for(auto const &clause: clauses) {
        for(auto const lit: clause) { std::cout << lit << ' '; }
        std::cout << "0\n";
    }

    return 0;
}
