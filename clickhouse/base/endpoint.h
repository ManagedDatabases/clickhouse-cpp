#pragma once

#include "../exceptions.h"

#include <optional>

class NetworkAddress;

namespace clickhouse {
    /// List of hostnames with service ports
    struct Endpoint {
        std::string host;
        std::optional<unsigned int> port = std::nullopt;
    };

    class EndpointConnector {
    public:
        std::vector<Endpoint> endpoints;

        struct Iterator {
            Iterator& operator++();

            bool operator!=(const Iterator& other);
            Endpoint& operator*() const;
        };

        Iterator begin() const;
        Iterator end() const;

        bool isConnected();

        void setCurrentEndpoint(const Iterator& iter) const {
            current_endpoint_ = iter;
        }
        Endpoint getCurrentEndpoint();

        void setNetworkAddress(std::shared_ptr<NetworkAddress> addr) const {
            addr_ = addr;
        }
        std::shared_ptr<NetworkAddress> getNetworkAddress() {
            return addr_;
        }


        enum ReconnectType {
            ONLY_CURRENT,
            ALL
        };

        ReconnectType getReconnectType() {
            return reconnectType_;
        }

        void setReconnectType(ReconnectType reconnectType) {
            if (reconnectType == ONLY_CURRENT) {
                begin_ = current_endpoint_;
                end_ = current_endpoint_;
                ++end_;
            } else if (reconnectType == ALL) {
                begin_ = current_endpoint_;
                // TODO
                end_ = current_endpoint_;
            } else {
                throw AssertionError("There is no such Reconnect Type: " + std::to_string(reconnectType));
            }
        }

        ReconnectType reconnectType_;
        Iterator begin_;
        Iterator end_;
        mutable Iterator current_endpoint_;
        mutable std::shared_ptr<NetworkAddress> addr_;
    };
}