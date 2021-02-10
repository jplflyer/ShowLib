#pragma once

#include <memory>

#include <Poco/Poco.h>
#include <Poco/Net/HTTPServer.h>

#include <nlohmann/json.hpp>

#include "JSONSerializable.h"
#include "Router.h"
#include "RequestHandlerFactory.h"



/**
 * Base class for anyone putting up a REST server. Uses Poco/Net/HTTPServer.
 */
class RESTServer
{
private:
    Router router;

    Poco::Net::HTTPServerParams * params = nullptr;
    std::shared_ptr<Poco::Net::HTTPServer> server = nullptr;
    std::shared_ptr<Poco::Net::ServerSocket> socket = nullptr;

    Poco::UInt16 port = 8080;
    bool running = false;

protected:
    Router & getRouter() { return router; }

public:
    //======================================================================
    // Startup methods.
    //======================================================================
    RESTServer();
    virtual ~RESTServer();

    RESTServer & addRoute(const std::string &method, const std::string &path, Route::Callback callback) {
        router.addRoute(method, path, callback);
        return *this;
    }

    RESTServer & setPort(Poco::UInt16 _port) { port = _port; return *this; }
    RESTServer & setMaxQueued(int value) { params->setMaxQueued(value); return *this; }
    RESTServer & setMaxThreads(int value) { params->setMaxThreads(value); return *this; }

    void start();
    void stop();

    //======================================================================
    // Convenience methods to help individual calls.
    //======================================================================
    static void setReturn(Poco::Net::HTTPServerResponse &, const JSON &json, Poco::Net::HTTPResponse::HTTPStatus code);

    /** Will return JSON with success = false and errorMessage */
    static void returnError(Poco::Net::HTTPServerResponse &, const std::string &errorMessage, Poco::Net::HTTPResponse::HTTPStatus code = Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);

    /** Will return JSON with success = true and message. */
    static void returnSuccessMessage(Poco::Net::HTTPServerResponse &, const std::string &okMessage, Poco::Net::HTTPResponse::HTTPStatus code = Poco::Net::HTTPResponse::HTTP_OK);

    /** Will return this JSON. */
    static void returnSuccessBody(Poco::Net::HTTPServerResponse &, const JSON &json, Poco::Net::HTTPResponse::HTTPStatus code = Poco::Net::HTTPResponse::HTTP_OK);

    /** Will return this object. */
    static void returnSuccessMessage(Poco::Net::HTTPServerResponse &, const ShowLib::JSONSerializable &, Poco::Net::HTTPResponse::HTTPStatus code = Poco::Net::HTTPResponse::HTTP_OK);

};

