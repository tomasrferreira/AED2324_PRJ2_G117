#include "Airport.h"

Airport::Airport(string code) {
    this->code = code;
}

Airport::Airport(string code, string name, string city, string country, double latitude, double longitude) {
    this->code = code;
    this->name = name;
    this->city = city;
    this->country = country;
    this->latitude = latitude;
    this-> longitude = longitude;
    node = 0;
}

const string &Airport::getCode() const {
    return code;
}

void Airport::setCode(const string &code) {
    this->code = code;
}

const string &Airport::getName() const {
    return name;
}

void Airport::setName(const string &name) {
    this->name = name;
}
