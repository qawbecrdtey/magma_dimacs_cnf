#ifndef MAGMA_DIMACS_CNF_SUM_HPP
#define MAGMA_DIMACS_CNF_SUM_HPP

#include <magma_dimacs_cnf/assert_and_assume.hpp>

#include <cstdint>
#include <vector>

namespace magma_dimacs_cnf {
    static constexpr std::int32_t sum(std::vector<std::int32_t> const &v_) noexcept {
        std::int32_t res {0};
        for(auto const &val: v_) {
            ASSERT_AND_ASSUME(0 < val);
            ASSERT_AND_ASSUME(
              static_cast<std::int64_t>(val) + static_cast<std::int64_t>(res)
              < static_cast<std::int64_t>(std::numeric_limits<std::int32_t>::max()));
            res += val;
        }
        return res;
    }
}  // namespace magma_dimacs_cnf

#endif
