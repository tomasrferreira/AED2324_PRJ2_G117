#include "Graph.h"
#include <queue>
#include <fstream>
#include <sstream>

/**
 * Function that reads the info from the csv files
 * Time complexity: O(n), n - length of csv file
 */
void Graph::readFiles(const string &file1, const string &file2, const string &file3) {
    std::fstream in1, in2, in3;
    std::string token;
    Airport *currentAirport;
    Airline *currentAirline;


    in1.open(file1);
    in2.open(file2);
    in3.open(file3);

    std::getline(in1, token, '\n'); //step var names ahead
    std::getline(in2, token, '\n');  //step var names ahead
    std::getline(in3, token, '\n');  //step var names ahead


    //reads airports.csv
    while (std::getline(in1, token, '\n')) {
        std::stringstream iss(token);
        std::vector<std::string> temp;
        std::string tempstr;
        currentAirport = new Airport();

        while ((std::getline(iss, tempstr, ','))) {
            if (!tempstr.empty() && tempstr[tempstr.size() - 1] == '\r')
                tempstr.erase(tempstr.size() - 1);
            temp.push_back(tempstr);
        }

        currentAirport->setCode(temp[0]);
        currentAirport->setName(temp[1]);
        currentAirport->setCity(temp[2]);
        currentAirport->setCountry(temp[3]);
        currentAirport->setLatitude(stod(temp[4]));
        currentAirport->setLongitude(stod(temp[5]));

        airports[currentAirport->getCode()] = currentAirport;
        n++;
    }


    //read airlines.csv
    while (std::getline(in2, token, '\n')) {
        std::stringstream iss(token);
        std::vector<std::string> temp;
        std::string tempstr;
        currentAirline = new Airline();

        while ((std::getline(iss, tempstr, ','))) {
            if (!tempstr.empty() && tempstr[tempstr.size() - 1] == '\r')
                tempstr.erase(tempstr.size() - 1);
            temp.push_back(tempstr);
        }

        currentAirline->setCode(temp[0]);
        currentAirline->setName(temp[1]);
        currentAirline->setCallSign(temp[2]);
        currentAirline->setCountry(temp[3]);

        airlines[currentAirline->getCode()] = currentAirline;
    }


    //reads flights.csv
    while (std::getline(in3, token, '\n')) {
        std::stringstream iss(token);
        std::vector<std::string> temp;
        std::string tempstr;
        Airport *origin, *destination;
        Airline *airline;

        while ((std::getline(iss, tempstr, ','))) {
            if (!tempstr.empty() && tempstr[tempstr.size() - 1] == '\r')
                tempstr.erase(tempstr.size() - 1);
            temp.push_back(tempstr);
        }

        origin = airports[temp[0]];
        destination = airports[temp[1]];
        airline = airlines[temp[2]];
        origin->addFlight(destination, airline);
    }

}

unordered_map<string, Airport *> Graph::getAirports() {
    return airports;
}

/**
 * Function that calculates the distance between two airports in km
 * It is used for the bfs search between airports
 * Time complexity: O(1)
 */
double Graph::calculateDistance(Airport *a1, Airport *a2) {
    double lat1 = a1->getLatitude();
    double lat2 = a2->getLatitude();
    double lon1 = a1->getLongitude();
    double lon2 = a2->getLongitude();
    int earthRadius = 6371000; // metres
    double latitudeSource = lat1 * M_PI / 180;
    double latitudeDestination = lat2 * M_PI / 180;
    double latitudeDiff = (lat2 - lat1) * M_PI / 180;
    double longitudeDiff = (lon2 - lon1) * M_PI / 180;

    double a = sin(latitudeDiff / 2) * sin(latitudeDiff / 2) +
               cos(latitudeSource) * cos(latitudeDestination) *
               sin(longitudeDiff / 2) * sin(longitudeDiff / 2);

    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return earthRadius * c / 1000; // in km
}

/**
 * Function that implements the BFS algorithm to search the destination airport
 * Time complexity: O(|V| + |E|), |V| - number of vertices in the graph (airports), |E| - number of edges in the graph (flights)
 */
