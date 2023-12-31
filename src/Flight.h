#ifndef FLIGHT_H
#define FLIGHT_H

#include "Airport.h"
#include "Airline.h"

class Airport;

/*
 * Class that represents the flights (a edge in the graph)
 */
class Flight {
    Airport *destination; // destination node
    Airline *airline;
    double distance{};
public:
    Flight() = default;

    Flight(Airport *destination, Airline *airline);

    Airport *getDestination() const;

    void setDestination(Airport *destination);

    Airline *getAirline() const;

    void setAirline(Airline *airline);

    double getDistance() const;

    void setDistance(double distance);
};

#endif
