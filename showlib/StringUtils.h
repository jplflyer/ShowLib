#pragma once

#include <string>
#include <utility>
#include <vector>

namespace ShowLib {

std::pair<std::string, std::string> splitPair(const std::string &, const std::string & atStr);
std::pair<std::string, std::string> splitAtWhitespace(const std::string &);

std::vector<std::string> split(const std::string &, const std::string & atStr);
std::vector<std::string> splitWithParens(const std::string &, const std::string & atChar);

std::string toLower(const std::string &);
std::string toUpper(const std::string &);

std::string camelToLower(const std::string &);

std::string firstUpper(const std::string &);
std::string firstLower(const std::string &);

bool startsWith(const std::string & str, const std::string & lookFor);
bool endsWith(const std::string & str, const std::string & lookFor);

bool allDigits(const std::string &);

long stol(const std::string &);

/** Left trim (in place) spaces. */
std::string & trimLeft(std::string & str);

/** Right trim (in place) spaces. */
std::string & trimRight(std::string & str);

/** Trim (copy) spaces. */
std::string trim(std::string str);

/** Trim (in place) spaces. */
std::string & trimInPlace(std::string & str);

}
