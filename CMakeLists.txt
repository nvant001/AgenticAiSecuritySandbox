cmake_minimum_required(VERSION 3.15)
project(AgenticAiSecuritySandbox)

# Enable M_PI and other math constants for MSVC
add_compile_definitions(_USE_MATH_DEFINES)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Eigen3 CONFIG REQUIRED)

add_executable(${PROJECT_NAME} main.cpp
EnvironmentGraph.cpp
Agent.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE Eigen3::Eigen)