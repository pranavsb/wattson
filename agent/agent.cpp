#include <iostream>
#include "toml.hpp"

class Agent {

};

int main(int argc, char *argv[]) {
    auto config = toml::parse_file( "../agent/config.toml" );

    Agent agent = Agent();
    std::cout << "agent launched\n";
}