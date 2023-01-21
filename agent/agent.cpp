#include <iostream>
#include "toml.hpp"
#include "../powercap/dummy_powercap.cpp"

class Agent {
private:
    std::unique_ptr<PowercapInterface> powercap_interface_;

public:
    explicit Agent(std::unique_ptr<PowercapInterface> &&powercapInterface = {}) : powercap_interface_(std::move(powercapInterface)) {

    }
};

int main(int argc, char *argv[]) {
    auto config = toml::parse_file("../agent/config.toml");

    Agent agent(std::make_unique<DummyPowercap>());
    std::cout << "agent launched\n";
}
