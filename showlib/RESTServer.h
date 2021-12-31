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
public:
    //======================================================================
    // Startup methods.
    //======================================================================
    RESTServer();
    virtual ~RESTServer();

    Route::Pointer addRoute(const std::string &method, const std::string &path, Route::Callback callback) {
        Route::Pointer route = router.addRoute(method, path, "", callback);
        return route;
    }

    Route::Pointer addRoute(const std::string &method, const std::string &path, const std::string &descr, Route::Callback callback) {
        Route::Pointer route = router.addRoute(method, path, descr, callback);
        return route;
    }

    Route::Pointer addRoute(const std::string &method, const std::string &path, const std::string &descr, bool requiresAuth, Route::Callback callback) {
        Route::Pointer route = router.addRoute(method, path, descr, callback);
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
    static void setReturn(HTTPServerResponse &, const JSON &json, Poco::Net::HTTPResponse::HTTPStatus code = Poco::Net::HTTPResponse::HTTP_OK);

    /** Will return JSON with success = false and errorMessage */
    static void returnError(HTTPServerResponse &, const std::string &errorMessage, Poco::Net::HTTPResponse::HTTPStatus code = Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);

    /** Will return JSON with success = true and message. */
    static void returnSuccess(HTTPServerResponse &, const std::string &okMessage, Poco::Net::HTTPResponse::HTTPStatus code = Poco::Net::HTTPResponse::HTTP_OK);

    static void returnSuccess(HTTPServerResponse &, const char * okMessage, Poco::Net::HTTPResponse::HTTPStatus code = Poco::Net::HTTPResponse::HTTP_OK);

    /** Will return this JSON. */
    static void returnSuccess(HTTPServerResponse &, const JSON &json, Poco::Net::HTTPResponse::HTTPStatus code = Poco::Net::HTTPResponse::HTTP_OK);

    /** Will return this object. */
    static void returnSuccess(HTTPServerResponse &, const ShowLib::JSONSerializable &, Poco::Net::HTTPResponse::HTTPStatus code = Poco::Net::HTTPResponse::HTTP_OK);

    /**
     * Produces JSON:
     * 		{
     * 			"status": message,
     * 			key: object-as-json
     * 		}
     */
    static void returnSuccess(
        HTTPServerResponse &response,
        const std::string &key,
        const ShowLib::JSONSerializable &object,
        const std::string &message,
        Poco::Net::HTTPResponse::HTTPStatus code = Poco::Net::HTTPResponse::HTTP_OK);

    static void returnHTML(
            HTTPServerResponse &response,
            const std::string &body,
            Poco::Net::HTTPResponse::HTTPStatus code = Poco::Net::HTTPResponse::HTTP_OK);

    static void returnPlainText(
            HTTPServerResponse &response,
            const std::string &body,
            Poco::Net::HTTPResponse::HTTPStatus code = Poco::Net::HTTPResponse::HTTP_OK);

    static void returnWithType(
            HTTPServerResponse &response,
            const std::string &body,
            const std::string &contentType,
            Poco::Net::HTTPResponse::HTTPStatus code = Poco::Net::HTTPResponse::HTTP_OK);

    //======================================================================
    // Helpful methods.
    //======================================================================
    static std::pair<std::string, std::string> getAuthentication(const HTTPServerRequest &, HTTPServerResponse &);

    static ShowLib::ArgumentVector getArguments(HTTPServerRequest &request);
    static std::string urlDecode(const std::string &input);

protected:
    Router & getRouter() { return router; }

private:
    Router router;

    Poco::Net::HTTPServerParams * params = nullptr;
    std::shared_ptr<Poco::Net::HTTPServer> server = nullptr;
    std::shared_ptr<Poco::Net::ServerSocket> socket = nullptr;

    Poco::UInt16 port = 8080;
    bool running = false;

};

