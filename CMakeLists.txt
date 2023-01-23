cmake_minimum_required(VERSION 3.20)
project(wattson)

set(CMAKE_CXX_STANDARD 17)

add_executable(wattson_agent
        agent/agent.cpp "powercap/powercap_interface.cpp" powercap/dummy_powercap.cpp)

add_executable(wattson_controller
        controller/controller.cpp)

target_link_libraries(wattson_agent ${PROJECT_SOURCE_DIR}/libs/libsockpp.a)
target_link_libraries(wattson_controller ${PROJECT_SOURCE_DIR}/libs/libsockpp.a)




