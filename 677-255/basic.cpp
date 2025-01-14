#include <iostream>
#include <vector>

int main(int argc, char *argv[]) {
    using ssize_t = long long;

    if(argc != 2) {
        std::cerr << "Usage: " << argv[0] << " [Number of elements]" << std::endl;
        return 1;
    }

    ssize_t n;
    try {
        n = std::stoll(argv[1]);
        if(n < 5) { throw std::invalid_argument("Number of elements must not be less than 5."); }
        if(n > 1'000'000) { throw std::invalid_argument("Number of elements is too large."); }
    } catch(std::exception const &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    std::vector<std::vector<ssize_t>> clauses;
    /** `var1(x, y, i)` is $X_{yx, i}$. */
    auto const var1 = [n](ssize_t const x, ssize_t const y, ssize_t const i) noexcept {
        return 1 + (x * n + y) * n + i;
    };
    /** `var2(x, y, i)` is $X_{(yx)y, i}$. */
    auto const var2 = [n](ssize_t const x, ssize_t const y, ssize_t const i) noexcept {
        return 1 + n * n * n + (x * n + y) * n + i;
    };
    /** `var3(x, y, i)` is $X_{x((yx)y), i}$. */
    auto const var3 = [n](ssize_t const x, ssize_t const y, ssize_t const i) noexcept {
        return 1 + 2 * n * n * n + (x * n + y) * n + i;
    };
    /** `var4(x, i)` is $X_{((xx)x)x, i}$. */
    auto const var4 = [n](ssize_t const x, ssize_t const i) noexcept {
        return 1 + 3 * n * n * n + x * n + i;
    };
    /**
     * `var5(x)` is equivalent to
     * $\neg (X_{x \cdot 0, 0} \lor \ldots \lor X_{x \cdot (n - 1), n - 1})$. */
    auto const var5 = [n](ssize_t const x) noexcept { return 1 + 3 * n * n * n + n * n + x; };

    /** Condition for 677 magma */

    /** For every $x$ and $y$, exactly one of $X_{yx, i}$ for $0 \leq i < N$ is true. */
    for(ssize_t x = 0; x < n; x++) {
        for(ssize_t y = 0; y < n; y++) {
            /**
             * For each distinct pair $X_{yx, i}$ and $X_{yx, j}$ of variables,
             * there is at least one false variable. */
            for(ssize_t i = 0; i < n; i++) {
                for(ssize_t j = i + 1; j < n; j++) {
                    clauses.push_back({-var1(x, y, i), -var1(x, y, j)});
                }
            }

            /** There is at least one true $X_{yx, i}$. */
            std::vector<ssize_t> clause;
            clause.reserve(n);
            for(ssize_t i = 0; i < n; i++) { clause.push_back(var1(x, y, i)); }
            clauses.push_back(std::move(clause));
        }
    }

    /** For every $x$ and $y$, exactly one of $X_{(yx)y, i}$ for $0 \leq i < N$ is true. */
    for(ssize_t x = 0; x < n; x++) {
        for(ssize_t y = 0; y < n; y++) {
            /**
             * For each distinct pair $X_{(yx)y, i}$ and $X_{(yx)y, j}$ of variables,
             * there is at least one false variable. */
            for(ssize_t i = 0; i < n; i++) {
                for(ssize_t j = i + 1; j < n; j++) {
                    clauses.push_back({-var2(x, y, i), -var2(x, y, j)});
                }
            }

            /** There is at least one true $X_{(yx)y, i}$. */
            std::vector<ssize_t> clause;
            clause.reserve(n);
            for(ssize_t i = 0; i < n; i++) { clause.push_back(var2(x, y, i)); }
            clauses.push_back(std::move(clause));
        }
    }

    /**
     * For every $x$, $y$, and $i$, $X_{(yx)y, i}$ is true if and only if there is some $j$ which
     * $X_{yx, j}$ and $X_{jy, i}$ are true.
     *
     * As we assumed there exists a unique $i$ which makes $X_{yx, j}$ true, we may simplify the
     * proposition as follows.
     *
     * For every $x$, $y$, and $i$, $X_{(yx)y, i}$ is true if and only if for every $j$ which
     * $X_{yx, j}$ is true, $X_{jy, i}$ is also true. */
    for(ssize_t x = 0; x < n; x++) {
        for(ssize_t y = 0; y < n; y++) {
            for(ssize_t i = 0; i < n; i++) {
                for(ssize_t j = 0; j < n; j++) {
                    clauses.push_back({var1(y, j, i), -var1(x, y, j), -var2(x, y, i)});
                    clauses.push_back({var2(x, y, i), -var1(x, y, j), -var1(y, j, i)});
                }
            }
        }
    }

    /** For every $x$ and $y$, exactly one of $X_{x((yx)y), i}$ for $0 \leq i < N$ is true. */
    for(ssize_t x = 0; x < n; x++) {
        for(ssize_t y = 0; y < n; y++) {
            /**
             * For each distinct pair $X_{x((yx)y), i}$ and $X_{x((yx)y), j}$ of variables,
             * there is at least one false variable. */
            for(ssize_t i = 0; i < n; i++) {
                for(ssize_t j = i + 1; j < n; j++) {
                    clauses.push_back({-var3(x, y, i), -var3(x, y, j)});
                }
            }

            /** There is at least one true $X_{x((yx)y), i}$. */
            std::vector<ssize_t> clause;
            clause.reserve(n);
            for(ssize_t i = 0; i < n; i++) { clause.push_back(var3(x, y, i)); }
            clauses.push_back(std::move(clause));
        }
    }

    /**
     * For every $x$, $y$, and $i$, $X_{x((yx)y), i}$ is true if and only if there is some $j$ which
     * $X_{(yx)y, j}$ and $X_{xj, i}$ are true.
     *
     * As we assumed there exists a unique $i$ which makes $X_{(yx)y, i}$ true, we may simplify the
     * proposition as follows.
     *
     * For every $x$, $y$, and $i$, $X_{x((yx)y), i}$ is true if and only if for every $j$ which
     * $X_{(yx)y, j}$ is true, $X_{xj, i}$ is also true. */
    for(ssize_t x = 0; x < n; x++) {
        for(ssize_t y = 0; y < n; y++) {
            for(ssize_t i = 0; i < n; i++) {
                for(ssize_t j = 0; j < n; j++) {
                    clauses.push_back({var1(j, x, i), -var2(x, y, j), -var3(x, y, i)});
                    clauses.push_back({var3(x, y, i), -var2(x, y, j), -var1(j, x, i)});
                }
            }
        }
    }

    /** For every $x$, $y$, and $i$, if $X_{x((yx)y), i}$ is true, then $X_{yi, x}$ is also true. */
    for(ssize_t x = 0; x < n; x++) {
        for(ssize_t y = 0; y < n; y++) {
            for(ssize_t i = 0; i < n; i++) { clauses.push_back({var1(i, y, x), -var3(x, y, i)}); }
        }
    }

    /** For every x, $X_{(xx)x, x} is true if and only if $X_{xx, x}$ is also true. */
    for(ssize_t x = 0; x < n; x++) {
        clauses.push_back({-var2(x, x, x), var1(x, x, x)});
        clauses.push_back({-var1(x, x, x), var2(x, x, x)});
    }

    /** For every x, $X_{x(xx), x}$ is true if and only if $X_{xx, x}$ is true. */
    for(ssize_t x = 0; x < n; x++) {
        for(ssize_t i = 0; i < n; i++) {
            clauses.push_back({var1(x, x, x), -var1(x, x, i), -var1(i, x, x)});
            clauses.push_back({var1(i, x, x), -var1(x, x, i), -var1(x, x, x)});
        }
    }

    /** For every $x$, $X_{x((xx)x), x}$ is true if and only if $X_{xx, x}$ is true. */
    for(ssize_t x = 0; x < n; x++) {
        clauses.push_back({-var3(x, x, x), var1(x, x, x)});
        clauses.push_back({-var1(x, x, x), var3(x, x, x)});
    }

    /** Condition for 677 magma end */

    /** Condition for 255 magma */

    /** For every $x$, exactly one of $X_{((xx)x)x, i}$ for $0 \leq i < n$ is true. */
    for(ssize_t x = 0; x < n; x++) {
        /**
         * For each distinct pair $X_{((xx)x)x, i}$ and $X_{((xx)x)x, j}$ of variables,
         * there is at least one false variable. */
        for(ssize_t i = 0; i < n; i++) {
            for(ssize_t j = i + 1; j < n; j++) { clauses.push_back({-var4(x, i), -var4(x, j)}); }
        }

        /** There is at least one true $X_{((xx)x)x, i}$. */
        std::vector<ssize_t> clause;
        clause.reserve(n);
        for(ssize_t i = 0; i < n; i++) { clause.push_back(var4(x, i)); }
        clauses.push_back(std::move(clause));
    }

    /**
     * For every $x$ and $i$, $X_{((xx)x)x, i}$ is true if and only if there is some $j$ which
     * $X_{(xx)x, j}$ and $X_{jx, i}$ are true.
     *
     * As we assumed there exists a unique $i$ which makes $X_{(xx)x, i}$ true, we may simplify the
     * proposition as follows.
     *
     * For every $x$ and $i$, $X_{((xx)x)x, i}$ is true if and only if for every $j$ which
     * $X_{(xx)x, j}$ is true, $X_{jx, i}$ is also true. */
    for(ssize_t x = 0; x < n; x++) {
        for(ssize_t i = 0; i < n; i++) {
            for(ssize_t j = 0; j < n; j++) {
                clauses.push_back({var1(x, j, i), -var2(x, x, j), -var4(x, i)});
                clauses.push_back({var4(x, i), -var2(x, x, j), -var1(x, j, i)});
            }
        }
    }

    /**
     * We want to find a case when the equation 677 (x = y(x((yx)y))) is satisfied but the equation
     * 255 (x = ((xx)x)x) is not.
     *
     * If there is some $x$ which $X_{((xx)x)x, x}$ is false, then we are done. */
    {
        std::vector<ssize_t> clause;
        clause.reserve(n);
        for(ssize_t x = 0; x < n; x++) { clause.push_back(-var4(x, x)); }
        clauses.push_back(std::move(clause));
    }

    /**
     * Assuming equation 677 holds, equation 255 is equivalent to
     * the existence of some $y$ for every $x$, which $yx = x$.
     * To refute equation 255, we introduce a new variable $Y_{x}$, which is equivalent to
     * $\neg (X_{x \cdot 0, 0} \lor \ldots \lor X_{x \cdot (n - 1), n - 1})$. */
    {
        for(ssize_t x = 0; x < n; x++) {
            std::vector<ssize_t> clause;
            clause.reserve(n + 1);
            clause.push_back(-var5(x));
            for(ssize_t i = 0; i < n; i++) {
                clause.push_back(var1(i, x, i));
                clauses.push_back({var5(x), -var1(x, i, x)});
            }
            clauses.push_back(std::move(clause));
        }

        std::vector<ssize_t> clause;
        clause.reserve(n);
        for(ssize_t x = 0; x < n; x++) { clause.push_back(-var5(x)); }
        clauses.push_back(std::move(clause));
    }

    /** Condition for 255 magma end */

    /** Symmetry breaking conditions */

    /**
     * As we are trying to refute 255, there should be some $x$ which $x \neq ((xx)x)x$.
     * This can happen only if there is some $x$ which $x \neq xx$.
     * So let $0 \cdot 0 = 1$ .
     * We may also set $(0 \cdot 0) \cdot 0 = 2$, as it is not $1$.
     * These constraints also implies that
     * * $1 \cdot 0 = 2$
     * * $0 \cdot 1 \neq 0$
     * * $0 \cdot ((0 \cdot 0) \cdot 0) \neq 0$
     * * $0 \cdot 2 \neq 0$ */
    {
        clauses.push_back({var1(0, 0, 1)});
        clauses.push_back({var2(0, 0, 2)});
        clauses.push_back({var1(0, 1, 2)});
        clauses.push_back({-var1(1, 0, 0)});
        clauses.push_back({-var3(0, 0, 0)});
        clauses.push_back({-var1(2, 0, 0)});
    }

    /** Symmetry breaking conditions end */

    // DIMACS CNF Generation start
    std::cout << "p cnf " << 3 * n * n * n + n * n + n << ' ' << clauses.size() << '\n';
    for(auto const &clause: clauses) {
        for(auto const lit: clause) { std::cout << lit << ' '; }
        std::cout << "0\n";
    }
}
