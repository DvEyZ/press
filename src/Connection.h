#pragma once

#include <functional>
#include <memory>
#include <exception>
#include <asio.hpp>
#include "http/Request.h"
#include "http/Response.h"

namespace ip = asio::ip;

namespace press {

    class Connection :public std::enable_shared_from_this<Connection> {
        const static int CHUNK_SIZE = 16;
        enum class WaitFor {
            Request, Headers, BodyWithSize, BodyChunked
        };
        int body_size;
        int body_transferred;
        int response_transfer;

        WaitFor wait_for;
        std::array<char, CHUNK_SIZE> chunk;
        std::string buffer;
        std::string response_buffer;
        http::Request request;

        ip::tcp::socket socket;

        std::function<http::Response(http::Request&&)> on_recv;
        std::function<http::Response(std::exception&, http::Status)> on_error;

        void read();
        void on_read(const asio::error_code e, size_t bytes_transferred);

        void write();
        void on_write(const asio::error_code e, size_t bytes_transferred);

        void respond();
        void respond_error(std::exception& e, http::Status status);

        void shutdown();
    public:
        Connection(
            ip::tcp::socket&& socket,
            std::function<http::Response(http::Request&&)> on_recv,
            std::function<http::Response(std::exception&, http::Status)> on_error
        )
            :wait_for(WaitFor::Request),
            socket(static_cast<ip::tcp::socket&&>(socket)),
            on_recv(on_recv), on_error(on_error)
        {}

        Connection(Connection&& connection) 
            :socket(std::move(connection.socket)),
            on_recv(std::move(connection.on_recv))
        {}

        ~Connection() {
            this->shutdown();
        }
        void run();
    };

}