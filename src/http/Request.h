#pragma once
#include "http.h"
#include <string>
#include <vector>
#include <map>
#include <optional>
#include <exception>
#include <concepts>

namespace press {
    namespace http {
        enum class Method {
            GET, HEAD, POST, PUT, PATCH, OPTIONS, TRACE, DELETE
        };

        bool method_is_cacheable(Method method);
        bool method_has_body(Method method);
        std::string method_to_string(Method method);
        std::optional<Method> method_from_string(std::string string);

        struct HttpPath {
            std::vector<std::string> path;
            std::map<std::string, std::string> query;

            HttpPath() {};
            HttpPath(std::string path);

            std::string path_part() const;
            std::string query_part() const;
            std::string join() const;
        };

        struct RequestHeaders {
            std::optional<int> content_length; // Content-Length
            std::optional<std::string> connection; // Connection
            std::optional<std::string> host;   // Host
            std::map<std::string, std::string> raw;

            RequestHeaders() {};
        };

        struct Request {
            Version version;
            Method method;
            HttpPath path;
            std::string body;
            RequestHeaders headers;

            Request() {};

            void add_first_line(std::string line);
            void add_header_line(std::string line);
        };

        class ParseError : public std::exception {
            std::string w;
        public:
            ParseError(std::string w) :w(w) {}
            const char* what() const noexcept override;
        };
    }
}