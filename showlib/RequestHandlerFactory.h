#pragma once

#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>

#include "Router.h"

class RequestHandlerFactory: public Poco::Net::HTTPRequestHandlerFactory
{
private:
    Router & router;

public:
    RequestHandlerFactory(Router &r);
    Poco::Net::HTTPRequestHandler * createRequestHandler(const Poco::Net::HTTPServerRequest &);
};

