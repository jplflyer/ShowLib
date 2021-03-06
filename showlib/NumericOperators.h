#pragma once

#include <string>

namespace ShowLib {
    inline bool inSizeRange(int value, size_t maxValue) {
        return value >= 0 && static_cast<size_t>(value) < maxValue;
    }

    template <class T>
    inline bool inSizeRange(int value, std::vector<T> vec) {
        return value >= 0 && static_cast<size_t>(value) < vec.size();
    }
}
