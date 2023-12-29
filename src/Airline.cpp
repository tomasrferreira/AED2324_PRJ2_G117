#include "Airline.h"

Airline::Airline(string code) {
    this->code = code;
}
Airline::Airline(string code, string name, string callsign, string country) {
    this->code = code;
    this->name = name;
    this->callsign = callsign;
    this->country = country;
}

const string &Airline::getName() const {return name;}

const string &Airline::getCallsign() const {return callsign;}

const string &Airline::getCountry() const {return country;}

const string &Airline::getCode() const {return code;}

void Airline::setCode(const string &code) {Airline::code = code;}

void Airline::setName(const string &name) {Airline::name = name;}

void Airline::setCallsign(const string &callsign) {Airline::callsign = callsign;}

void Airline::setCountry(const string &country) {Airline::country = country;}

bool Airline::operator<(const Airline& a2) const {
    return code < a2.code;
}

bool Airline::operator==(const Airline& a2) const{
    return code == a2.code;
}