#pragma once
#include "UnitTesting.h"

class TestVectorUtilities: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestVectorUtilities);
    CPPUNIT_TEST(testRemoveIf);
    CPPUNIT_TEST(testWeakPtrs);
    CPPUNIT_TEST(testIndexOf);
    CPPUNIT_TEST_SUITE_END();

public:
    TestVectorUtilities();

    void testRemoveIf();
    void testWeakPtrs();
    void testIndexOf();
};

