#include <iostream>
#include <iomanip>
#include <regex>

#include <showlib/CommonUsing.h>

#include "TestRegex.h"
string TEST_NAME = "TestRegex";

CPPUNIT_TEST_SUITE_REGISTRATION(TestRegex);

void tryIt(const std::string &reStr, std::regex &re, const string &path) {
    std::smatch sm;
    bool matches = std::regex_match(path, sm, re);

    cout << std::boolalpha;
    cout << std::setw(20) << std::left << reStr
         << " With " << std::setw(20) << std::left << path
         << " == " << matches
         << " with match-count: " << sm.size()
         << endl;

    if (matches) {
        for (const auto &sub: sm) {
            cout << "SubMatch: " << sub.str() << endl;
        }
    }
}

void TestRegex::testBasics()
{
    string re1Str = "/authors(/(\\d*))?";
    string re2Str = "/authors/((\\d*))";
    std::regex re1(re1Str, std::regex_constants::extended);
    std::regex re2(re2Str, std::regex_constants::extended);

    string path1 = "/authors";
    string path2 = "/authors/";
    string path3 = "/authors/10";

    tryIt(re1Str, re1, path1);
    tryIt(re1Str, re1, path2);
    tryIt(re1Str, re1, path3);

    tryIt(re2Str, re2, path1);
    tryIt(re2Str, re2, path2);
    tryIt(re2Str, re2, path3);

    string re3Str = "/authors/((\\d+))/universes(/(\\d*))?";
    std::regex re3(re3Str, std::regex_constants::extended);

    string path4 = "/authors/10/universes";
    tryIt(re3Str, re3, path4);
}
