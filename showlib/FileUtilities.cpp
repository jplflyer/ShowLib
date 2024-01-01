#include <cstdio>
#include <fstream>

#include "CommonUsing.h"
#include "FileUtilities.h"
#include "StringUtils.h"

/**
 * Read file contents.
 */
string
ShowLib::FileUtilities::readFile(const string &filename)
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
ShowLib::StringVector ShowLib::FileUtilities::readLines(const std::string &filename, bool doTrim) {
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

/**
 * This makes sure this path exists and creates it if not. Note that if the final name
 * exists but is the wrong type, we don't do anything to fix it.
 */
void ShowLib::FileUtilities::ensurePath(const std::string &dirName) {
    std::filesystem::path path{ dirName };
    if ( !std::filesystem::exists(path)) {
        std::filesystem::current_path(path);
    }
}

/**
 * If the contents of tempName are different than origName, copy tempName on top of origName.
 * If they're the same, just remove tempName.
 */
void ShowLib::FileUtilities::moveIfDifferences(const std::string &origName, const std::string tempName) {
    string origData = readFile(origName);
    string tempData = readFile(tempName);

    if ( origData != tempData ) {
        std::rename(tempName.c_str(), origName.c_str());
    }
    else {
        std::remove(tempName.c_str());
    }
}
