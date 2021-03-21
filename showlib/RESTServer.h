#pragma once

#include <memory>

#include <Poco/Poco.h>
#include <Poco/Net/HTTPServer.h>

#include <nlohmann/json.hpp>

#include "ArgumentVector.h"
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

    Route::Pointer addRoute(const std::string &method, const std::string &path, Route::Callback callback) {
        return router.addRoute(method, path, callback);
    }

    Route::Pointer addRoute(const std::string &method, const std::string &path, bool requiresAuth, Route::Callback callback) {
        Route::Pointer route = router.addRoute(method, path, callback);
        route->requiresAuthorization = requiresAuth;
        return route;
    }

    RESTServer & setPort(Poco::UInt16 _port) { port = _port; return *this; }
    RESTServer & setMaxQueued(int value) { params->setMaxQueued(value); return *this; }
    RESTServer & setMaxThreads(int value) { params->setMaxThreads(value); return *this; }

    void start();
    void stop();

    //======================================================================
    // These are return methods so individual calls don't need to do this
    // directly.
    //======================================================================
    static void setReturn(Poco::Net::HTTPServerResponse &, const JSON &json, Poco::Net::HTTPResponse::HTTPStatus code = Poco::Net::HTTPResponse::HTTP_OK);

    /** Will return JSON with success = false and errorMessage */
    static void returnError(Poco::Net::HTTPServerResponse &, const std::string &errorMessage, Poco::Net::HTTPResponse::HTTPStatus code = Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);

    /** Will return JSON with success = true and message. */
    static void returnSuccess(Poco::Net::HTTPServerResponse &, const std::string &okMessage, Poco::Net::HTTPResponse::HTTPStatus code = Poco::Net::HTTPResponse::HTTP_OK);

    static void returnSuccess(Poco::Net::HTTPServerResponse &, const char * okMessage, Poco::Net::HTTPResponse::HTTPStatus code = Poco::Net::HTTPResponse::HTTP_OK);

    /** Will return this JSON. */
    static void returnSuccess(Poco::Net::HTTPServerResponse &, const JSON &json, Poco::Net::HTTPResponse::HTTPStatus code = Poco::Net::HTTPResponse::HTTP_OK);

    /** Will return this object. */
    static void returnSuccess(Poco::Net::HTTPServerResponse &, const ShowLib::JSONSerializable &, Poco::Net::HTTPResponse::HTTPStatus code = Poco::Net::HTTPResponse::HTTP_OK);

    /**
     * Produces JSON:
     * 		{
     * 			"status": message,
     * 			key: object-as-json
     * 		}
     */
    static void returnSuccess(
        Poco::Net::HTTPServerResponse &response,
        const std::string &key,
        const ShowLib::JSONSerializable &object,
        const std::string &message,
        Poco::Net::HTTPResponse::HTTPStatus code = Poco::Net::HTTPResponse::HTTP_OK);

    static void returnHTML(
            Poco::Net::HTTPServerResponse &response,
            const std::string &body,
            Poco::Net::HTTPResponse::HTTPStatus code = Poco::Net::HTTPResponse::HTTP_OK);

    static void returnPlainText(
            Poco::Net::HTTPServerResponse &response,
            const std::string &body,
            Poco::Net::HTTPResponse::HTTPStatus code = Poco::Net::HTTPResponse::HTTP_OK);

    static void returnWithType(
            Poco::Net::HTTPServerResponse &response,
            const std::string &body,
            const std::string &contentType,
            Poco::Net::HTTPResponse::HTTPStatus code = Poco::Net::HTTPResponse::HTTP_OK);

    static ShowLib::ArgumentVector getArguments(Poco::Net::HTTPServerRequest &request);
    static std::string urlDecode(const std::string &input);
};

