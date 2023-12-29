#ifndef AED2324_PRJ2_G117_AIRLINE_H
#define AED2324_PRJ2_G117_AIRLINE_H

#include <string>

using namespace std;

class Airline{
private:
    string code;
    string name;
    string callsign;
    string country;
public:
    Airline(string code);
    Airline(string code , string name , string callsign , string country);

    const string &getCode() const;
    const string &getName() const;
    const string &getCallsign() const;
    const string &getCountry() const;

    void setCode(const string &code);
    void setName(const string &name);
    void setCallsign(const string &callsign);
    void setCountry(const string &country);

    /**
     * Checks if this airport has less priority than airport a2 by comparing their code using operator <
     * @param a2 airport to be compared
     * @return true if this airport has less priority than airport a2, else false
     */
    bool operator<(const Airline& a2) const;
    /**
     * Checks if this airport is equal to airport a2 by comparing their code using operator ==
     * @param a2 airport to be compared
     * @return true if this airport is equal to airport a2, else false
     */
    bool operator== (const Airline& a2) const;
};

#endif //AED2324_PRJ2_G117_AIRLINE_H
