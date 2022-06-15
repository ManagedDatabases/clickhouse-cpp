#pragma once

#include "../exceptions.h"

#include <optional>
#include <queue>
#include <iostream>
namespace clickhouse {
class NetworkAddress;

    /// List of hostnames with service ports
    struct Endpoint {
        std::string host;
        std::optional<unsigned int> port = std::nullopt;
    };

    class EndpointConnector {
    public:
        std::vector<Endpoint> endpoints;
	explicit EndpointConnector(std::vector<Endpoint> endpoints) {
	std::cout << "connector constructor" << endpoints.size() << std::endl;
		this->endpoints = endpoints;
	}

        struct Iterator {
	    Iterator() {
		endpoints_ = nullptr;
                finished_ = true;
	    }
	    Iterator(const std::vector<Endpoint> *endpoints, const std::vector<Endpoint>::const_iterator & start_with, bool finished = false) {
                if (finished) {
		    finished_ = true;
		}
		it_ = start_with;
		start_with_ = start_with;
		endpoints_ = endpoints;
	    }
            Iterator& operator++() {
		++it_;
                if (it_ == endpoints_->end()) {
		    it_ = endpoints_->begin();
	        }
	        if (it_ == start_with_) {
		    finished_ = true;
	        }	    
		return *this;
	    }

            bool operator!=(const Iterator& other) {
		std::cout << "compare start" << std::endl;
		if (finished_ && other.finished_) {
		    return false;
		}
		std::cout << "first" << std::endl;
		if (finished_ != other.finished_) {
		    return true;
		}
		std::cout << "second" << std::endl;
	        if (other.it_ != it_) {
                    return true;
		}
		std::cout << "third" << std::endl;
		return false;
	    }
            const Endpoint& operator*() const {
		std::cout << "dereference" << std::endl;
	    	return *it_;
	    }

	    std::vector<Endpoint>::const_iterator getInsideIterator() {
		return it_;
	    }

	private:
	    bool finished_ = false;
	    std::vector<Endpoint>::const_iterator start_with_;
	    std::vector<Endpoint>::const_iterator it_;
	    const std::vector<Endpoint> *endpoints_;
        };

        Iterator begin() const {
	    return Iterator(&endpoints, endpoints.begin());
	}
        Iterator end() const {
	    return Iterator(&endpoints, endpoints.begin(), true);
	}

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
                end_ = Iterator(&endpoints, current_endpoint_.getInsideIterator(), true);
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
