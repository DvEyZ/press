#pragma once
#include "http.h"
#include <string>
#include <optional>
#include <map>

namespace press {
    namespace http {
        enum class Status {
            Continue = 100,
            SwitchingProtocols = 101,

            OK = 200,
            Created = 201,
            Accepted = 202,
            NoContent = 204,
            
            MovedPermanently = 301,
            Found = 302,
            SeeOther = 303,
            NotModified = 304,
            TemporaryRedirect = 307,
            PermanentRedirect = 308,

            BadRequest = 400,
            Unauthorized = 401,
            PaymentRequired = 402,
            Forbidden = 403,
            NotFound = 404,
            MethodNotAllowed = 405,
            NotAcceptable = 406,
            RequestTimeout = 408,
            Conflict = 409,
            Gone = 410,
            LengthRequired = 411,
            PreconditionFailed = 412,
            PayloadTooLarge = 413,
            UriTooLong = 414,
            UnsupportedMediaType = 415,
            ImaTeapot = 418,
            UpgradeRequired = 426,
            TooManyRequests = 429,
            ReqHeaderFieldsTooLarge = 431,
            LegalReasons = 451,

            Internal = 500,
            NotImplemented = 501,
            ServiceUnavailable = 503
        };

        std::string status_to_string(Status status);

        struct ResponseHeaders {
            std::map<std::string, std::string> raw;
        };

        struct Response {
            Version version;
            Status status;
            ResponseHeaders headers;
            std::string body;

            std::string to_string();
        };
    }
}