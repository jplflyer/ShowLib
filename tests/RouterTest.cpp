#include "RouterTest.h"

using std::string;
using std::cout;
using std::endl;

CPPUNIT_TEST_SUITE_REGISTRATION(RouterTest);
string TEST_NAME = "RouterTest";

void RouterTest::testBasic()
{
}

void RouterTest::testRegex()
{
    Route route("GET", "/authors/\\d", "Get authors", [](HTTPServerRequest &, HTTPServerResponse &){} );
    std::smatch sm;

    cout << "Mathes PUT /authors/10: " << route.matches("PUT", "/authors/10", sm);
    cout << "Mathes GET /authors: " << route.matches("GET", "/authors", sm);
    cout << "Mathes GET /authors/10: " << route.matches("GET", "/authors/10", sm);
}
