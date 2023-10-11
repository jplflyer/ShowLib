#include <cstdlib>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Infos.hpp>

#include <nlohmann/json.hpp>

#include "Base64.h"
#include "RESTClient.h"
#include "URI.h"

using std::cout;
using std::cerr;
using std::endl;
using std::shared_ptr;
using std::string;

using namespace ShowLib;
using JSON = nlohmann::json;

//======================================================================
// Non-static methods.
//======================================================================

/**
 * Constructor.
 */
RESTClient::RESTClient() {
}

/**
 * Constructor.
 */
RESTClient::RESTClient(const std::string & _server)
{
    setHost(_server);
}

/**
 * Destructor.
 */
RESTClient::~RESTClient() {
}

/**
 * Set our host.
 */
RESTClient &
RESTClient::setHost(const string &value) {
	host = value;
    return *this;
}

/**
 * Set a timeout used only on the very next call.
 */
RESTClient &
RESTClient::setSingleTimeout(std::chrono::milliseconds timeout) {
    singleTimeout = timeout;
    return *this;
}

/**
 * Set a timeout used only on on all calls.
 */
RESTClient &
RESTClient::setDefaultTimeout(std::chrono::milliseconds timeout) {
    defaultTimeout = timeout;
    return *this;
}

/**
 * Remember a header to set on all requests.
 */
void RESTClient::setStandardHeader(const string &name, const string &value) {
    shared_ptr<HTTPHeader> header = nullptr;

    for (shared_ptr<HTTPHeader> & thisHeader: standardHeaders) {
		if (thisHeader->name == name) {
			header = thisHeader;
			break;
		}
	}

    if (header == nullptr) {
        header = std::make_shared<HTTPHeader>(name, value);
		standardHeaders.push_back(header);
	}
	else if (header->value != value) {
		header->setValue(value);
	}
}

/**
 * This is an add or update of a standard header. If the header already
 * is set, this replaces its value. Otherwise we add it.
 */
void RESTClient::replaceStandardHeader(HTTPHeader::Pointer hdr) {
    for (const HTTPHeader::Pointer &thisHeader: standardHeaders) {
        if (thisHeader->name == hdr->name) {
            thisHeader->setValue(hdr->value);
            return;
        }
    }

    standardHeaders.push_back(hdr);
}

//======================================================================
//
//======================================================================
typedef curlpp::OptionTrait<long, CURLOPT_TIMEOUT_MS> TimeoutMS;

/**
 * Prepare this curl request. Set the URL, set all the headers.
 */
void
RESTClient::prepare(curlpp::Easy & request, const std::string &url, HTTPHeader::Vector *headers) {
    std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();

    if (verbose) {
        cout << "URL: " << url << endl;
    }

    request.setOpt<curlpp::options::Url>(host + url);
    if (allowSelfSigned) {
        request.setOpt(cURLpp::Options::SslVerifyPeer(false));
        request.setOpt(cURLpp::Options::SslVerifyHost(false));
    }

    std::list<string> outHeaders;

    outHeaders.push_back("Expect:");

    if (headers != nullptr) {
        for (HTTPHeader::Pointer & hdr: *headers) {
            outHeaders.push_back(hdr->name + ": " + hdr->value);
        }
    }

    for (HTTPHeader::Pointer & hdr: standardHeaders) {
        outHeaders.push_back(hdr->name + ": " + hdr->value);
    }

    if (outHeaders.size() > 0) {
        request.setOpt( new curlpp::options::HttpHeader(outHeaders));
    }

    if (singleTimeout.count() != 0) {
        request.setOpt( new TimeoutMS(singleTimeout.count()) );
        //request.setOpt( new curlpp::options::Timeout(singleTimeout.count() / 1000) );
        singleTimeout = std::chrono::milliseconds(0);
    }
    else if (defaultTimeout.count() != 0) {
        request.setOpt( new TimeoutMS(defaultTimeout.count()) );
    }
    if (reportTimes) {
        std::chrono::system_clock::time_point endTime = std::chrono::system_clock::now();
        cout << "Time to prepare: " << (std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime)).count()
             << " microseconds." << endl;
    }
}

/**
 * Perform this request. Submits it, gets the body and JSON-parses it, and also stores the
 * status code.
 */
