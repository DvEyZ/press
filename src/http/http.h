#pragma once
#include <string>
#include <optional>

namespace press {
    namespace http {
        
        enum class Version {
            HTTP_0_9,
            HTTP_1_0,
            HTTP_1_1,
            HTTP_2_0,
        };

        std::string version_to_string(Version version);
        std::optional<Version> version_from_string(std::string str);

    }
}