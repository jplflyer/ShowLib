#include <iostream>
#include <string>

#include <nlohmann/json.hpp>
#include <showlib/StringUtils.h>

#include "Router.h"
#include "RESTServer.h"

using std::cout;
using std::endl;
using std::string;
using JSON = nlohmann::json;

using namespace ShowLib;

/**
 * Constructor.
 */
Router::Router() {
}

/**
 * Handle the request.
 */
void Router::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) {
    try {
        string method = toUpper(request.getMethod());
        string path = toLower(request.getURI());
        string accept = request.get("Accept");

        for (Route::Pointer &route: routes) {
            if (route->matches(method, path)) {
                if (route->requiresAuthorization && authCallback != nullptr) {
                    if (!authCallback(request, response)) {
                        return;
                    }
                }

                route->callback(request, response);
                return;
            }
        }

        if (accept == "text/json") {
            notRecognized_JSON(response);
        }
        else {
            notRecognized_HTML(response);
        }
    }
    catch (std::exception &e) {
        cout << "Exception: " << e.what() << endl;
        RESTServer::returnError(response, "Server error", Poco::Net::HTTPResponse::HTTPStatus::HTTP_INTERNAL_SERVER_ERROR);
    }
}

/**
 * This method gets called if we get through the route table and don't
 * find a call AND if the Accept header is text/json.
 */
void Router::notRecognized_JSON(Poco::Net::HTTPServerResponse &response) {
    RESTServer::returnError(response, "Request not recognized", Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
}

/**
 * This gets called when we don't find a route and the Accept header is
 * anything other than text/json.
 */
void Router::notRecognized_HTML(Poco::Net::HTTPServerResponse &response) {
    string body = "<html><head><title>Path Not Found</title></head><body>Path not found</body></html>\n";

    response.setContentType("text/html");
    response.setContentLength(body.length());
    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);

    std::ostream & str = response.send();
    str << body;
}

/**
 * Add a route.
 */
Route::Pointer Router::addRoute(const std::string &method, const std::string &path, Route::Callback callback) {
    Route::Pointer route = std::make_shared<Route>(method, path, callback);

    routes.push_back(route);
    return route;
}

/**
 * Constructor.
 */
Route::Route(const std::string &m, const std::string &p, Route::Callback c)
    : method(toUpper(m)), path(toLower(p)), callback(c)
{
}

/**
 * Does this route match?
 */
bool Route::matches(const std::string &m, const std::string &p) const {
    if (m != method) {
        return false;
    }

    int myPathLen = path.length();
    string thisPath = p.substr(0, myPathLen);
    if (thisPath == path) {
        return true;
    }

    return false;
}
