#include "Airport.h"

Airport::Airport(string code, string name, string city, string country, double latitude, double longitude,
                 bool visited) {
    this->code = code;
    this->name = name;
    this->city = city;
    this->country = country;
    this->latitude = latitude;
    this->longitude = longitude;
    this->visited = visited;
}

double Airport::getDistance() const {
    return distance;
}

void Airport::setDistance(double distance) {
    Airport::distance = distance;
}

bool Airport::isVisited() const {
    return visited;
}

void Airport::setVisited(bool visited) {
    Airport::visited = visited;
}


void Airport::setCode(const string &code) {
    Airport::code = code;
}

void Airport::setName(const string &name) {
    Airport::name = name;
}

void Airport::setCity(const string &city) {
    Airport::city = city;
}

void Airport::setCountry(const string &country) {
    Airport::country = country;
}

void Airport::setLatitude(double latitude) {
    Airport::latitude = latitude;
}

void Airport::setLongitude(double longitude) {
    Airport::longitude = longitude;
}

const string &Airport::getCode() const {
    return code;
}

const string &Airport::getName() const {
    return name;
}

const string &Airport::getCity() const {
    return city;
}

const string &Airport::getCountry() const {
    return country;
}

double Airport::getLatitude() const {
    return latitude;
}

double Airport::getLongitude() const {
    return longitude;
}

void Airport::addFlight(Airport *destination, Airline *airline) {
    flights.emplace_back(destination, airline);
}

vector<Flight> Airport::getFlights() const {
    return flights;
}

vector<pair<Airport *, Airline *>> Airport::getScales() const {
    return scales;
}

void Airport::setScales(vector<pair<Airport *, Airline *>> scales) {
    Airport::scales = scales;
}

void Airport::setFlights(const vector<Flight> &flights) {
    Airport::flights = flights;
}

