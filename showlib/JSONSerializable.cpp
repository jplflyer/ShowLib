#include "JSONSerializable.h"

using namespace ShowLib;

using std::string;

bool JSONSerializable::hasKey(const JSON &json, const std::string &key) {
    auto it = json.find(key);
    return it != json.end();
}

/**
 * Return ourself as a JSON string.
 */
std::string
JSONSerializable::toString() const {
    return toJSON().dump();
}

/**
 * Return ourself as a JSON string.
 */
std::string
JSONSerializable::toString(int indent) const {
    return toJSON().dump(indent);
}

/**
 * Output to a stream.
 */
std::ostream &
ShowLib::operator<<(std::ostream &output, const JSONSerializable &obj) {
    output << obj.toString(2);
    return output;
}

/**
 * Get this key's value as a string.
 *
 * @returns The object in string representation or an empty string if the key isn't found.
 */
std::string
JSONSerializable::stringValue(const JSON &json, const std::string &key, const std::string &defaultValue)
{
    auto it = json.find(key);
    if (it != json.end()) {
        JSON value = *it;
        if (value.is_null()) {
            return "";
        }
        else if (value.is_string()) {
            return value.get<string>();
        }
        else {
            return value.dump();
        }
    }
    return defaultValue;
}



/**
 * Get this key's value as an int. May throw an exception if the key is found but
 * it's a non-integer value.
 *
 * @returns the value or defaultValue if not found.
 */
int
JSONSerializable::intValue(const JSON &json, const std::string &key, int defaultValue)
{
    auto it = json.find(key);
    if (it != json.end()) {
        JSON value = *it;
        if (!value.is_null()) {
            return value.get<int>();
        }
    }
    return defaultValue;
}

/**
 * Get this key's value as a long. May throw an exception if the key is found but
 * it's a non-numeric value.
 *
 * @returns the value or 0 if not found.
 */
long
JSONSerializable::longValue(const JSON &json, const std::string &key, long defaultValue)
{
    auto it = json.find(key);
    if (it != json.end()) {
        JSON value = *it;
        if (!value.is_null()) {
            return value.get<long>();
        }
    }
    return defaultValue;
}

/**
 * Get this key's value as a double. May throw an exception if the key is found but
 * it's a non-floating value.
 *
 * @returns the value or 0.0 if not found.
 */
double
JSONSerializable::doubleValue(const JSON &json, const std::string &key, double defaultValue)
{
    auto it = json.find(key);
    if (it != json.end()) {
        JSON value = *it;
        if (!value.is_null()) {
            return value.get<double>();
        }
    }
    return defaultValue;
}

/**
 * Get this key's value as a boolean. May throw an exception if the key is found but
 * it's a non-boolean value.
 *
 * @returns the value or false if not found.
 */
bool JSONSerializable::boolValue(const JSON &json, const std::string &key, bool defaultValue)
{
    auto it = json.find(key);
    if (it != json.end()) {
        JSON value = *it;
        if (!value.is_null()) {
            return value.get<bool>();
        }
    }
    return defaultValue;
}

/**
 * Find this key and return the corresponding JSON object.
 *
 * @returns the value or an empty JSON object.
 */
JSON JSONSerializable::jsonValue(const JSON &json, const std::string &key) {
    auto it = json.find(key);
    if (it != json.end()) {
        return *it;
    }
    return JSON::object();
}

/**
 * Find this key and return the corresponding JSON array.
 *
 * @returns the value or an empty JSON object.
 */
JSON JSONSerializable::jsonArray(const JSON &json, const std::string &key) {
    auto it = json.find(key);
    if (it != json.end()) {
        return *it;
    }
    return JSON::array();
}

/**
 * We expect this vector to be an array of integer values.
 */
std::vector<int> JSONSerializable::intVectorValue(const JSON &json, const std::string &key) {
    std::vector<int> retVal;
    auto it = json.find(key);

    if (it != json.end()) {
        const JSON jsonArray = *it;
        for (auto iter = jsonArray.begin(); iter != jsonArray.end(); ++iter) {
            const JSON value = *iter;
            if (value.is_number()) {
                retVal.push_back( value.get<int>() );
            }
        }
    }

    return retVal;
}

/**
 * Sets a string value in this JSON only if we have content.
 */
void
JSONSerializable::setStringValue(JSON &json, const std::string &key, const std::string &value) {
    if (key.length() > 0 && value.length() > 0) {
        json[key] = value;
    }
}

void JSONSerializable::setJSONValue(JSON & json, const std::string &key, const JSON & jsonToAdd) {
    if (key.length() > 0 && !jsonToAdd.empty()) {
        json[key] = jsonToAdd;
    }
}

void JSONSerializable::translateAndSet(JSON & json, const std::string &key, const JSONSerializable & serializable) {
    setJSONValue(json, key, serializable.toJSON());
}


/**
 * Sets a long value in this JSON only if we have content -- value != 0. If you want to store
 * zeroes anyway, don't use this method.
 */
void
JSONSerializable::setLongValue(JSON &json, const std::string &key, long value) {
    if (key.length() > 0 && value != 0) {
        json[key] = value;
    }
}

