#include "toml.hpp"

class controller {

};

int main() {
    auto config = toml::parse_file( "config.toml" );
}
