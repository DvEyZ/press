#pragma once
#include "../http/Response.h"

namespace press {
    namespace utils {

        class ResponseBuilder {
            http::Response res;
        public:
            ResponseBuilder() {
                res.version = http::Version::HTTP_1_1;
                res.status = http::Status::OK;
            }

            void status(http::Status status) {
                res.status = status;
            }

            void content_type(std::string type) {
                res.headers.raw.emplace("Content-Type", type + ";charset=utf-8");
            }

            void body(std::string body) {
                res.headers.raw.emplace("Content-Length", std::to_string(body.length()));
                res.body = body;
            }

            void header(std::string name, std::string value) {
                res.headers.raw.emplace(name, value);
            }

            http::Response&& build() {
                return std::move(res);
            } 
        };

    }
}