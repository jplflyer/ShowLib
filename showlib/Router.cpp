#include <iostream>
#include <string>

#include <nlohmann/json.hpp>
#include <showlib/StringUtils.h>

#include "Router.h"

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
    string method = toUpper(request.getMethod());
    string path = toLower(request.getURI());
    string accept = request.get("Accept");

    for (Route::Pointer &route: routes) {
        if (route->matches(method, path)) {
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

void Router::notRecognized_JSON(Poco::Net::HTTPServerResponse &response) {
    JSON json = JSON::object();

    json["status"] = "failure";
    json["error"] = "Request not recognized";

    string body = json.dump(2);

    response.setContentType("text/json");
    response.setContentLength(body.length());
    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);

    std::ostream & str = response.send();
    str << body;
}

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
void Router::addRoute(const std::string &method, const std::string &path, Route::Callback callback) {
    Route::Pointer route = std::make_shared<Route>(method, path, callback);

    routes.push_back(route);
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
    cout << "Look for match of method: " << m << " with path " << p << endl;

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