Airport *Graph::bestTravel(Airport *origin, Airport *destination) {
    for (auto it: airports) it.second->setVisited(false);
    for (auto it: airports) it.second->setScales({});
    queue<Airport *> q; // queue of unvisited nodes
    q.push(origin);
    origin->setVisited(true);
    origin->setDistance(0);
    while (!q.empty()) { // while there are still unvisited nodes
        Airport *u = q.front();
        q.pop();
        for (auto e: u->getFlights()) {
            Airport *w = e.getDestination();
            vector<pair<Airport *, Airline *>> current;
            pair<Airport *, Airline *> curr;
            if (!w->isVisited()) {
                q.push(w);
                w->setVisited(true);
                double dist = calculateDistance(w, u);
                w->setDistance(u->getDistance() + dist);
                for (auto scale: u->getScales())
                    current.push_back(scale);
                curr.first = u;
                curr.second = e.getAirline();
                current.push_back(curr);
                w->setScales(current);
            }
            if (w == destination)
                return w;
        }
    }
    Airport *empty{};
    return empty;
}

/**
 * Function that calls the bestTravel function and searches the destination airport
 * Prints best flight
 * Time complexity: O(|V| + |E|), |V| - number of vertices in the graph (airports), |E| - number of edges in the graph (flights)
 */
void Graph::bestTravelAirport(Airport *origin, Airport *destination) {
    Airport *w = bestTravel(origin, destination);
    cout << "Flight from " << origin->getName() << " - " << origin->getCode() << " to "
         << destination->getName() << " - " << destination->getCode() << endl;
    cout << "Number of flights: " << w->getScales().size() << endl;
    cout << "Path: ";
    for (auto scale: w->getScales())
        cout << scale.first->getName() << " - " << scale.first->getCode() <<
             " --(flying with " << scale.second->getName() << " - " << scale.second->getCode() << ")--> ";
    cout << destination->getName() << " - " << destination->getCode() << endl;
}

/**
 * Function that calls the bestTravel function and searches the destination city
 * Prints the best flight options
 * Time complexity: O(n^2)
 */
void Graph::bestTravelCity(const string &origin, const string &destination) {
    vector<Airport *> originA;
    vector<Airport *> destinationA;
    int min = INT_MAX;

    vector<Airport *> resOrigin;
    vector<Airport *> resDestination;
    vector<Airport *> res;


    for (const auto &airport: airports) {
        if (airport.second->getCity() == origin)
            originA.push_back(airport.second);

        if (airport.second->getCity() == destination)
            destinationA.push_back(airport.second);
    }

    for (auto pOrigin: originA) {
        for (auto pDestination: destinationA) {
            Airport *check = bestTravel(pOrigin, pDestination);
            if (check->getScales().size() < min) {
                resOrigin.clear();
                resDestination.clear();
                resOrigin.push_back(pOrigin);
                resDestination.push_back(pDestination);
                min = check->getScales().size();
            } else if (check->getScales().size() == min) {
                resOrigin.push_back(pOrigin);
                resDestination.push_back(pDestination);
            }
        }
    }

    cout << "\nBest flights options from " << origin << " to " << destination << ": " << endl << "\n";
    for (int i = 0; i < resOrigin.size(); i++) {
        res.push_back(bestTravel(resOrigin[i], resDestination[i]));
        cout << "From " << resOrigin[i]->getName() << " - " << resOrigin[i]->getCode() << " to "
             << resDestination[i]->getName() << " - " << resDestination[i]->getCode();
        cout << "\nNumber of flights needed: " << res.front()->getScales().size() << endl;
        cout << "Path: ";
        for (auto scale: res[i]->getScales())
            cout << scale.first->getName() << " - " << scale.first->getCode() <<
                 " --(flying with " << scale.second->getName() << " - " << scale.second->getCode() << ")--> ";
        cout << resDestination[i]->getName() << " - " << resDestination[i]->getCode() << endl << "\n";
    }
}


/**
 * Function that implements the BFS algorithm to search the destination airport using just one airline
 * Time complexity: O(|V| + |E|), |V| - number of vertices in the graph (airports), |E| - number of edges in the graph (flights)
 */
Airport *Graph::airlineBestTravel(Airport *origin, Airport *destination, const string &airlineCode) {
    for (auto it: airports) it.second->setVisited(false);
    for (auto it: airports) it.second->setScales({});
    queue<Airport *> q; // queue of unvisited nodes
    q.push(origin);
    origin->setVisited(true);
    origin->setDistance(0);
    while (!q.empty()) { // while there are still unvisited nodes
        Airport *u = q.front();
        q.pop();
        for (auto e: u->getFlights()) {
            // Check if the current flight uses the specified airline
            if (e.getAirline()->getCode() != airlineCode) {
                continue;
            }

            Airport *w = e.getDestination();
            vector<pair<Airport *, Airline *>> current;
            pair<Airport *, Airline *> curr;
            if (!w->isVisited()) {
                q.push(w);
                w->setVisited(true);
                double dist = calculateDistance(w, u);
                w->setDistance(u->getDistance() + dist);
                for (auto scale: u->getScales())
                    current.push_back(scale);
                curr.first = u;
                curr.second = e.getAirline();
                current.push_back(curr);
                w->setScales(current);
            }
            if (w == destination)
                return w;
        }
    }
    Airport *empty{};
    return empty;
}


