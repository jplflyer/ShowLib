#pragma once

#include <showlib/StringUtils.h>
#include "UnitTesting.h"

/**
 * Unit testing of StringUtils.h.
 */
class TestStrings: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestStrings);
    CPPUNIT_TEST(testBasic);
    CPPUNIT_TEST(testLonger);
    CPPUNIT_TEST(testParens);
    CPPUNIT_TEST(testConversion);
    CPPUNIT_TEST(testCamel);
    CPPUNIT_TEST(testFirstUpper);
    CPPUNIT_TEST_SUITE_END();

public:
    void testBasic();
    void testLonger();
    void testParens();
    void testConversion();
    void testCamel();
    void testFirstUpper();
};

