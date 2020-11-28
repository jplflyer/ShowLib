#include <stdio.h>
#include <iostream>
#include <fstream>

#include "FileUtilities.h"

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
