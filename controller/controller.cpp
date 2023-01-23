#include <iostream>
#include <thread>

#include "toml.hpp"
#include "../sockpp/tcp_acceptor.h"

class Controller {
private:
    in_port_t port_ = 5757;
    int total_power_budget_;
    int total_agent_count_;

    void ParseConfig() {
        auto config = toml::parse_file("../controller/config.toml");
        total_power_budget_ = config["total_power_budget"].value_or(15);
        total_agent_count_ = config["total_agent_count"].value_or(2);
        port_ = config["port"].value_or(5757);
    }

    void HandleAgent(sockpp::tcp_socket socket) {}

public:
    explicit Controller() {
        ParseConfig();
        sockpp::initialize();
        sockpp::tcp_acceptor acc(port_);
        if (!acc) {
            std::cerr << "Error creating the acceptor: " << acc.last_error_str() << std::endl;
            return;
        }
        std::cout << "Awaiting connections on port " << port_ << "..." << std::endl;
        while (true) {
            sockpp::inet_address agent;
            // accept a new client connection from an agent
            sockpp::tcp_socket socket = acc.accept(&agent);
            std::cout << "Received a connection request from " << agent << std::endl;
            if (!socket) {
                std::cerr << "Error accepting incoming connection: "
                     << acc.last_error_str() << std::endl;
            }
            else {
                // Create a thread and transfer the new stream to it.
                std::thread thread(&Controller::HandleAgent, this, std::move(socket));
                thread.detach();
            }
        }
    }
};

int main() {
    auto config = toml::parse_file("../controller/config.toml");
    int power_budget = config["total_power_budget"].value_or(0);
    std::cout << power_budget << " W\n";


    Controller controller;

}
