#pragma once

#include <optional>

namespace clickhouse {
    /// List of hostnames with service ports
    struct Endpoint {
        std::string host;
        std::optional<unsigned int> port = std::nullopt;
    };
}