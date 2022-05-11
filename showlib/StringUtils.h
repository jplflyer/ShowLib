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

    //======================================================================
    // Various predicates.
    //======================================================================
    bool startsWith(const std::string & str, const std::string & lookFor);
    bool endsWith(const std::string & str, const std::string & lookFor);

    bool allDigits(const std::string &);

    //======================================================================
    // Data conversion.
    //======================================================================
    long stol(const std::string &);

    //======================================================================
    // For trimming strings.
    //======================================================================

    /** Left trim (in place) spaces. */
    std::string & trimLeft(std::string & str);

    /** Right trim (in place) spaces. */
    std::string & trimRight(std::string & str);

    /** Trim (copy) spaces. */
    std::string trim(std::string str);

    /** Trim (in place) spaces. */
    std::string & trimInPlace(std::string & str);

    /**
     * This returns a substring of str after the first occurence of after.
     * If after is not contained in str, then it returns all of str.
     */
    std::string substringAfter(const std::string &str, const std::string &after);

    /**
     * This returns a substring of str after the last occurence of after.
     * If after is not contained in str, then it returns all of str.
     */
    std::string substringAfterLast(const std::string &str, const std::string &after);

    std::string substringUntil(const std::string &str, const std::string &upUntil);
    std::string substringUntilLast(const std::string &str, const std::string &upUntil);

    /** If this string ends in this tail, trim it. For files, this removes a particular extension. */
    std::string trimTail(const std::string &str, const std::string &tail);

    //======================================================================
    // Miscellaneous.
    //======================================================================
    std::string getEnv(const std::string &, const std::string &defValue = "");
}

/** This makes 2_sz a size_t. */
constexpr std::size_t operator "" _sz ( unsigned long long n ) { return n; }
