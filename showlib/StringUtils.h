#pragma once

#include <string>
#include <utility>
#include <vector>

namespace ShowLib {

std::pair<std::string, std::string> splitPair(const std::string &, const std::string & atStr);

std::vector<std::string> split(const std::string &, const std::string & atStr);
std::vector<std::string> splitWithParens(const std::string &, const std::string & atChar);

std::string toLower(const std::string &);
std::string toUpper(const std::string &);

std::string camelToLower(const std::string &);

std::string firstUpper(const std::string &);

long stol(const std::string &);
}
