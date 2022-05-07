#pragma once

#include "UnitTesting.h"

class TestRegex: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestRegex);
    CPPUNIT_TEST(testBasics);
    CPPUNIT_TEST_SUITE_END();

public:
    void testBasics();
};

