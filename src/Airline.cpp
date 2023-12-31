#include "Airline.h"

Airline::Airline(string code, string name, string callSign, string country) {
    this->code = code;
    this->name = name;
    this->callSign = callSign;
    this->country = country;
}

void Airline::setCode(string code) {
    this->code = code;
}

string Airline::getCode() const {
    return this->code;
}

void Airline::setName(string name) {
    this->name = name;
}

string Airline::getName() const {
    return this->name;
}

void Airline::setCallSign(string callSign) {
    this->callSign = callSign;
}

string Airline::getCallSign() const {
    return this->callSign;
}

void Airline::setCountry(string country) {
    this->country = country;
}

string Airline::getCountry() const {
    return this->country;
}

bool Airline::operator<(const Airline &a1) const {
    return this->code < a1.getCode();
}