/**
 * Function that gives the user the best flight using just one airline
 * Time complexity: O(n)
 */
void Graph::oneAirlineBestTravel(Airport *origin, Airport *destination, const string &airlineCode) {
    Airport *w = airlineBestTravel(origin, destination, airlineCode);
    if (w == nullptr) {
        cout << "There is no flight from " << origin->getName() << " - " << origin->getCode() << " to "
             << destination->getName() << " - " << destination->getCode() << " using only " << airlineCode << endl;
        return;
    } else {
        cout << "Flight from " << origin->getName() << " - " << origin->getCode() << " to "
             << destination->getName() << " - " << destination->getCode() << endl;
        cout << "Number of flights: " << w->getScales().size() << endl;
        cout << "Path: ";
        for (auto scale: w->getScales())
            cout << scale.first->getName() << " - " << scale.first->getCode() <<
                 " --(flying with " << scale.second->getName() << " - " << scale.second->getCode() << ")--> ";
        cout << destination->getName() << " - " << destination->getCode() << endl;
    }
}

/**
 * Function that gives the user the best flight using just multiple airlines by his choice, uses BFS
 * Time complexity: O(|V| + |E|), |V| - number of vertices in the graph (airports), |E| - number of edges in the graph (flights)
 */
Airport *Graph::multipleAirlineBestTravel(Airport *origin, Airport *destination, const vector<string> &airlineCodes) {
    for (auto it: airports) it.second->setVisited(false);
    for (auto it: airports) it.second->setScales({});
    queue<Airport *> q; // queue of unvisited nodes
    q.push(origin);
    origin->setVisited(true);
    origin->setDistance(0);
    while (!q.empty()) { // while there are still unvisited nodes
        Airport *u = q.front();
        q.pop();
        for (auto e: u->getFlights()) {
            // Check if the current flight uses one of the specified airlines
            if (find(airlineCodes.begin(), airlineCodes.end(), e.getAirline()->getCode()) == airlineCodes.end()) {
                continue;
            }

            Airport *w = e.getDestination();
            vector<pair<Airport *, Airline *>> current;
            pair<Airport *, Airline *> curr;
            if (!w->isVisited()) {
                q.push(w);
                w->setVisited(true);
                double dist = calculateDistance(w, u);
                w->setDistance(u->getDistance() + dist);
                for (auto scale: u->getScales())
                    current.push_back(scale);
                curr.first = u;
                curr.second = e.getAirline();
                current.push_back(curr);
                w->setScales(current);
            }
            if (w == destination)
                return w;
        }
    }
    Airport *empty{};
    return empty;
}

/**
 * Function that prints the flight using multiple airlines from function multipleAirlineBestTravel
 * Time complexity: O(n)
 */
void Graph::multipleAirlinesPrint(Airport *origin, Airport *destination, const vector<string> &airlineCodes) {
    Airport *w = multipleAirlineBestTravel(origin, destination, airlineCodes);
    if (w == nullptr) {
        cout << "There is no flight from " << origin->getName() << " - " << origin->getCode() << " to "
             << destination->getName() << " - " << destination->getCode() << " using only the specified airlines"
             << endl;
        return;
    } else {
        cout << "Flight from " << origin->getName() << " - " << origin->getCode() << " to "
             << destination->getName() << " - " << destination->getCode() << endl;
        cout << "Number of flights: " << w->getScales().size() << endl;
        cout << "Path: ";
        for (auto scale: w->getScales())
            cout << scale.first->getName() << " - " << scale.first->getCode() <<
                 " --(flying with " << scale.second->getName() << " - " << scale.second->getCode() << ")--> ";
        cout << destination->getName() << " - " << destination->getCode() << endl;
    }
}

/**
 * Function that returns the number of flights that depart from the specified airport
 * Time complexity: O(1)
 */
int Graph::getNumberOfFlightsForAirport(const string &airportCode) {
    if (airports.find(airportCode) == airports.end())
        return -1;

    Airport *airport = airports[airportCode];
    return airport->getFlights().size();
}

/**
 * Function that lists the flights that depart from the specified airport
 * Time complexity: O(n)
 */
void Graph::listFlights(const string &airportCode) {
    if (airports.find(airportCode) == airports.end())
        return;

    Airport *airport = airports[airportCode];
    for (auto flight: airport->getFlights())
        cout << flight.getAirline()->getName() << "(" << flight.getAirline()->getCode() << ") - " << airportCode
             << " -> "
             << flight.getDestination()->getCode() << " (" << flight.getDestination()->getCity() << ") " << endl;
}

