#pragma once

#include <string>

#include "JSONSerializable.h"

namespace ShowLib {
class StringVector;
}

/**
 * This is a vector of string pointers that owns the contents (but we'll give them away if you ask nicely).
 *
 * On destruction or calls to erase(), any strings we still hold are deleted. This means if you have
 * lingering pointers to them, you lose.
 */
class ShowLib::StringVector: public std::vector<std::shared_ptr<std::string>>, public ShowLib::JSONSerializable
{
public:
    typedef std::shared_ptr<std::string> Pointer;
    typedef std::function<bool(Pointer)> Comparator;

	StringVector();
    StringVector(const std::string &str, const char splitter = ' ');
    virtual ~StringVector();

    void add(const std::string &value);
    void addFront(const std::string &value);
    void remove(const std::string &value);
    void tokenize(const char *str, const char *splitter = " ");
    void tokenize(const std::string &str, const char splitter = ' ');

    bool contains(const std::string &str);

    void fromJSON(const JSON &) override;
    JSON & toJSON(JSON &) const override;
    Pointer findIf(Comparator comp) const;
};
