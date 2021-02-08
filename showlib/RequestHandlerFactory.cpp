#include <iostream>

#include "RequestHandlerFactory.h"

using std::cout;
using std::endl;

RequestHandlerFactory::RequestHandlerFactory(Router &r)
    : router(r)
{
}

Poco::Net::HTTPRequestHandler *
RequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest &) {
    return new RouterShadow(router);
}
