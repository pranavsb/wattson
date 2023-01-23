#include <iostream>
#include <thread>
#include <chrono>

#include "toml.hpp"
#include "../powercap/dummy_powercap.cpp"
#include "../sockpp/tcp_connector.h"

class Agent {
private:
    std::unique_ptr<PowercapInterface> powercap_interface_;

    std::string controller_endpoint_;
    in_port_t controller_port_;
    bool is_primary_;
    ssize_t power_reading_period_;


    void SendPowerReadings(sockpp::tcp_connector conn) {
        // TODO note that this implementation is currently not portable across machines (differing endianness or 32 vs 64 bit machines)
        int buf_size = sizeof(int) + sizeof(float);
        // send int 0 or 1 to indicate replica or primary followed by power reading float
        char msgbuf[buf_size];
        while (true) {
            float current_power = powercap_interface_->GetPowerReading();
            std::cout << "Send power reading " << current_power << std::endl;
            int is_primary = is_primary_ ? 1 : 0;

            std::memcpy(msgbuf, &is_primary, sizeof(int));
            std::memcpy(msgbuf + sizeof(int), &current_power, sizeof(float));
            if (conn.write(msgbuf, buf_size) != buf_size) {
                std::cout << "Error sending power reading" << std::endl;
                break;
            }
            std::this_thread::sleep_for(std::chrono::seconds(power_reading_period_));
        }
    }

    void ReceivePowercap(sockpp::tcp_socket read_socket) {
        // TODO note that this implementation is currently not portable across machines (differing endianness or 32 vs 64 bit machines)
        std::cout << "Receive powercap" << std::endl;
        float powercap;
        while (read_socket.read_n(&powercap, sizeof(powercap)) > 0) {
            // received new powercapping instruction from server
            std::cout << "New powercapping instruction from server " << powercap << std::endl;
            powercap_interface_->SetPowercap(powercap);
            std::cout << "Set new powercap " << powercap << std::endl;
        }
        std::cout << "Shutdown socket" << std::endl;
        read_socket.shutdown();
    }

    void ParseConfig() {
        auto config = toml::parse_file("../agent/config.toml");
        controller_endpoint_ = config["controller_endpoint"].value_or("localhost");
        controller_port_ = config["controller_port"].value_or(5757);
        is_primary_ = config["is_primary"].value_or(false);
        power_reading_period_ = config["power_reading_period"].value_or(5);
        // todo parse all values in config
    }

public:
    explicit Agent(std::unique_ptr<PowercapInterface> &&powercapInterface = {}) : powercap_interface_(std::move(powercapInterface)) {
        ParseConfig();
        sockpp::initialize();

        // Agent keeps trying to connect if connection breaks
        while (true) {
            sockpp::tcp_connector conn({controller_endpoint_, controller_port_});
            if (!conn) {
                // retry after one second
                // TODO something like exponential backoff?
                std::this_thread::sleep_for(std::chrono::seconds(1));
                continue; // retry conn
            }
            std::cout << "Connected to controller" << std::endl;

            // Create a read thread and send socket clone
            // Read thread will wait for new powercap instructions from controller
            // Write thread will periodically send power readings
            std::thread read_thread(&Agent::ReceivePowercap, this, std::move(conn.clone()));
            SendPowerReadings(std::move(conn));
            read_thread.join();
        }
    }
};

int main(int argc, char *argv[]) {
    // let's use DummyPowercap for now
    Agent agent(std::make_unique<DummyPowercap>());
}
