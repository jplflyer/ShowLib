#include "Base64.h"
#include "RESTServer.h"
#include "StringUtils.h"
#include "StringVector.h"

using std::cout;
using std::endl;
using std::string;

using namespace ShowLib;
using ArgumentPair = ArgumentVector::ArgumentPair;


/**
 * Constructor.
 */
RESTServer::RESTServer()
    : params(new Poco::Net::HTTPServerParams())
{
}

/**
 * Destructor.
 */
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
    returnWithType(response, body, "text/json", code);
}

/**
 * Return HTML.
 */
void
RESTServer::returnHTML(Poco::Net::HTTPServerResponse &response, const string &body, Poco::Net::HTTPResponse::HTTPStatus code) {
    returnWithType(response, body, "text/html", code);
}

/**
 * Return text.
 */
void
RESTServer::returnPlainText(Poco::Net::HTTPServerResponse &response, const string &body, Poco::Net::HTTPResponse::HTTPStatus code) {
    returnWithType(response, body, "text/plain", code);
}

/**
 * Return random JSON.
 */
void
RESTServer::returnWithType(
    Poco::Net::HTTPServerResponse &response,
    const string &body,
    const string &contentType,
    Poco::Net::HTTPResponse::HTTPStatus code)
{
    response.setContentType(contentType);
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
    JSON json = JSON::object();

    json["success"] = false;
    json["httpStatus"] = code;
    json["errorMessage"] = errorMessage;
    setReturn(response, json, code);
}

/** Will return JSON with success = true and message. */
void
RESTServer::returnSuccess(Poco::Net::HTTPServerResponse &response, const std::string &okMessage, Poco::Net::HTTPResponse::HTTPStatus code) {
    JSON json = JSON::object();

    json["success"] = true;
    json["httpStatus"] = code;
    json["message"] = okMessage;
    setReturn(response, json, code);
}

/** Will return JSON with success = true and message. */
void
RESTServer::returnSuccess(Poco::Net::HTTPServerResponse &response, const char * okMessage, Poco::Net::HTTPResponse::HTTPStatus code) {
    JSON json = JSON::object();

    json["success"] = true;
    json["httpStatus"] = code;
    json["message"] = okMessage;
    setReturn(response, json, code);
}

/** Will return this JSON. */
void
RESTServer::returnSuccess(Poco::Net::HTTPServerResponse &response, const JSON &json, Poco::Net::HTTPResponse::HTTPStatus code) {
    setReturn(response, json, code);
}

/** Will return this object. */
void
RESTServer::returnSuccess(Poco::Net::HTTPServerResponse &response, const ShowLib::JSONSerializable &obj, Poco::Net::HTTPResponse::HTTPStatus code) {
    setReturn(response, obj.toJSON(), code);
}

/**
 * Produces JSON:
 * 		{
 * 			"message": message,
 * 			key: object-as-json
 * 		}
 */
void
RESTServer::returnSuccess(
    Poco::Net::HTTPServerResponse &response,
    const std::string &key,
    const ShowLib::JSONSerializable &object,
    const std::string &message,
    Poco::Net::HTTPResponse::HTTPStatus code)
{
    JSON json = JSON::object();

    json[key] = object.toJSON();
    if (message.length() > 0) {
        json["message"] = message;
    }
    json["httpStatus"] = code;
    json["success"] = true;

    setReturn(response, json, code);
}

/**
 * Retrieve authentication for Basic only, username and password.
 */
ShowLib::StringsTuple
RESTServer::getAuthentication(const HTTPServerRequest &request, HTTPServerResponse &response)
{
    StringsTuple tuple;

    string scheme;
    string authInfo;

    request.getCredentials(scheme, authInfo);

    tuple.first = scheme;

    if (scheme == "Basic") {
        string decoded = Base64::decodeToString(authInfo);
        auto [username,  password] = splitPair(decoded, string{":"} );

        tuple.second = toLower(username);
        tuple.third = password;
    }
    else if (scheme == "Bearer") {
        tuple.second = authInfo;
    }

    return tuple;
}

/**
 * Retrive all the args from the URL.
 */
ShowLib::ArgumentVector
RESTServer::getArguments(Poco::Net::HTTPServerRequest &request) {
    ShowLib::ArgumentVector vec;
    string path = request.getURI();
    size_t startOfArgs = path.find("?");

    if (startOfArgs != string::npos) {
        ShowLib::StringVector strVec;
        strVec.tokenize(path.substr(startOfArgs + 1), '&');

        for (const std::shared_ptr<string> &arg: strVec) {
            ShowLib::StringVector parts;
            parts.tokenize(*arg, '=');
            vec.append(*parts[0],  (parts.size() > 1) ? urlDecode(*parts[1]) : "");
        }
    }

    return vec;
}

/**
 * Perform URL decoding of this string.
 */
std::string
RESTServer::urlDecode(const std::string &text)
{
    string output;

    for (auto i = text.begin(), nd = text.end(); i < nd; ++i)
    {
        const char c = ( *i );

        switch(c)
        {
            case '%':
                if (i + 2 < nd) {
                    char hs[]{ i[1], i[2], 0 };
                    output += static_cast<char>(strtol(hs, nullptr, 16));
                    i += 2;
                }
                break;

            case '+':
                output += ' ';
                break;

            default:
                output += c;
        }
    }

    return output;
}

