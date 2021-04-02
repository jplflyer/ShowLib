#pragma once
#include "UnitTesting.h"

class TestVectorUtilities: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestVectorUtilities);
    CPPUNIT_TEST(testEraseIf);
    CPPUNIT_TEST(testWeakPtrs);
    CPPUNIT_TEST(testIndexOf);
    CPPUNIT_TEST(testAddIfNot);
    CPPUNIT_TEST_SUITE_END();

public:
    TestVectorUtilities();

    void testEraseIf();
    void testWeakPtrs();
    void testIndexOf();
    void testAddIfNot();
};

