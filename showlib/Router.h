#pragma once

//======================================================================
// To use, see this code:
//
// #include <Poco/Poco.h>
// #include <Poco/Net/HTTPServer.h>
//
// #include <nlohmann/json.hpp>
//
// #include "Router.h"
// #include "RequestHandlerFactory.h"
// void handleHello(const Poco::Net::HTTPServerRequest &, Poco::Net::HTTPServerResponse &);
//
// int main()
// {
//    Poco::UInt16 port = 9900;
//    Poco::Net::HTTPServerParams * params = new Poco::Net::HTTPServerParams;
//    Router router;
//
//    router.addRoute("GET", "/hello.json", handleHello);
//
//    params->setMaxQueued(100);
//    params->setMaxThreads(16);
//
//    Poco::Net::ServerSocket svs(port);
//    RequestHandlerFactory factory {router};
//    Poco::Net::HTTPServer server(&factory, svs, params);
//
//    server.start();
//
//    // Main sleeps forever.
//    while(true) {
//        std::this_thread::sleep_for(std::chrono::minutes(10));
//    }
//
//    server.stop();
//
//    return 0;
// }
//
// void handleHello(const Poco::Net::HTTPServerRequest &, Poco::Net::HTTPServerResponse &response) {
//    JSON json = JSON::object();
//
//    json["status"] = "success";
//    json["message"] = "Happy Halloween!";
//
//    string body = json.dump(2);
//
//    response.setContentType("text/json");
//    response.setContentLength(body.length());
//    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_OK);
//
//    std::ostream & str = response.send();
//    str << body;
// }

//======================================================================

#include <functional>
#include <memory>
#include <regex>
#include <string>
#include <vector>

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;

/**
 * This defines what to do with one call, such as "GET /foo"
 */
class Route {
public:
    typedef std::shared_ptr<Route> Pointer;
    typedef std::vector<Pointer> Vector;
    typedef std::function<void(HTTPServerRequest &, HTTPServerResponse &)> Callback;
    typedef std::function<void(HTTPServerRequest &, HTTPServerResponse &, const std::smatch &)> CallbackR;

    std::string method;
    std::string path;
    std::regex pathRegex;
    std::string description;
    Callback callback = nullptr;
    CallbackR callbackR = nullptr;
    bool requiresAuthorization = true;
    bool isRegex = false;

    Route();
    Route(const std::string &m, const std::string &p, const std::string &d, Route::Callback );
    Route(const std::string &m, const std::string &p, const std::string &d, Route::CallbackR );
    bool matches(const std::string &m, const std::string &p, std::smatch &myMatch) const;
};

/**
 * This just wraps the collection of routes and dispatches calls
 * according to the rules. Rules are evaluated in order, so when adding
 * routes, add more specific rules first, as we only check an incoming route
 * up to the length of the given path. For instance:
 *
 * 		/foo
 * 		/foo/bar
 *
 * If the call is /foo/bar, but you've defined rules in the above order,
 * it will match the wrong one.
 */
class Router: public Poco::Net::HTTPRequestHandler
{
public:
    typedef std::function<bool(const HTTPServerRequest &, HTTPServerResponse &)> AuthCallback;

    Router();

    void disableHelp() { helpEnabled = false; }
    void setVerbose(bool value) { verbose = value; }
    void handleRequest(HTTPServerRequest &, HTTPServerResponse &);

    Route::Pointer addRoute(const std::string &method, const std::string &path, Route::Callback callback);
    Route::Pointer addRoute(const std::string &method, const std::string &path, const std::string &descr, Route::Callback callback);
    Route::Pointer addRouteR(const std::string &method, const std::string &path, Route::CallbackR callback);
    Route::Pointer addRouteR(const std::string &method, const std::string &path, const std::string &descr, Route::CallbackR callback);

    void setAuthorization(AuthCallback cb) { authCallback = cb; }

private:
    Route::Vector routes;
    AuthCallback authCallback = nullptr;
    bool helpEnabled = true;
    bool verbose = false;

    void notRecognized_JSON(HTTPServerResponse &);
    void notRecognized_HTML(HTTPServerResponse &);
    void help_JSON(HTTPServerResponse &);
    void help_Text(HTTPServerResponse &);
    void help_HTML(HTTPServerResponse &);

};

/**
 * We use a shadow because the Poco::Net server expects these to be
 * use-once. But Router is thread safe, and we don't need to keep
 * destroying them. So the shadow gets created and destroyed willy-nilly,
 * but they're cheap.
 */
class RouterShadow: public Poco::Net::HTTPRequestHandler {
private:
    Router & router;

public:
    RouterShadow(Router &r) : router(r) { }
    void handleRequest(HTTPServerRequest &request, HTTPServerResponse &response) {
        router.handleRequest(request, response);
    }
};
