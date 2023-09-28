#include "http.h"

using namespace press;

std::string http::version_to_string(Version version) {
    if(version == Version::HTTP_0_9) return "HTTP/0.9";
    if(version == Version::HTTP_1_0) return "HTTP/1.0";
    if(version == Version::HTTP_1_1) return "HTTP/1.1";
    if(version == Version::HTTP_2_0) return "HTTP/2.0";
    return "";
}

std::optional<http::Version> http::version_from_string(std::string str) {
    if(str == "HTTP/0.9") return std::optional(http::Version::HTTP_0_9);
    if(str == "HTTP/1.0") return std::optional(http::Version::HTTP_1_0);
    if(str == "HTTP/1.1") return std::optional(http::Version::HTTP_1_1);
    if(str == "HTTP/2.0") return std::optional(http::Version::HTTP_2_0);

    return std::optional<http::Version>();
}