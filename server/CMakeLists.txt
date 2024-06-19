cmake_minimum_required(VERSION 3.10.2)

project(server)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES "*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})