#include "RESTServer.h"

using std::string;

RESTServer::RESTServer()
    : params(new Poco::Net::HTTPServerParams())
{
}


RESTServer::~RESTServer()
{
    if (running) {
        stop();
    }
}

/**
 * Start the server. Note that the server is going to take ownership
 * of both the params and the request handler and will destroy them
 * when it's done, so we'll forget about them. I don't know what it
 * does with the socket...
 */
void
RESTServer::start() {
    if (!running) {
        RequestHandlerFactory * factory = new RequestHandlerFactory(router);

        socket = std::make_shared<Poco::Net::ServerSocket>(port);
        server = std::make_shared<Poco::Net::HTTPServer>(factory, *socket, params);

        server->start();

        params = nullptr;
    }
}

void
RESTServer::stop() {
    if (running) {
        server->stop();
        running = false;
    }
}

/**
 * Return random JSON.
 */
void
RESTServer::setReturn(Poco::Net::HTTPServerResponse &response, const JSON &json, Poco::Net::HTTPResponse::HTTPStatus code) {
    string body = json.dump(2) + "\n";

    response.setContentType("text/json");
    response.setContentLength(body.length());
    response.setStatus(code);

    std::ostream & str = response.send();
    str << body;
}


/**
 * Will return JSON with success = false and errorMessage
*/
void
RESTServer::returnError(Poco::Net::HTTPServerResponse &response, const std::string &errorMessage, Poco::Net::HTTPResponse::HTTPStatus code) {
    JSON json { JSON::object() };

    json["success"] = false;
    json["errorMessage"] = errorMessage;
    setReturn(response, json, code);
}

/** Will return JSON with success = true and message. */
void
RESTServer::returnSuccessMessage(Poco::Net::HTTPServerResponse &response, const std::string &okMessage, Poco::Net::HTTPResponse::HTTPStatus code) {
    JSON json { JSON::object() };

    json["success"] = true;
    json["message"] = okMessage;
    setReturn(response, json, code);
}

/** Will return this JSON. */
void
RESTServer::returnSuccessBody(Poco::Net::HTTPServerResponse &response, const JSON &json, Poco::Net::HTTPResponse::HTTPStatus code) {
    setReturn(response, json, code);
}

/** Will return this object. */
void
RESTServer::returnSuccessMessage(Poco::Net::HTTPServerResponse &response, const ShowLib::JSONSerializable &obj, Poco::Net::HTTPResponse::HTTPStatus code) {
    JSON json { JSON::object() };
    obj.toJSON(json);
    setReturn(response, json, code);
}
