#ifndef MAGMA_TOOLS_HPP
#define MAGMA_TOOLS_HPP

#include "types.hpp"

#include <algorithm>
#include <vector>

namespace magma {
    inline void remove_duplicates(std::vector<std::vector<ssize>> &v) {
        for(auto &cl: v) { std::sort(cl.begin(), cl.end()); }
        std::sort(v.begin(), v.end());
        v.erase(std::unique(v.begin(), v.end()), v.end());
    }
}  // namespace magma

#endif  // MAGMA_TOOLS_HPP
