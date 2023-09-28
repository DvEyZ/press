#include "../src/Server.h"
#include "../src/http/Request.h"
#include "../src/http/Response.h"
#include "../src/utils/ResponseBuilder.h"

#define MAKE_ROUTE(METHOD, ENDP_STR, HANDLER) \
    std::unique_ptr<press::Route>(new press::Route(press::Endpoint(METHOD, ENDP_STR), HANDLER))
