#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <nlohmann/json.hpp>

using JSON = nlohmann::json;

namespace ShowLib {

/**
 * Treat this like an interface.
 */
class JSONSerializable {
public:
    virtual void fromJSON(const JSON &) = 0;
    virtual JSON & toJSON(JSON &) const = 0;

    JSON getJSON() const;

    std::string toString() const;
    std::string toString(int indent) const;

    virtual bool isArray() const { return false; }

    // Various getters.
    static std::string stringValue(const JSON &json, const std::string &key);
    static int intValue(const JSON &json, const std::string &key, int defaultValue = 0);
    static long longValue(const JSON &json, const std::string &key, long defaultValue = 0L);
    static double doubleValue(const JSON &json, const std::string &key, double defaultValue = 0.0);
    static bool boolValue(const JSON &json, const std::string &key, bool defaultValue = false);
    static JSON jsonValue(const JSON &json, const std::string &key);
    static JSON jsonArray(const JSON &json, const std::string &key);
    static std::vector<int> intVectorValue(const JSON &json, const std::string &key);

    // These setters only push non-empty values.
    static void setStringValue(JSON & json, const std::string &key, const std::string &value);
    static void setLongValue(JSON & json, const std::string &key, const long value);
    static void setJSONValue(JSON & json, const std::string &key, const JSON & jsonToAdd);
    static void translateAndSet(JSON & json, const std::string &key, const JSONSerializable & jsonToAdd);
};

std::ostream & operator<<(std::ostream &, const JSONSerializable &);

/**
 * For a vector of JSONSerializable objects stored as shared_ptrs.
 */
template <typename ObjectType>
class JSONSerializableVector: public std::vector<std::shared_ptr<ObjectType>>, public JSONSerializable {
public:
    typedef std::shared_ptr<ObjectType> Pointer;
    typedef std::function<bool(Pointer)> Comparator;

    bool isArray() const override { return true; }

    /**
     * Populate the vector from this JSON array.
     */
    void fromJSON(const JSON &json) override {
        for (auto iter = json.begin(); iter != json.end(); ++iter) {
            const nlohmann::json obj = *iter;

            Pointer thisDiff = std::make_shared<ObjectType>();
            thisDiff->fromJSON(obj);
            this->push_back(thisDiff);
        }
    }

    /**
     * Serialize this vector into this JSON array.
     */
    JSON & toJSON(JSON &json) const override {
        if (!json.is_array()) {
            json = JSON::array();
        }
        for (const std::shared_ptr<ObjectType> & obj: *this) {
            JSON childJson = JSON::object();
            obj->toJSON(childJson);
            json.push_back(childJson);
        }

        return json;
    }

    void addAll(JSONSerializableVector<ObjectType> &vec) {
        for (Pointer & ptr: vec) {
            this->push_back(ptr);
        }
    }

    /**
     * Remove all instances that match the comparator -- generally a lambda
     * that takes a shared_ptr<ObjectType>.
     */
    void removeAll(Comparator comp) {
        this->erase(std::remove_if(this->begin(), this->end(), comp), this->end());
    }

    /**
     * Finder with a comparator.
     */
    Pointer findIf(Comparator comp) const {
        auto objIter = std::find_if(this->cbegin(), this->cend(), comp);

        return (objIter == this->cend()) ? nullptr : *objIter;
    }
};

/**
 * This template specifically is for a map of string -> object.
 */
template <typename ObjectType>
class JSONSerializableMap: public std::unordered_map<std::string, std::shared_ptr<ObjectType>>, public JSONSerializable
{
public:
    typedef std::shared_ptr<ObjectType> Pointer;

    /**
     * Populate from JSON.
     */
    void fromJSON(const JSON & json) override {
        for (auto it = json.begin(); it != json.end(); ++it) {
            std::string key = it.key();
            std::shared_ptr<ObjectType> ptr = std::make_shared<ObjectType>();

            ptr->fromJSON(it.value());
            this->insert(std::make_pair(key, ptr));
        }
    }

    /**
     * Write to JSON.
     */
    JSON & toJSON(JSON & json) const override {
        for (auto it = this->cbegin(); it != this->cend(); ++it) {
            JSON child;
            it->second->toJSON(child);
            json[it->first] = child;
        }
        return json;
    }
};

}

/**
 * Helper method.
 */
template <typename T>
void addJSON(JSON & json, const std::string &key, ShowLib::JSONSerializableVector<T> vec) {
    if (vec.size() > 0) {
        JSON jv = JSON::array();
        json[key] = vec.toJSON(jv);
    }
}

