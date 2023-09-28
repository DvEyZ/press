#include "Server.h"
#include "Connection.h"
#include "utils/ResponseBuilder.h"
#include <memory>
#include <iostream>

using namespace press;

Server::Server(unsigned short port, asio::io_context& context) 
    :port(port),
    context(context), 
    acceptor(ip::tcp::acceptor(this->context, ip::tcp::endpoint(ip::tcp::v4(), port)))
{}

void Server::route(std::unique_ptr<IRoute>&& route) {
    this->routes.push_back(std::move(route));
}

void Server::accept() {
    this->acceptor.async_accept([this] (const std::error_code e, ip::tcp::socket sock) {
        if(e) {
            std::cerr << "error: " << e << "\n";
        }
        
        auto conn = std::shared_ptr<Connection>(new Connection(std::move(sock), 
            [this] (http::Request&& req) -> http::Response {
                auto endpoint = Endpoint::FromPath(req.method, req.path.path_part());

                std::stringstream msg;
                msg << "\033[32mReceived request - " <<
                    http::method_to_string(endpoint.get_method()) << " " << endpoint.get_path() 
                << "\033[0m" << std::endl;
                std::cout << msg.str();

                int prox = 65535;
                auto handler = this->routes.end();
                for(unsigned int i = 0; i < this->routes.size(); i++) {
                    auto p = routes[i]->get_endpoint().get_proximity(endpoint);
                    if(p == -1) continue;

                    if(p <= prox) {
                        prox = p;
                        handler = this->routes.begin() + i;
                    }
                }

                if(handler == this->routes.end()) {
                    utils::ResponseBuilder res;
                    res.status(http::Status::NotFound);
                    res.content_type("text/plain");
                    res.body("URL not found.");
                    
                    std::stringstream msg;
                    msg << "\033[34m> \033[31mNo route matched - returned 404\033[0m" << std::endl;
                    std::cout << msg.str();
                    return res.build();
                }
                
                Meta meta;
                for(unsigned int i = 0; i < endpoint.get_segs().size(); i++) {
                    auto c = handler->get()->get_endpoint().get_segs()[i];
                    if(c.index() == 1) {
                        meta.params.emplace(std::get<Param>(c).name, std::get<std::string>(endpoint.get_segs()[i]));
                    }
                }
                
                std::stringstream msg2;
                msg2 << "\033[34m> \033[33mMatched route - " <<
                    http::method_to_string(handler->get()->get_endpoint().get_method()) << " " << handler->get()->get_endpoint().get_path() 
                << "\033[0m" << std::endl;
                std::cout << msg2.str();

                try {
                    auto res = handler->get()->handle(std::move(req), std::move(meta));
                    
                    std::stringstream msg;
                    msg << "\033[34m> \033[33mResponded - " << 
                        (int)res.status << " " << http::status_to_string(res.status) 
                    << "\033[0m" << std::endl;
                    std::cout << msg.str();

                    return res;
                } catch(std::exception& e) {
                    utils::ResponseBuilder res;
                    res.status(http::Status::Internal);
                    res.content_type("text/plain");

                    std::string body = "Error:\n";
                    body.append(e.what());
                    res.body(body);
                    
                    std::stringstream msg;
                    msg << "\033[34m> \033[31mException caught - " << e.what() << "\033[0m" << std::endl; 
                    std::cout << msg.str();

                    return res.build();
                }
            },
            [this] (std::exception& err, http::Status status) -> http::Response {
                utils::ResponseBuilder res;
                res.status(status);
                res.content_type("text/plain");
                
                std::stringstream body;
                body << "Error: " << static_cast<int>(status) << " " << http::status_to_string(status) << "\n" << std::string(err.what());
                res.body(body.str());

                std::stringstream msg;
                msg << "\033[31mError - " << err.what() << "\033[0m" << std::endl; 
                std::cout << msg.str();

                return res.build();
            }
        ));
        
        conn->run();
        this->accept();
    });
}

void Server::init() {
    this->accept();
    std::cout << "\033[1m\033[34mpress running on port " << this->port << "\033[0m" << std::endl;
}