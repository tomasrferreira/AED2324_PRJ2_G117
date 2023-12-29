#ifndef AED2324_PRJ2_G117_FLIGHTMANAGER_H
#define AED2324_PRJ2_G117_FLIGHTMANAGER_H

#include "Airport.h"
#include "Airline.h"
#include <unordered_set>
#include <fstream>
#include <iostream>
#include <sstream>
#include <set>
#include <string>
#include "graph.h"


typedef unordered_set<Airport,airportHash,airportHash> airportTable;

class FlightManager{
private:
    airportTable airports;
    set<Airline> airlines;
    Graph flights;
public:
    FlightManager();
    void lerFicheiros();
    void showAirportInfoMenu();
    void showAirlineMenu();
    void showStartingPointMenu();
    void showDestinationMenu();
    void askWhichPlaceMenu();
    void askForAirport();
    void askInfoToTakeFromAirport(const Airport& airport);

    /**
     * Shows in the standard output the number of flights that leave airport
     * @param airport
     */
    bool numberOfFlights(const Airport& airport);
    /**
     * Shows in the standard output the number of airlines that have flights that leave airport
     * @param airport
     */
    bool numberOfAirlines(const Airport &airport);
    /**
     * Shows in the standard output the number of different destinies one can go to from airport
     * @param airport
     */
    bool numberOfDestinies(const Airport &airport);
    bool numberOfDestinyCountries(const Airport &airport);
    bool numberReachable(const Airport &airport);
    bool airportsReachable(const Airport &airport, int number_of_flights);
    bool citiesReachable(const Airport &airport, int number_of_flights);
    bool countriesReachable(const Airport &airport, int amount_of_flights);
    void startingPoint();
    void destination(list<int> airportsStartingPoint);
    void askForAirportStartingPoint();
    void askForAirportDestination(list<int> airportsStartingPoint);
    void askForCityStartingPoint();
    void askForCityDestination(list<int> airportsStartingPoint);
    void askForLocationStartingPoint();
    void askForLocationDestination(list<int> airportsStartingPoint);
    void airline(list<int> airportsStartingPoint, list<int> airportsDestination);
    void askForAirline(list<int> airportsStartingPoint, list<int> airportsDestination, list<string> airlinesList);
    void askForOtherInfo();
    void askForOtherInfoMenu();
    void numberOfCountries();
    void numberOfCities();
    Airline getAirline(const string& code);
    void averageAirportsByCountry();
    void showBestTrajectories(const list<int>& s , const list<int>& d , const list<string>& a);
    void numberOfFlights();
    void airportMostFlights();
    void diameter();
    void articulationPoints();
    void airportMostAirlines();
};

#endif //AED2324_PRJ2_G117_FLIGHTMANAGER_H
