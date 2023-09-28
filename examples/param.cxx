#include <press.hxx>
#include <asio.hpp>
#include <iostream>

using namespace press;
using namespace press::http;
using namespace press::utils;

int main() {
    asio::io_context ctx;
    auto server = Server(9000, ctx);

    server.route(MAKE_ROUTE(
        Method::GET, "/<name>/<value>", 
        [] ([[maybe_unused]] Request&& req, Meta&& meta) -> Response {
            std::stringstream body;
          
            for(auto& i : meta.params) {
                body << i.first << " " << i.second << "\n";
            }

            ResponseBuilder res;
            res.status(Status::OK);
            res.content_type("text/plain");
            res.body(body.str());
            return res.build();
        }
    ));
    
    server.init();
    ctx.run();
    return 0;
}