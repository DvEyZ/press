#pragma once
#include "Endpoint.h"
#include "http/Response.h"
#include "Meta.h"

namespace press {
    class IRoute {
    public:
        virtual ~IRoute() {};
        virtual Endpoint& get_endpoint() = 0;
        virtual http::Response handle(http::Request&& req, Meta&& meta) = 0;
    };

    class Route : public IRoute {
    public:
        typedef std::function<http::Response(http::Request&&, Meta&&)> Handler;
    private:
        Endpoint endpoint;
        Handler handler;
    public:
        Route(Endpoint endpoint, Handler handler)
            :endpoint(endpoint), handler(handler)
        {};
        virtual ~Route() {};

        virtual Endpoint& get_endpoint();
        virtual http::Response handle(http::Request&& req, Meta&& meta);
    };

}