#include <iostream>
#include <showlib/VectorUtilities.h>
#include "TestVectorUtilities.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestVectorUtilities);

using namespace ShowLib;
using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::shared_ptr;
using std::weak_ptr;

string TEST_NAME = "TestStrings";

TestVectorUtilities::TestVectorUtilities()
{
}

/**
 * Test removeIf.
 */
void
TestVectorUtilities::testRemoveIf()
{
    vector<int> vec { 1, 2, 3, 4, 5 };

    CPPUNIT_ASSERT_SIZE_T_EQUAL("Unexpected initial size", 5, vec.size());
    eraseIf(vec, [](int & it) { return it == 3; } );
    CPPUNIT_ASSERT_SIZE_T_EQUAL("Unexpected final size", 4, vec.size());

}

/**
 * Test cleaning up weak pointers.
 */
void
TestVectorUtilities::testWeakPtrs()
{
    vector<weak_ptr<string>> vec;
    shared_ptr<string> s1 = std::make_shared<string>("Hello");
    shared_ptr<string> s2 = std::make_shared<string>("there");
    shared_ptr<string> s3 = std::make_shared<string>("you");
    shared_ptr<string> s4 = std::make_shared<string>("lovely");
    shared_ptr<string> s5 = std::make_shared<string>("friend.");

    vec.push_back(s1);
    vec.push_back(s2);
    vec.push_back(s3);
    vec.push_back(s4);
    vec.push_back(s5);

    s3 = nullptr;

    CPPUNIT_ASSERT_SIZE_T_EQUAL("Unexpected initial size", 5, vec.size());
    removeDeadLinks(vec);
    CPPUNIT_ASSERT_SIZE_T_EQUAL("Unexpected final size", 4, vec.size());
}
