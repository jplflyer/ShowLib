#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <nlohmann/json.hpp>

#include <showlib/CommonUsing.h>
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
        if (verbose) {
            cout << "Method: " << method << " path: " << path << endl;
        }

        for (Route::Pointer &route: routes) {
            std::smatch myMatch;
            if (route->matches(method, path, myMatch)) {
                if (route->requiresAuthorization && authCallback != nullptr) {
                    if (!authCallback(request, response)) {
                        return;
                    }
                }

                if (route->isRegex) {
                    route->callbackR(request, response, myMatch);
                }
                else {
                    route->callback(request, response);
                }
                return;
            }
        }

        //----------------------------------------------------------------------
        // If we get this far, it's not something we recognize from addRoute().
        // It might be help, or it might be garbage.
        //----------------------------------------------------------------------
        if (helpEnabled && path == "/help") {
            if (accept == "text/json") {
                help_JSON(response);
            }
            else if (accept == "text/html") {
                help_HTML(response);
            }
            else {
                help_Text(response);
            }
            return;
        }

        if (accept == "text/json") {
            notRecognized_JSON(response);
        }
        else {
            notRecognized_HTML(response);
        }
    }
    catch (std::exception &e) {
        cout << "Exception handling request: " << e.what() << endl;
        RESTServer::returnError(response, "Server error", Poco::Net::HTTPResponse::HTTPStatus::HTTP_INTERNAL_SERVER_ERROR);
    }
}

/**
 * This method gets called if we get through the route table and don't
 * find a call AND if the Accept header is text/json.
 */
void Router::notRecognized_JSON(Poco::Net::HTTPServerResponse &response) {
    RESTServer::returnError(response, "Request not recognized. Try /help.", Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
}

/**
 * This gets called when we don't find a route and the Accept header is
 * anything other than text/json.
 */
void Router::notRecognized_HTML(Poco::Net::HTTPServerResponse &response) {
    string body = "<html><head><title>Path Not Found</title></head><body>Path not found. Try /help.</body></html>\n";
    RESTServer::returnHTML(response, body, Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
}

/**
 * /help -- JSON format.
 */
void Router::help_JSON(HTTPServerResponse &response) {
    JSON json = JSON::object();
    JSON paths = JSON::array();

    for (const Route::Pointer &route: routes) {
        JSON routeJSON = JSON::object();
        routeJSON["method"] = route->method;
        routeJSON["path"] = route->path;
        routeJSON["description"] = route->description;
        paths.push_back(routeJSON);
    }

    json["curl_example"] = "curl -u username:password [-x PUT|POST|DELETE] [-d '{JSON}'] http://localhost:port/path";
    json["paths"] = paths;

    RESTServer::setReturn(response, json);
}

/**
 * /help -- plain text format.
 */
void Router::help_Text(HTTPServerResponse &response) {
    std::ostringstream oStr;

    oStr << "We recognize the following routes:" << endl << endl;
    size_t longest = 0;
    for (const Route::Pointer &route: routes) {
        if (route->path.length() > longest) {
            longest = route->path.length();
        }
    }
    for (const Route::Pointer &route: routes) {
        if (verbose) {
            cout << "Descr: " << route->description << endl;
        }
        oStr << "    " << std::setw(6) << std::left << route->method
             << "  " << std::setw(longest) << std::left << route->path
             << " " << route->description << endl;
    }

    oStr << endl << "A curl example:" << endl
         << "	curl -u username:password [-x PUT|POST|DELETE] [-d '{JSON}'] http://localhost:port/path" << endl;

    RESTServer::returnPlainText(response, oStr.str());
}

/**
 * /help -- HTML.
 */
void Router::help_HTML(HTTPServerResponse &response) {
    std::ostringstream oStr;

    oStr << "<html>" << endl
         << "<head>" << endl
         << "	<title>Help</title>" << endl
         << "</head>" << endl
         << "<body>" << endl
         << "	<p>We recognize the following routes:</p>" << endl << endl
         << "	<table>" << endl
            ;

    for (const Route::Pointer &route: routes) {
        oStr << "		<tr><td>" << route->method
             << "</td><td>" << route->path
             << "</td><td>" << route->description
             << "</td></tr>" << endl;
    }

    oStr << "	</table>" << endl << endl
         << "	<p>curl -u username:password [-x PUT|POST|DELETE] [-d '{JSON}'] http://localhost:port/path</p>" << endl
         << "</body>" << endl
         <<"</html>" << endl;

    RESTServer::returnHTML(response, oStr.str());
}

/**
 * Add a route.
 */
Route::Pointer Router::addRoute(const std::string &method, const std::string &path, Route::Callback callback) {
    return addRoute(method, path, "", callback);
}

/**
 * Add a route.
 */
Route::Pointer Router::addRoute(const std::string &method, const std::string &path, const std::string &descr, Route::Callback callback)
{
    Route::Pointer route = std::make_shared<Route>( method, path, descr, callback );

    routes.push_back(route);
    return route;
}

/**
 * Add a route using a regular expression as the path.
 */
Route::Pointer Router::addRouteR(const std::string &method, const std::string &path, Route::CallbackR callback) {
    return addRouteR(method, path, "", callback);
}

/**
 * Add a route using a regular expression as the path.
 */
Route::Pointer Router::addRouteR(const std::string &method, const std::string &path, const std::string &descr, Route::CallbackR callback)
{
    Route::Pointer route = std::make_shared<Route>( method, path, descr, callback );

    routes.push_back(route);
    return route;
}

/**
 * Constructor.
 */
Route::Route(const std::string &m, const std::string &p, const std::string &d, Route::Callback c)
    : method(toUpper(m)), path(toLower(p)), description(d), callback(c)
{
}

/**
 * Constructor for regex.
 */
Route::Route(const std::string &m, const std::string &p, const std::string &d, Route::CallbackR c)
    : method(toUpper(m)), path(toLower(p)), description(d), callbackR(c)
{
    isRegex = true;
    try {
        pathRegex = std::regex( path,  std::regex_constants::extended | std::regex_constants::icase );
    }
    catch (std::regex_error &e) {
        cerr << "Regex error processing " << path << " -- " << e.what() << endl;
        exit(1);
    }
}


/**
 * Does this route match?
 */
bool Route::matches(const std::string &m, const std::string &p, std::smatch & myMatch) const {
    if (m != method) {
        return false;
    }

    if (!isRegex) {
        int myPathLen = path.length();
        string thisPath = p.substr(0, myPathLen);
        if (thisPath == path) {
            return true;
        }
    }

    else {
        return std::regex_match(p, myMatch, pathRegex);
    }

    return false;
}
