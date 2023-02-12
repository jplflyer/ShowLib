#pragma once

#include <string>

namespace ShowLib {

template <typename FirstType, typename SecondType, typename ThirdType>
class Tuple {
public:
    FirstType first;
    SecondType second;
    ThirdType third;
};

using StringsTuple = Tuple<std::string, std::string, std::string>;

} // namespace ShowLib
