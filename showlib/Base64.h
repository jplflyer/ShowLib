#pragma once
#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <cstdint>

namespace Base64
{
    std::string encode(const std::vector<std::uint8_t>& input);
    std::string encode(const std::string & input);
    std::vector<std::uint8_t> decode(const std::string& input);
    std::string decodeToString(const std::string& input);
}
