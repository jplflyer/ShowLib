#pragma once

#include <string>
#include <vector>

namespace ShowLib {
    class ArgumentVector;
}

class ShowLib::ArgumentVector
{
public:
    using ArgumentPair = std::pair<std::string, std::string>;

    ArgumentVector();

    ArgumentPair &append(const std::string &name, const std::string &value);
    auto begin() { return vector.begin(); }
    auto end() { return vector.end(); }
    ArgumentPair & operator[](size_t index) { return vector[index]; }
    ArgumentPair * operator[](const std::string &);

private:
    std::vector<ArgumentPair> vector;
};

