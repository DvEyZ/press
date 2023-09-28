#include "Request.h"
#include <stdexcept>

using namespace press;

bool http::method_is_cacheable(Method method) {
    if(method == Method::GET || method == Method::HEAD) {
        return true;
    } else {
        return false;
    }
}

bool http::method_has_body(Method method) {
    if(method == Method::POST || method == Method::PUT || method == Method::PATCH) {
        return true;
    } else {
        return false;
    }
}

std::string http::method_to_string(Method method) {
    if(method == Method::GET) {
        return "GET";
    }
    if(method == Method::POST) {
        return "POST";
    }
    if(method == Method::PATCH) {
        return "PATCH";
    }
    if(method == Method::PUT) {
        return "PUT";
    }
    if(method == Method::HEAD) {
        return "HEAD";
    }
    if(method == Method::OPTIONS) {
        return "OPTIONS";
    }
    if(method == Method::TRACE) {
        return "TRACE";
    }
    if(method == Method::DELETE) {
        return "DELETE";
    }
    return "";
}

std::optional<http::Method> http::method_from_string(std::string str) {
    if(str == "GET") {
        return std::optional(Method::GET);
    }
    if(str == "POST") {
        return std::optional(Method::POST);
    }
    if(str == "PATCH") {
        return std::optional(Method::PATCH);
    }
    if(str == "PUT") {
        return std::optional(Method::PUT);
    }
    if(str == "HEAD") {
        return std::optional(Method::HEAD);
    }
    if(str == "OPTIONS") {
        return std::optional(Method::OPTIONS);
    }
    if(str == "TRACE") {
        return std::optional(Method::TRACE);
    }
    if(str == "DELETE") {
        return std::optional(Method::DELETE);
    }
    return std::optional<Method>();
}

http::HttpPath::HttpPath(std::string path) {
    auto pos = path.find("?");
    if(pos == std::string::npos) pos = path.length();
    std::string path_part = path.substr(0, pos);
    std::string query_part = (pos == path.length()) ? "" : path.substr(pos);
    if(!query_part.empty()) query_part.erase(0, 1);

    while(!path_part.empty()) {
        auto s_pos = path_part.find("/");
        if(s_pos == std::string::npos) s_pos = path_part.length();

        auto s = path_part.substr(0, s_pos);
        path_part.erase(0, s_pos + 1);
        if(s.empty()) continue;
        this->path.push_back(s);
    }

    while(!query_part.empty()) {
        auto q_pos = query_part.find("/");
        if(q_pos == std::string::npos) q_pos = query_part.length();

        auto param = query_part.substr(0, q_pos);
        query_part.erase(0, q_pos + 1);
        if(param.empty()) continue;

        auto eq_pos = param.find("=");
        std::string name = param.substr(0, eq_pos);
        std::string value = param.substr(eq_pos + 1);
        this->query.emplace(name, value);
    }
}

std::string http::HttpPath::path_part() const {
    std::string j;
    for(auto i :this->path) {
        j.append("/");
        j.append(i);
    }
    if(j.empty()) {
        j.append("/");
    }
    return j;
}

std::string http::HttpPath::query_part() const {
    std::string j;
    for(auto i :this->query) {
        j.append(i.first);
        j.append("=");
        j.append(i.second);
        j.append("&");
    }
    if(!j.empty() && j.at(j.length() - 1) == '&') {
        j.pop_back();
    }
    return j;
}

std::string http::HttpPath::join() const {
    std::string path = this->path_part();
    std::string query = this->query_part();
                
    if(query.empty()) return path;
    return path + "?" + query;  
}

void http::Request::add_first_line(std::string line) {
    auto pos = line.find(" ");
    if(pos == std::string::npos) pos = line.length();
    std::string method_s = line.substr(0, pos);
    line.erase(0, pos + 1);

    pos = line.find(" ");
    if(pos == std::string::npos) pos = line.length();
    std::string path_s = line.substr(0, pos);
    line.erase(0, pos + 1);

    std::string version_s = line.substr();

    if(path_s.empty()) {
        throw ParseError("no path provided");
    }
    
    auto method = http::method_from_string(method_s);
    if(!method.has_value()) {
        throw ParseError("invalid HTTP method");
    }
    auto version = http::version_from_string(version_s);
    if(!version.has_value()) {
        throw ParseError("invalid HTTP version");
    }
    auto path = HttpPath(path_s);

    this->version = version.value();
    this->method = method.value();
    this->path = path;
}

void http::Request::add_header_line(std::string line) {
    auto pos = line.find(":");
    if(pos == std::string::npos) pos = line.length() - 1;
    std::string name = line.substr(0, pos);
    std::string value_field = line.substr(pos + 1);

    auto vbegin = value_field.find_first_not_of(" \t");
    const std::string value = value_field.substr(vbegin);

    std::transform(name.begin(), name.end(), name.begin(), [](char v) {
        return tolower(v);
    });

    if(name == "content-length") {
        try {
            this->headers.content_length = std::stoi(value);
        } catch(std::invalid_argument& e) {
            throw ParseError("content-length header value must be a number");
        }
    } else if(name == "accept") {
        
    } else if(name == "accept-encoding") {
        
    } else if(name == "accept-language") {
        
    } else if(name == "connection") {
    
    } else if(name == "host") {
        
    }
    
    this->headers.raw.emplace(name, value);
}

const char* http::ParseError::what() const noexcept {
    return this->w.c_str();
}