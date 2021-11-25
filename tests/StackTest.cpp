#include "showlib/Stack.h"

#include "StackTest.h"

using std::string;

CPPUNIT_TEST_SUITE_REGISTRATION(StackTest);

string TEST_NAME = "TestJSON";

typedef std::shared_ptr<string> SPointer;

SPointer sString(const string &str) {
    return std::make_shared<string>(str);
}

/**
 * A basic test.
 */
void StackTest::testBasics() {
    ShowLib::Stack<string> stack;

    CPPUNIT_ASSERT(stack.empty());
    CPPUNIT_ASSERT_SIZE_T_EQUAL("Expected empty at start", 0, stack.size());

    stack.push(sString("Hello"))
         .push(sString("World"));

    CPPUNIT_ASSERT(!stack.empty());
    CPPUNIT_ASSERT_SIZE_T_EQUAL("Expected length 2", 2, stack.size());

    SPointer s1 = stack.pop();
    CPPUNIT_ASSERT(!stack.empty());
    CPPUNIT_ASSERT_SIZE_T_EQUAL("Expected length 1 after 1 pop", 1, stack.size());
    CPPUNIT_ASSERT(s1 != nullptr);
    CPPUNIT_ASSERT_EQUAL(string{"World"}, *s1);

    SPointer s2 = stack.pop();
    CPPUNIT_ASSERT(stack.empty());
    CPPUNIT_ASSERT_SIZE_T_EQUAL("Expected empty at end", 0, stack.size());
    CPPUNIT_ASSERT(s2 != nullptr);
    CPPUNIT_ASSERT_EQUAL(string{"Hello"}, *s2);

    SPointer s3 = stack.pop();
    CPPUNIT_ASSERT(s3 == nullptr);
}
