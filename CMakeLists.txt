cmake_minimum_required(VERSION 3.23)
project(wattson)

set(CMAKE_CXX_STANDARD 17)

include_directories(agent)

add_executable(wattson_agent
        agent/agent.cpp)

add_executable(wattson_controller
        controller/controller.cpp)



