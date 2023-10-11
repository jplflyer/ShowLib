#pragma once

#include <iostream>
#include <chrono>
#include <mutex>
#include <sstream>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>

#include <nlohmann/json.hpp>
#include "URI.h"

namespace ShowLib {
    class CURLWrapper;
    class RESTClient;
}

/**
 * This provides an RAII-like interface to wrap calls to curlpp::initialize() and
 * curlpp::terminate(). Each RESTClient instantiates one of these and the rest just works.
 */
class ShowLib::CURLWrapper {
public:
    CURLWrapper();
    virtual ~CURLWrapper();

private:
    static std::mutex & getMutex();
    static int counter;
};


/**
 * Used for posting REST calls.
 *
 * If you need to set standard headers, you can call setStandardHeader. The values set will be
 * applied to all future calls, so you can set once and forget.
 *
 * We need to determine if we need more flexibility in headers.
 *
 * I recommend using get(), put(), post(), and del() for the calls and not call submit() directly.
 */
class ShowLib::RESTClient {
public:
    typedef nlohmann::json JSON;

    /** A single header. */
	class HTTPHeader {
	public:
        /** Header name. */
		std::string	name;

        /** Header value. */
		std::string value;

        /** Constructor. */
		HTTPHeader(const std::string &_name, const std::string &_value): name(_name), value(_value) {}

        /** Set this header's value. */
		void setValue(const std::string &_value) { value = _value; }

        typedef std::shared_ptr<HTTPHeader> Pointer;
        typedef std::vector<Pointer> Vector;
	};

    /** Constructor. */
    RESTClient();

    /** Constructor. */
    RESTClient(const std::string & _server);

	virtual ~RESTClient();

    RESTClient & setHost(const std::string &value);
    RESTClient & setVerbose(bool value) { verbose = value; return *this; }
    RESTClient & setReportTimes(bool value) { reportTimes = value; return *this; }
    RESTClient & setAllowSelfSigned(bool value) { allowSelfSigned = value; return *this; }

    RESTClient & setSingleTimeout(std::chrono::milliseconds timeout);
    RESTClient & setDefaultTimeout(std::chrono::milliseconds timeout);

    /** Hostname we connect to. */
	const std::string & getHost() const { return host; }

	void setStandardHeader(const std::string &name, const std::string &value);
    void pushStandardHeader(HTTPHeader::Pointer hdr) { standardHeaders.push_back(hdr); }
    void replaceStandardHeader(HTTPHeader::Pointer hdr);

    nlohmann::json get(const std::string &url, HTTPHeader::Vector *headers = nullptr);
    nlohmann::json put(const std::string &url, const nlohmann::json &json, HTTPHeader::Vector *headers = nullptr);
    nlohmann::json post(const std::string &url, const nlohmann::json &json, HTTPHeader::Vector *headers = nullptr);
    nlohmann::json post(const std::string &url, const std::string &body, HTTPHeader::Vector *headers = nullptr);
    nlohmann::json del(const std::string &url, const nlohmann::json &body, HTTPHeader::Vector *headers = nullptr);

    std::string getRaw(const std::string &url, HTTPHeader::Vector *headers = nullptr);

    int lastStatusCode() const { return statusCode; }

    HTTPHeader::Pointer makeAuthHeader(const std::string & username, const std::string & pw);

protected:
    /** Our host name. */
    std::string host;

    /** Any standard headers we'll set on every request. */
    HTTPHeader::Vector	standardHeaders;

    std::chrono::milliseconds singleTimeout = std::chrono::milliseconds(0);
    std::chrono::milliseconds defaultTimeout = std::chrono::milliseconds(0);

    int statusCode;

    bool verbose = false;
    bool reportTimes = false;
    bool allowSelfSigned = false;

    CURLWrapper curlWrapper;

    void prepare(curlpp::Easy & request, const std::string &url, HTTPHeader::Vector *headers);
    JSON perform(curlpp::Easy & request);
    std::string performRaw(curlpp::Easy & request);

};
