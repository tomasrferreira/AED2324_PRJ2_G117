#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <string>
#include <list>
#include <cmath>
#include <vector>
#include <set>
#include <unordered_map>
#include "Airline.h"
#include "Airport.h"

using namespace std;

/**
 * Class that represents the main graph
 */
class Graph {
    int n{};       // size of graph
    unordered_map<string,Airport*> airports; // nodes
    unordered_map<string,Airline*> airlines;
public:
    void readFiles(const std::string& file1, const std::string& file2, const std::string& file3);
    static double calculateDistance(Airport* a1, Airport* a2);
    Airport* bestTravel(Airport* origin, Airport* destination);
    void bestTravelAirport(Airport* origin, Airport* destination);
    void bestTravelCity(const string& origin, const string& destination);
    unordered_map<string,Airport*> getAirports();
    int getNumberOfFlightsForAirport(const string &airportCode);
    int getNumberOfAirlinesAirport(const string &airportCode);
    void listAirlines(const string &airportCode);
    void listFlights(const string &airportCode);
    int getNumberOfReachableCities(const string &airportCode);
    int getNumberOfReachableCountries(const string &airportCode);
    Airport *airlineBestTravel(Airport *origin, Airport *destination, const string &airlineCode);
    void oneAirlineBestTravel(Airport *origin, Airport *destination, const string &airlineCode);
    Airport *multipleAirlineBestTravel(Airport *origin, Airport *destination, const vector<string> &airlineCodes);
    void multipleAirlinesPrint(Airport *origin, Airport *destination, const vector<string> &airlineCodes);
    int multipleFlightsReachableCities(const string &airportCode, int numFlights);
    int multipleFlightsReachableCountries(const string &airportCode, int numFlights);
};

#endif