#include "TestJSON.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestJSON);

using namespace ShowLib;
using std::string;
using std::cout;
using std::endl;

string TEST_NAME = "TestJSON";

class MyExample : public JSONSerializable
{
public:
    string aStringValue;
    int anIntValue;

    void fromJSON(const JSON &json) {
        aStringValue = stringValue(json, "aStringValue");
        anIntValue = intValue(json, "anIntValue");
    }

    JSON & toJSON(JSON &json) const {
        json["aStringValue"] = aStringValue;
        json["anIntValue"] = anIntValue;

        return json;
    }

};

void
TestJSON::testBasic() {
    MyExample example;
    example.aStringValue = "Hello World";
    example.anIntValue = 1234;

    JSON json = example.getJSON();

    MyExample outExample;

    outExample.fromJSON(json);

    CPPUNIT_ASSERT_EQUAL(example.aStringValue, outExample.aStringValue);
    CPPUNIT_ASSERT_EQUAL(example.anIntValue, outExample.anIntValue);
}
