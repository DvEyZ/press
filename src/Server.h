#pragma once
#include <string>
#include <asio.hpp>
#include <memory>
#include "Endpoint.h"
#include "http/Response.h"
#include "Route.h"

namespace ip = asio::ip;

namespace press {

    class Server {
    private:
        short port;
        asio::io_context& context;
        ip::tcp::acceptor acceptor;
        std::vector<std::unique_ptr<IRoute>> routes;
        void accept();
    public:
        Server(unsigned short port, asio::io_context& context);
        void route(std::unique_ptr<IRoute>&& route);
        void init();
    };
}