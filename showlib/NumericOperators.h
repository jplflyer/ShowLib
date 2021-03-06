#pragma once

#include <string>

namespace ShowLib {
    inline bool inSizeRange(int value, size_t maxValue) { return value >= 0 && static_cast<size_t>(value) < maxValue; }
}
