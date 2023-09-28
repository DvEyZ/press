#include "Endpoint.h"

using namespace press;

Endpoint Endpoint::FromPath(http::Method method, std::string path) {
    Endpoint e;
    auto path_http = http::HttpPath(path);
    for(auto& i : path_http.path) {
        e.segs.push_back(std::variant<std::string, Param>(i));
    }
    e.method = method;
    return e;
}

Endpoint::Endpoint(http::Method method, std::string path) :method(method) {
    auto path_http = http::HttpPath(path);
    auto param_pattern = std::regex("<.*>");
    for(auto& i : path_http.path) {
        if(std::regex_match(i, param_pattern)) {
            auto param_name = i.substr(1, i.length() - 2);
            segs.push_back(std::variant<std::string, Param>(Param { param_name }));
        } else {
            segs.push_back(std::variant<std::string, Param>(i));
        }
    }
}

int Endpoint::get_proximity(const Endpoint& other) const {
    if(other.segs.size() != this->segs.size()) {
        return -1;
    }

    if(other.method != this->method) {
        return -1;
    }

    int param_count = 0;
    for(unsigned int i = 0; i < this->segs.size(); i++) {
        auto& this_seg = this->segs[i];
        auto& other_seg = other.segs[i];
        if(this_seg.index() == 0 && other_seg.index() == 0) {
            auto t = std::get<std::string>(this_seg);
            auto o = std::get<std::string>(other_seg);
            if(t != o) {
                return -1;
            }
        } else {
            param_count += 1;
        }
    }

    return param_count;
}

http::Method Endpoint::get_method() const {
    return this->method;
}

std::string Endpoint::get_path() const {
    std::string o;
    for(auto& i : this->segs) {
        o.append("/");
        if(i.index() == 0) {
            o.append(std::get<std::string>(i));
        } else {
            o.append("<");
            o.append(std::get<Param>(i).name);
            o.append(">");
        }
    }
    if(o.empty()) o = "/";
    return o;
}

const std::vector<std::variant<std::string, Param>>& Endpoint::get_segs() const {
    return this->segs;
}