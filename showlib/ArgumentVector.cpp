#include "ArgumentVector.h"

using namespace ShowLib;
using ArgumentPair = ArgumentVector::ArgumentPair;

/**
 * Constructor.
 */
ArgumentVector::ArgumentVector()
{
}

/**
 * Append to the vector.
 */
ArgumentPair &
ArgumentVector::append(const std::string &name, const std::string &value) {
    ArgumentPair & pair = vector.emplace_back();
    pair.first = name;
    pair.second = value;

    return pair;
}

/**
 * Search for an argument with this name.
 */
ArgumentPair *
ArgumentVector::operator[](const std::string &name) {
    for (ArgumentPair &pair: vector) {
        if (pair.first == name) {
            return &pair;
        }
    }
    return nullptr;
}
