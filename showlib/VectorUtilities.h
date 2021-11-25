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
 * This adds this object if the predicate returns false for every object.
 * This is useful as an add-if-unique if the predicate tests for you.
 */
template <typename T, class UnaryPredicate>
bool addIfNot(std::vector<T> & vec, const T & obj, UnaryPredicate f) {
    for (const T &testObj: vec) {
        if (f(testObj)) {
            return false;
        }
    }
    vec.push_back(obj);
    return true;
}

/**
 * This removes dead links from a vector of weak_ptr. The caller
 * is responsible for thread safety.
 */
template <typename T>
void removeDeadLinks(std::vector<std::weak_ptr<T>> & vec) {
    eraseIf(vec, [](auto it){ return it.expired(); } );
}

/**
 * Return the index for the first object that matches the predicate.
 * Returns a sane integer if found, SIZE_T_MAX if not found.
 */
template <typename T, class UnaryPredicate>
size_t indexOf(std::vector<T> & vec, UnaryPredicate f) {
    int count = vec.size();
    for (int index = 0; index < count; ++index) {
        if (f(vec[index])) {
            return index;
        }
    }

    return SIZE_MAX;
}

}
