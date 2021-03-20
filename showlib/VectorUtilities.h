#pragma once

#include <iostream>
#include <algorithm>
#include <functional>
#include <memory>
#include <vector>

namespace ShowLib {

/**
 * The sort of method that std::vector should already have.
 */
template <typename T, class UnaryPredicate>
void eraseIf(std::vector<T> & vec, UnaryPredicate f) {
    vec.erase(std::remove_if(vec.begin(), vec.end(),  f), vec.end());
}

/**
 * This removes dead links from a vector of weak_ptr. The caller
 * is responsible for thread safety.
 */
template <typename T>
void removeDeadLinks(std::vector<std::weak_ptr<T>> & vec) {
    eraseIf(vec, [](auto it){ return it.expired(); } );
}

}
