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
    CPPUNIT_TEST(testWhitespace);
    CPPUNIT_TEST(testLonger);
    CPPUNIT_TEST(testParens);
    CPPUNIT_TEST(testConversion);
    CPPUNIT_TEST(testCamel);
    CPPUNIT_TEST(testFirstUpper);
    CPPUNIT_TEST(testRemove);
    CPPUNIT_TEST(testAdd);
    CPPUNIT_TEST(testSubstringAfter);
    CPPUNIT_TEST(testSubstringUntil);
    CPPUNIT_TEST(testTrimTail);
    CPPUNIT_TEST_SUITE_END();

public:
    void testBasic();
    void testWhitespace();
    void testLonger();
    void testParens();
    void testConversion();
    void testCamel();
    void testFirstUpper();
    void testRemove();
    void testAdd();
    void testSubstringAfter();
    void testSubstringUntil();
    void testTrimTail();
};

