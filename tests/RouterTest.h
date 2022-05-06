#pragma once

#include <showlib/Router.h>

#include "UnitTesting.h"

class RouterTest: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(RouterTest);
    CPPUNIT_TEST(testBasic);
    CPPUNIT_TEST(testRegex);
    CPPUNIT_TEST_SUITE_END();

public:
    void testBasic();
    void testRegex();
};

