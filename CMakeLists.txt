cmake_minimum_required(VERSION 3.23)
project(AEDproject2)

set(CMAKE_CXX_STANDARD 14)

add_executable(AEDproject2 main.cpp src/FlightManager.h src/FlightManager.cpp src/Airport.cpp src/Airport.h src/graph.cpp src/graph.h src/Airline.cpp src/Airline.h)