JSON
RESTClient::perform(curlpp::Easy & request) {
    std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point t2;
    std::chrono::system_clock::time_point t3;
    JSON json;
    string data;

    try {
        std::ostringstream stream;
        request.setOpt( new curlpp::options::WriteStream( &stream ) );

        t2 = std::chrono::system_clock::now();
        request.perform();
        t3 = std::chrono::system_clock::now();

        statusCode = curlpp::infos::ResponseCode::get(request);
        if ((statusCode != 200) && (statusCode != 201)) {
            cerr << "perform() request returned statusCode=" << statusCode << endl;
        }

        data = stream.str();

        if (verbose) {
            cout << "Got response: " << data << endl;
        }

        if (!data.empty()) {
            json = JSON::parse(data);
        }
    }
    catch (curlpp::RuntimeError e) {
        cerr << "perform() RumtimeError exception what=[" << e.what() << "] data=[" << data << "]" << endl;
        json["error"] = e.what();
        statusCode = -1;
    }
    catch (JSON::exception e) {
        cerr << "perform() JSON exception what=[" << e.what() << "] data=[" << data << "]" << endl;
        // Probably a JSON parse error, which might mean it's just a string.
        if (data.length() > 0) {
            json = JSON(data);
        }
    }

    if (reportTimes) {
        std::chrono::system_clock::time_point endTime = std::chrono::system_clock::now();
        cout << "Perform"
             << " Total: " << (std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime)).count()
             << "us. Setup: " << (std::chrono::duration_cast<std::chrono::microseconds>(t2 - startTime)).count()
             << "us. perform(): " << (std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2)).count()
             << "us. Parse: " << (std::chrono::duration_cast<std::chrono::microseconds>(endTime - t3)).count()
             << "us." << endl;
    }
    return json;
}

std::string
RESTClient::performRaw(curlpp::Easy & request) {
    string data;

    try {
        std::ostringstream stream;
        request.setOpt( new curlpp::options::WriteStream( &stream ) );

        request.perform();
        statusCode = curlpp::infos::ResponseCode::get(request);
        data = stream.str();
    }
    catch (curlpp::RuntimeError e) {
        cerr << "performRaw() Curl RuntimeError: " << e.what() << endl;
        statusCode = -1;
    }

    return data;
}

/**
 * Do a GET operation.
 */
std::string
RESTClient::getRaw(const string & url, HTTPHeader::Vector *headers)
{
    curlpp::Easy request;

    prepare(request, url, headers);

    return performRaw(request);
}

/**
 * Do a GET operation.
 */
JSON
RESTClient::get(const string & url, HTTPHeader::Vector *headers)
{
    curlpp::Easy request;

    prepare(request, url, headers);

    return perform(request);
}

/**
 * Do a PUT operation.
 */
JSON
RESTClient::put(const string & url, const JSON &json, HTTPHeader::Vector *headers) {
    curlpp::Easy request;
    string body = json.dump(2);
    std::istringstream inputStream{body};

    prepare(request, url, headers);

    request.setOpt( new curlpp::options::InfileSize( body.length() ) );
    request.setOpt( new curlpp::options::ReadStream( &inputStream) );
    request.setOpt<curlpp::options::Put>(true);

    return perform(request);
}

/**
 * Do a POST operation.
 */
JSON
RESTClient::post(const string & url, const JSON &json, HTTPHeader::Vector *headers) {
    return post(url, !json.is_null() ? json.dump(2) : "", headers);
}

/**
 * Do a POST operation.
 */
JSON
RESTClient::post(const string & url, const std::string &body, HTTPHeader::Vector *headers) {
    curlpp::Easy request;

    prepare(request, url, headers);

    request.setOpt( new curlpp::options::Post(true)) ;
    request.setOpt( new curlpp::options::PostFields(body));

    return perform(request);
}

/**
 * Do a DELETE operation.
 */
JSON
RESTClient::del(const string & url, const JSON &json, HTTPHeader::Vector *headers) {
    curlpp::Easy request;

    prepare(request, url, headers);

    request.setOpt<curlpp::options::CustomRequest>("DELETE");
    if (!json.is_null()) {
        request.setOpt<curlpp::options::PostFields>(json.dump(2));
    }

    return perform(request);
}

RESTClient::HTTPHeader::Pointer
RESTClient::makeAuthHeader(const std::string &username, const std::string &pw) {
    string key = username + ":" + pw;
    string encoded = Base64::encode(key);
    HTTPHeader::Pointer ptr = std::make_shared<HTTPHeader>("Authorization", string{"Basic "} + encoded);
    return ptr;
}

//======================================================================
// This class lets us wrap curlpp::initialize() // curlpp::terminate().
//======================================================================

int CURLWrapper::counter = 0;

/**
 * Constructor.
 */
CURLWrapper::CURLWrapper() {
    std::unique_lock<std::mutex> lock(getMutex());

    if (counter++ == 0) {
        curlpp::initialize();
    }
}

/**
 * Destructor.
 */
CURLWrapper::~CURLWrapper() {
    std::unique_lock<std::mutex> lock(getMutex());

    if (--counter == 0) {
        curlpp::terminate();
    }
}

/**
 * We use a mutex lock from our constructor and destructor so we're very, very clean.
 */
std::mutex & CURLWrapper::getMutex() {
    static std::mutex myMutex;

    return myMutex;
}

