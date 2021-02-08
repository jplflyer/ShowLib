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
#include <string>
#include <vector>

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

class Route {
public:
    typedef std::shared_ptr<Route> Pointer;
    typedef std::vector<Pointer> Vector;
    typedef std::function<void(const Poco::Net::HTTPServerRequest &, Poco::Net::HTTPServerResponse &)> Callback;

    std::string method;
    std::string path;
    Callback callback;

    Route();
    Route(const std::string &m, const std::string &p, Route::Callback );
    bool matches(const std::string &m, const std::string &p) const;
};

class Router: public Poco::Net::HTTPRequestHandler
{
private:
    Route::Vector routes;

    void notRecognized_JSON(Poco::Net::HTTPServerResponse &);
    void notRecognized_HTML(Poco::Net::HTTPServerResponse &);

public:
    Router();

    void handleRequest(Poco::Net::HTTPServerRequest &, Poco::Net::HTTPServerResponse &);

    void addRoute(const std::string &method, const std::string &path, Route::Callback callback);
};


class RouterShadow: public Poco::Net::HTTPRequestHandler {
private:
    Router & router;

public:
    RouterShadow(Router &r) : router(r) { }
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) {
        router.handleRequest(request, response);
    }
};
