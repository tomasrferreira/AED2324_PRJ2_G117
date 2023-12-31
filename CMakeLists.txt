cmake_minimum_required(VERSION 3.23)
project(AEDproject2)

set(CMAKE_CXX_STANDARD 14)

add_executable(AEDproject2 src/main.cpp src/Flight.h src/Flight.cpp src/Airport.cpp src/Airport.h src/Graph.cpp src/Graph.h src/Airline.cpp src/Airline.h)
