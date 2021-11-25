#pragma once

#include "UnitTesting.h"

class StackTest: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(StackTest);
    CPPUNIT_TEST(testBasics);
    CPPUNIT_TEST_SUITE_END();

public:
    void testBasics();
};

