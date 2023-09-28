#include "Route.h"

using namespace press;

Endpoint& Route::get_endpoint() {
    return this->endpoint;
}

http::Response Route::handle(http::Request&& req, Meta&& meta) {
    return this->handler(std::move(req), std::move(meta));
}