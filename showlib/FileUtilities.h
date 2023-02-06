#pragma once

#include <string>

#include "StringVector.h"

namespace ShowLib {
    namespace FileUtilities {
        std::string readFile(const std::string &filename);
        StringVector readLines(const std::string &filename, bool doTrim = false);
    }
}
