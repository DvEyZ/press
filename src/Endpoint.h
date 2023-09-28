#pragma once
#include <string>
#include <variant>
#include <span>
#include <regex>
#include "http/Request.h"

namespace press {
    struct Param {
        std::string name;
    };

    class Endpoint {
        http::Method method;
        std::vector<std::variant<std::string, Param>> segs;
    
    public:
        Endpoint() {};

        static Endpoint FromPath(http::Method method, std::string path);
        Endpoint(http::Method method, std::string path);

        int get_proximity(const Endpoint& other) const;
        http::Method get_method() const;
        std::string get_path() const;
        const std::vector<std::variant<std::string, Param>>& get_segs() const;
    };
}