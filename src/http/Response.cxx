#include "Response.h"
#include <sstream>

using namespace press;

std::string http::status_to_string(http::Status status) {
    switch(static_cast<int>(status))
    {
    case 100:
        return "Continue";
    case 101:
        return "Switching Protocols";
    case 200:
        return "OK";
    case 201:
        return "Created";
    case 202:
        return "Accepted";
    case 204:
        return "No Content";
    case 301:
        return "Moved Permanently";
    case 302:
        return "Found";
    case 303:
        return "See Other";
    case 304:
        return "Not Modified";
    case 307:
        return "Temporary Redirect";
    case 308:
        return "Permanent Redirect";
    case 400:
        return "Bad Request";
    case 401:
        return "Unauthorized";
    case 402:
        return "Payment Required";
    case 403:
        return "Forbidden";
    case 404:
        return "Not Found";
    case 405:
        return "Method Not Allowed";
    case 406:
        return "Not Acceptable";
    case 408:
        return "Request Timeout";
    case 409:
        return "Conflict";
    case 410:
        return "Gone";
    case 411:
        return "Length Required";
    case 412:
        return "Precondition Failed";
    case 413:
        return "Payload Too Large";
    case 414:
        return "URI Too Long";
    case 415:
        return "Unsupported Media Type";
    case 418:
        return "I'm A Teapot";
    case 426:
        return "Upgrade Required";
    case 429:
        return "Too Many Requests";
    case 431:
        return "Request Header Fields Too Large";
    case 451:
        return "Unavailable For Legal Reasons";
    case 500:
        return "Internal Server Error";
    case 501:
        return "Not Implemented";
    case 503:
        return "Service Unavailable";
    }
    return "";
}

std::string http::Response::to_string() {
    std::stringstream stream;
    
    stream << http::version_to_string(this->version) << " "
        << static_cast<int>(this->status) << " "
        << http::status_to_string(this->status) << "\r\n";
    
    for(auto i : this->headers.raw) {
        stream << i.first << ": " << i.second << "\r\n";
    }

    stream << "\r\n";

    if(!this->body.empty()) {
        stream << this->body;
    }

    return stream.str();
}