/**
 * Function that returns the number of airlines that have planes that depart from the specified airport
 * Time complexity: O(n)
 */
int Graph::getNumberOfAirlinesAirport(const string &airportCode) {
    if (airports.find(airportCode) == airports.end())
        return -1;

    list<Airline *> airlines;
    Airport *airport = airports[airportCode];
    for (auto flight: airport->getFlights()) {
        Airline *airline = flight.getAirline();
        airlines.push_back(airline);
    }

    // Remove duplicate entries from the list
    airlines.sort();
    airlines.unique();

    return airlines.size();
}

/**
 * Function that lists the airlines that have planes that depart from the specified airport
 * Time complexity: O(n)
 */
void Graph::listAirlines(const string &airportCode) {
    if (airports.find(airportCode) == airports.end())
        return;

    list<Airline *> airlines;
    Airport *airport = airports[airportCode];
    for (auto flight: airport->getFlights()) {
        Airline *airline = flight.getAirline();
        airlines.push_back(airline);
    }

    // Remove duplicate entries from the list
    airlines.sort();
    airlines.unique();

    for (auto airline: airlines)
        cout << airline->getName() << " - " << airline->getCode() << endl;
}

/**
 * Function that returns the number of the reachable cities from the specified airport using just one flight
 * Time complexity: O(n)
 */
int Graph::getNumberOfReachableCities(const string &airportCode) {
    if (airports.find(airportCode) == airports.end())
        return -1;

    list<string> reachableCities;
    Airport *currentAirport = airports[airportCode];
    for (auto flight: currentAirport->getFlights()) {
        Airport *destination = flight.getDestination();
        reachableCities.push_back(destination->getCity());
    }

    reachableCities.sort();
    reachableCities.unique();
    return reachableCities.size();
}

/**
 * Function that returns the number of the reachable countries from the specified airport using just one flight
 * Time complexity: O(n)
 */
int Graph::getNumberOfReachableCountries(const string &airportCode) {
    if (airports.find(airportCode) == airports.end())
        return -1;

    list<string> reachableCountries;
    Airport *airport = airports[airportCode];
    for (auto &flight: airport->getFlights())
        reachableCountries.push_back(flight.getDestination()->getCountry());

    reachableCountries.sort();
    reachableCountries.unique();
    return reachableCountries.size();
}

/**
 * Function that returns the number of the reachable cities from the specified airport using multiple flights
 * Time complexity: O(n^2)
 */
int Graph::multipleFlightsReachableCities(const string &airportCode, int numFlights) {
    if (airports.find(airportCode) == airports.end())
        return -1;

    list<string> reachableCities;
    queue<Airport *> airportsToVisit;
    airportsToVisit.push(airports[airportCode]);

    int flightsTaken = 0;
    while (!airportsToVisit.empty()) {
        Airport *currentAirport = airportsToVisit.front();
        airportsToVisit.pop();

        if (find(reachableCities.begin(), reachableCities.end(), currentAirport->getCity()) == reachableCities.end()) {
            reachableCities.push_back(currentAirport->getCity());

            if (flightsTaken < numFlights) {
                for (auto flight: currentAirport->getFlights()) {
                    Airport *destination = flight.getDestination();
                    airportsToVisit.push(destination);
                }
                flightsTaken++;
            }
        }
    }
    reachableCities.sort();
    reachableCities.unique();

    return reachableCities.size();
}

/**
 * Function that returns the number of the reachable countries from the specified airport using multiple flights
 * Time complexity: O(n^2)
 */
int Graph::multipleFlightsReachableCountries(const string &airportCode, int numFlights) {
    if (airports.find(airportCode) == airports.end())
        return -1;

    list<string> reachableCountries;
    queue<Airport *> airportsToVisit;
    airportsToVisit.push(airports[airportCode]);

    int flightsTaken = 0;
    while (!airportsToVisit.empty()) {
        Airport *currentAirport = airportsToVisit.front();
        airportsToVisit.pop();

        if (find(reachableCountries.begin(), reachableCountries.end(), currentAirport->getCountry()) ==
            reachableCountries.end()) {
            reachableCountries.push_back(currentAirport->getCountry());

            if (flightsTaken < numFlights) {
                for (auto flight: currentAirport->getFlights()) {
                    Airport *destination = flight.getDestination();
                    airportsToVisit.push(destination);
                }
                flightsTaken++;
            }
        }
    }
    reachableCountries.sort();
    reachableCountries.unique();

    return reachableCountries.size();
}