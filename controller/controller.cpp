#include <iostream>
#include <thread>

#include "toml.hpp"
#include "../sockpp/tcp_acceptor.h"

class Controller {
private:
    in_port_t port_ = 5757;
    ssize_t total_power_budget_;
    ssize_t total_agent_count_;

    void ParseConfig() {
        auto config = toml::parse_file("../controller/config.toml");
        total_power_budget_ = config["total_power_budget"].value_or(15);
        total_agent_count_ = config["total_agent_count"].value_or(2);
        port_ = config["port"].value_or(5757);
    }

    void HandleAgent(sockpp::tcp_socket socket, sockpp::inet_address agent_addr) {
        // TODO note that this implementation is currently not portable across machines (differing endianness or 32 vs 64 bit machines)
        int buf_size = sizeof(int) + sizeof(float);
        char msgbuf[buf_size];

        int is_primary;
        float power_reading;

        int counter = 0;

        while (socket.read_n(&msgbuf, buf_size) > 0) {
            std::memcpy(&is_primary, msgbuf, sizeof(int));
            std::memcpy(&power_reading, msgbuf + sizeof(int), sizeof(float));
            std::cout << agent_addr << " sent is_primary:" << is_primary << " power reading: " << power_reading << std::endl;

            counter++;
            if (counter % 3 == 0) {
                // once every n times
                float new_powercap = 9.0;
                socket.write(&new_powercap, sizeof(float));
            }
        }
    }

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
            } else {
                // Create a thread and transfer the new stream to it.
                std::thread thread(&Controller::HandleAgent, this, std::move(socket), agent);
                thread.detach();
            }
        }
    }
};

int main() {
    Controller controller;
}
