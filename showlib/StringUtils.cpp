#include <iostream>
#include <algorithm>
#include <cctype>

#include "StringUtils.h"

using std::string;

/**
 * This splits the string into two at the first instances of atStr.
 */
std::pair<std::string, std::string> ShowLib::splitPair(const std::string &str, const std::string & atStr) {
    std::pair<std::string, std::string> rv;
    size_t pos = str.find(atStr);

    if (pos == string::npos) {
        rv.first = str;
    }
    else {
        rv.first = str.substr(0, pos);
        rv.second = str.substr(pos + 1);
    }

    return rv;
}

/**
 * This splits the string at the first whitespace.
 */
std::pair<std::string, std::string> ShowLib::splitAtWhitespace(const std::string &str) {
    std::pair<std::string, std::string> rv;
    auto pos = std::find_if(str.begin(), str.end(), [](char c){ return isspace(c); });

    if (pos == str.end()) {
        rv.first = str;
    }
    else {
        rv.first = string { str.begin(), pos };
        rv.second = string { ++pos, str.end() };
    }

    return rv;
}

/**
 * Split a string into requisite parts.
 *
 * vector<string> vec = split("abc:def", ':') -- returns a vector split at the colon.
 */
std::vector<std::string> ShowLib::split(const std::string &str, const std::string & atStr) {
    std::vector<std::string> vec;
    size_t last = 0;
    size_t pos;

    while ( (pos = str.find(atStr, last)) != string::npos) {
        vec.push_back(str.substr(last, pos - last));
        last = pos + atStr.length();
    }

    // Catch the rest.
    vec.push_back(str.substr(last));

    return vec;
}

/**
 * This is for a complicated string of the form name[(delimeted-list)].
 * Will parse:
 *
 * 		varchar
 * 		foo()
 * 		varchar(10)
 * 		numeric(10,5)
 */
std::vector<std::string> ShowLib::splitWithParens(const std::string &str, const std::string & atStr) {
    std::vector<std::string> vec;

    size_t openParenPos = str.find('(');
    if (openParenPos == string::npos) {
        vec.push_back(str);
    }
    else {
        size_t closeParenPos = str.find(')', openParenPos + 1);
        if (closeParenPos == string::npos) {
            // Open paren but no close paren.
            return vec;
        }

        string inside =  str.substr(openParenPos + 1, closeParenPos - openParenPos - 1);

        vec = split(str.substr(openParenPos + 1, closeParenPos - openParenPos - 1), atStr);
        vec.insert(vec.begin(), str.substr(0, openParenPos));
    }

    return vec;
}

std::string ShowLib::toLower(const std::string &str) {
    string copy = str;
    std::transform(str.begin(), str.end(), copy.begin(), ::tolower);
    return copy;
}

std::string ShowLib::toUpper(const std::string &str) {
    string copy = str;
    std::transform(str.begin(), str.end(), copy.begin(), ::toupper);
    return copy;
}

/**
 * This is a special form of toLower that converts a string in
 * camelCase into all-lower such as camel_case. We avoid producing
 * a double underscore or introducing a leading underscore.
 */
std::string ShowLib::camelToLower(const std::string &str) {
    string retVal;
    bool lastWasUnderscore = true;

    for (char c: str) {
        if (isupper(c)) {
            if (!lastWasUnderscore) {
                retVal += '_';
            }
            retVal += ::tolower(c);
            lastWasUnderscore = true;
        }
        else {
            retVal += c;
            lastWasUnderscore = c == '_';
        }
    }

    return retVal;
}

/**
 * This returns a string with the first character forced upper.
 */
std::string ShowLib::firstUpper(const std::string &str) {
    string copy = str;
    std::transform(str.begin(), str.begin() + 1, copy.begin(), ::toupper);
    return copy;
}

/**
 * This returns a string with the first character forced lower case.
 */
std::string ShowLib::firstLower(const std::string &str) {
    string copy = str;
    std::transform(str.begin(), str.begin() + 1, copy.begin(), ::tolower);
    return copy;
}

long ShowLib::stol(const std::string &str) {
    return str.length() > 0 ? std::stol(str) : 0;
}

/**
 * Does str end with lookFor?
 */
bool ShowLib::endsWith(const string & str, const string & lookFor) {
    if (lookFor.size() > str.size()) return false;
    return std::equal(str.begin() + str.size() - lookFor.size(), str.end(), lookFor.begin());
}

bool ShowLib::startsWith(const string & str, const string & lookFor) {
    if (lookFor.size() > str.size()) return false;
    return std::equal(str.begin(), str.begin() + lookFor.size(), lookFor.begin());
}

/**
 * Does this string contain only digits?
 */
bool ShowLib::allDigits(const std::string &str) {
    for (char c: str) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

/** Left trim (in place) spaces. */
std::string & ShowLib::trimLeft(std::string & str) {
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int ch) {
            return !std::isspace(ch);
        }));

    return str;
}

/** Right trim (in place) spaces. */
std::string & ShowLib::trimRight(std::string & str) {
    str.erase(std::find_if(str.rbegin(), str.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), str.end());

    return str;
}

/** Trim (copy) spaces. */
std::string ShowLib::trim(std::string str) {
    return trimRight(trimLeft(str));
}

/** Trim (in place) spaces. */
std::string & ShowLib::trimInPlace(std::string & str) {
    return trimRight(trimLeft(str));
}
