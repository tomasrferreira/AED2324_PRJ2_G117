#include "Flight.h"

Flight::Flight(Airport *destination, Airline *airline) {
    this->destination = destination;
    this->airline = airline;
}

Airport *Flight::getDestination() const {
    return destination;
}

void Flight::setDestination(Airport *destination) {
    Flight::destination = destination;
}

Airline *Flight::getAirline() const {
    return airline;
}

void Flight::setAirline(Airline *airline) {
    Flight::airline = airline;
}

double Flight::getDistance() const {
    return distance;
}

void Flight::setDistance(double distance) {
    Flight::distance = distance;
}