#ifndef AIRLINE_H
#define AIRLINE_H

#include <string>

using namespace std;

/**
 * Class that represents the airlines
 */
class Airline {
    string code, name, callSign, country;
public:
    Airline() = default;

    Airline(string code, string name, string callSign, string country);

    void setCode(string code);

    string getCode() const;

    void setName(string name);

    string getName() const;

    void setCallSign(string callSign);

    string getCallSign() const;

    void setCountry(string country);

    string getCountry() const;

    bool operator<(const Airline &a1) const;
};

#endif
