#include <iostream>
#include <thread>
#include <chrono>

#include "toml.hpp"
#include "../powercap/dummy_powercap.cpp"

class Agent {
private:
    std::unique_ptr<PowercapInterface> powercap_interface_;
    int power_reading_period_;

    void AgentRunner() {
        std::cout << "Running Agent...\n";
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(power_reading_period_));
            float current_power = powercap_interface_->GetPowerReading();
            std::cout << "Send power reading " << current_power << "\n";

        }
    }

public:
    explicit Agent(std::unique_ptr<PowercapInterface> &&powercapInterface = {}) : powercap_interface_(std::move(powercapInterface)) {
//        std::thread background_thread(&Agent::AgentRunner, this);
        auto config = toml::parse_file("../agent/config.toml");
        power_reading_period_ = config["power_reading_period"].value_or(5);
        AgentRunner();
    }
};

int main(int argc, char *argv[]) {
    auto config = toml::parse_file("../agent/config.toml");

    // let's use DummyPowercap for now
    Agent agent(std::make_unique<DummyPowercap>());
    std::cout << "agent launched\n";
}
