#include <stdio.h>
#include <iostream>
#include <fstream>

#include "FileUtilities.h"
#include "StringUtils.h"

using std::string;

using namespace ShowLib;

/**
 * Read file contents.
 */
string
FileUtilities::readFile(const string &filename)
{
	std::ifstream ifs(filename);
    if (!ifs.good()) {
        return "";
    }

	return string( (std::istreambuf_iterator<char>(ifs) ), (std::istreambuf_iterator<char>() ) );
}

/**
 * Read lines from the file, returning one line per entry in the vector.
 */
StringVector readLines(const std::string &filename, bool doTrim) {
    StringVector vec;
    std::ifstream ifs(filename);
    string line;

    while (getline(ifs, line)) {
        if (doTrim) {
            trimInPlace(line);
        }
        vec.add(line);
    }

    return vec;
}
