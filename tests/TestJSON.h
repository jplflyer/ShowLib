#pragma once

#include <showlib/JSONSerializable.h>
#include "UnitTesting.h"

/**
 * Unit testing of JSONSerializable.
 */
class TestJSON: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestJSON);
    CPPUNIT_TEST(testBasic);
    CPPUNIT_TEST_SUITE_END();

public:
    void testBasic();
};

