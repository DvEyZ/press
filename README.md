# Press - a simple C++ web framework

## Features
- [X] Requests and responses
- [X] Path parameters

## Dependencies

- Asio

## Example

```c++
#include <asio.hpp>
#include <press.hxx>

using namespace press;
using namespace press::http;
using namespace press::utils;

int main() {
    asio::io_context ctx;
    Server server(8000, ctx);
    
    server.route(MAKE_ROUTE(Method::GET, "/", [](Request&& req, Meta&& meta) -> Response {
        ResponseBuilder res;
        res.status(Status::OK);
        res.content_type("text/html");
        res.body("<p>Hello, world!</p>");
        return res.build();
    }));

    server.init();
    ctx.run();
    return 0;
}
```