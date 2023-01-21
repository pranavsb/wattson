#include <iostream>
#include "toml.hpp"

class Controller {

};

int main() {
    auto config = toml::parse_file("../controller/config.toml");
    int power_budget = config["total_power_budget"].value_or(0);
    std::cout << power_budget << " W\n";
}
