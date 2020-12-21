#include "TestStrings.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestStrings);

using namespace ShowLib;
using std::string;
using std::cout;
using std::endl;

string TEST_NAME = "TestStrings";

void
TestStrings::testBasic() {
    string str = ":abc:def:ghi:";
    std::vector<string> vec = split(str, ":");

    CPPUNIT_ASSERT_SIZE_T_EQUAL("Unexpected size", 5, vec.size());
    CPPUNIT_ASSERT_EQUAL(string{""}, vec.at(0));
    CPPUNIT_ASSERT_EQUAL(string{"abc"}, vec.at(1));
    CPPUNIT_ASSERT_EQUAL(string{"def"}, vec.at(2));
    CPPUNIT_ASSERT_EQUAL(string{"ghi"}, vec.at(3));
    CPPUNIT_ASSERT_EQUAL(string{""}, vec.at(4));
}

/**
 * This is identical to testBasic except we use a multi-character delimeter.
 */
void
TestStrings::testLonger() {
    string str = "::abc::def::ghi::";
    std::vector<string> vec = split(str, "::");

    CPPUNIT_ASSERT_SIZE_T_EQUAL("Unexpected size", 5, vec.size());
    CPPUNIT_ASSERT_EQUAL(string{""}, vec.at(0));
    CPPUNIT_ASSERT_EQUAL(string{"abc"}, vec.at(1));
    CPPUNIT_ASSERT_EQUAL(string{"def"}, vec.at(2));
    CPPUNIT_ASSERT_EQUAL(string{"ghi"}, vec.at(3));
    CPPUNIT_ASSERT_EQUAL(string{""}, vec.at(4));
}

/**
 * This tests the splitWithParens() code.
 */
void
TestStrings::testParens() {
    string strZero = "VarChar";
    string strSimple = "VarChar(10)";
    string strDouble = "Numeric(10,5)";
    string strBad = "VarChar(10";

    std::vector<string> vec = splitWithParens(strZero, ",");
    CPPUNIT_ASSERT_SIZE_T_EQUAL("Unexpected size for VarChar", 1, vec.size());
    CPPUNIT_ASSERT_EQUAL(string{"VarChar"}, vec.at(0));

    vec = splitWithParens(strSimple, ",");
    CPPUNIT_ASSERT_SIZE_T_EQUAL("Unexpected size for VarChar(10)", 2, vec.size());
    CPPUNIT_ASSERT_EQUAL(string{"VarChar"}, vec.at(0));
    CPPUNIT_ASSERT_EQUAL(string{"10"}, vec.at(1));

    vec = splitWithParens(strDouble, ",");
    CPPUNIT_ASSERT_SIZE_T_EQUAL("Unexpected size for Numeric(10,5)", 3, vec.size());
    CPPUNIT_ASSERT_EQUAL(string{"Numeric"}, vec.at(0));
    CPPUNIT_ASSERT_EQUAL(string{"10"}, vec.at(1));
    CPPUNIT_ASSERT_EQUAL(string{"5"}, vec.at(2));

    vec = splitWithParens(strBad, ",");
    CPPUNIT_ASSERT_SIZE_T_EQUAL("Unexpected size for a bad format", 0, vec.size());
}

/**
 * Tests toLower and toUpper().
 */
void
TestStrings::testConversion() {
    string str = "MixCaseString";
    string lowered = toLower(str);
    string uppered = toUpper(str);

    CPPUNIT_ASSERT_EQUAL(string{"mixcasestring"}, lowered);
    CPPUNIT_ASSERT_EQUAL(string{"MIXCASESTRING"}, uppered);
}

/**
 * Test camelToLower().
 */
void
TestStrings::testCamel() {
    CPPUNIT_ASSERT_EQUAL(string{"mix_case_string"},  camelToLower("MixCaseString"));
    CPPUNIT_ASSERT_EQUAL(string{"url"},  camelToLower("URL"));
    CPPUNIT_ASSERT_EQUAL(string{"my_url"},  camelToLower("myURL"));
}

/**
 * Test firstUpper().
 */
void
TestStrings::testFirstUpper() {
    CPPUNIT_ASSERT_EQUAL(string{"MixCaseString"},  firstUpper("mixCaseString"));
}
