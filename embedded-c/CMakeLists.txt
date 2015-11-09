cmake_minimum_required(VERSION 3.3)
project(Enno-device-sdk-embedded-c)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(samples/simple-pubsub)