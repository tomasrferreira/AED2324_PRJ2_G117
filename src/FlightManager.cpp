#include "FlightManager.h"

/**
 * Reads files and builds graph with flights,hash table with airports and list with airlines.
 *
 * Complexity : O(|V|+|A|+|E|) where |V|,|E| and |A| is the cardinality of set of airports,flights and airlines
 * respectively.
 */
void FlightManager::lerFicheiros() {
    cout << "Loading airports...\n";
    ifstream airports_file;
    airports_file.open("../resources/airports.csv");
    if (!airports_file.is_open()){
        cout << "File not found\n";
        return;
    }
    list<Airport> temp;
    string line;
    getline(airports_file,line);
    while(getline(airports_file, line)){
        string code , name , city , country;
        double latitude , longitude;
        istringstream ss(line);
        getline(ss,code,',');
        getline(ss,name , ',');
        getline(ss,city , ',');
        getline(ss,country,',');
        ss >> latitude;
        ss.ignore(1);
        ss >> longitude;
        Airport airport(code , name , city , country , latitude , longitude);
        temp.push_back(airport);
    }
    airports_file.close();
    cout << "There are " << temp.size() <<  " airports!\n";

    cout << "Loading airlines...\n";
    ifstream airlines_file;
    airlines_file.open("../resources/airlines.csv");
    if (!airlines_file.is_open()){
        cout << "File not found\n";
        return;
    }
    getline(airlines_file,line);
    while(getline(airlines_file, line)){
        string code , name , callsign , country;
        istringstream ss(line);
        getline(ss,code,',');
        getline(ss,name , ',');
        getline(ss,callsign,',');
        getline(ss,country);

        Airline airline(code , name , callsign , country);
        airlines.insert(airline);
    }
    airlines_file.close();
    cout << "There are " << airlines.size() <<  " airlines!\n";

    cout << "Loading flights...\n";
    ifstream flights_file;
    flights_file.open("../resources/flights.csv");
    if (!flights_file.is_open()){
        cout << "File not found\n";
        return;
    }
    flights = Graph((int)temp.size(),true);
    auto ptr = temp.begin();
    for (int i = 1; i <= temp.size(); i++){
        flights.setAirport(i,ptr->getCode());
        ptr->setNode(i);
        ptr++;
    }
    for (const Airport& airport : temp)
        airports.insert(airport);
    getline(flights_file,line);
    while(getline(flights_file, line)){
        string source,target,airline;
        istringstream ss(line);
        getline(ss,source,',');
        getline(ss,target,',');
        getline(ss,airline);
        auto source_pointer = airports.find(Airport(source));
        auto target_pointer = airports.find(Airport(target));
        flights.addEdge(source_pointer->getNode(),target_pointer->getNode(),airline);
    }
    flights_file.close();
    cout << "There are " << flights.getNumFlightsTotal() <<  " flights!\n\n";
}

FlightManager::FlightManager() : flights(0){}


void FlightManager::askForAirport() {
    cout << "What is the code of the airport that you want information about?\n";
    string code;
    cin >>code;
    if (cin.fail()){
        cin.clear();
        cin.ignore(256,'\n');
        code = "";
    }
    Airport airport(code);
    auto airport_pointer = airports.find(airport);
    if (airport_pointer == airports.end()) {
        cout << "Invalid Input!\n";
        return;
    }
    askInfoToTakeFromAirport(*airport_pointer);
}


void FlightManager::startingPoint(){
    bool keepRunning = true;
    while(keepRunning){
        showStartingPointMenu();
        int option;
        cin >> option;
        if (cin.fail()){
            cin.clear();
            cin.ignore(256,'\n');
            option = 0;
        }
        switch (option){
            case 1:
                askForAirportStartingPoint();
                break;
            case 2:
                askForCityStartingPoint();
                break;
            case 3:
                askForLocationStartingPoint();
                break;
            case 4:
                return;
            default:
                cout << "!!Invalid Input!!\n\n";
        }

    }
}

void FlightManager::destination(list<int> airportsStartingPoint){
    bool keepRunning = true;
    while(keepRunning){
        showDestinationMenu();
        int option;
        cin >> option;
        if (cin.fail()){
            cin.clear();
            cin.ignore(256,'\n');
            option = 0;
        }
        switch (option){
            case 1:
                askForAirportDestination(airportsStartingPoint);
                break;
            case 2:
                askForCityDestination(airportsStartingPoint);
                break;
            case 3:
                askForLocationDestination(airportsStartingPoint);
                break;
            case 4:
                return;
            default:
                cout << "!!Invalid Input!!\n\n";
        }

    }
}

void FlightManager::airline(list<int> airportsStartingPoint, list<int> airportsDestination){
    bool keepRunning = true;
    list<string> airlines;
    while(keepRunning){
        showAirlineMenu();
        int option;
        cin >> option;
        if (cin.fail()){
            cin.clear();
            cin.ignore(256,'\n');
            option = 0;
        }
        switch (option){
            case 1:
                askForAirline(airportsStartingPoint, airportsDestination, airlines);
                break;
            case 3:
                return;
            case 2:
                showBestTrajectories(airportsStartingPoint,airportsDestination,{});
                break;
            default:
                cout << "!!Invalid Input!!\n\n";
        }

    }
}

void FlightManager::askForAirline(list<int> airportsStartingPoint, list<int> airportsDestination, list<string> airlinesList){
    cout << "What is the code of the airline you want to fly with?\n";
    string code;
    cin >>code;
    if (cin.fail()){
        cin.clear();
        cin.ignore(256,'\n');
        code = "";
    }
    Airline airline(code);
    auto airline_pointer = airlines.find(airline);
    if (airline_pointer == airlines.end()) {
        cout << "Invalid Input!\n";
        return;
    }

    airlinesList.push_back(airline_pointer->getCode());

    cout << "Do you want to fly with another airline as well? (y/n)\n";
    string answer;
    cin >> answer;
    if (cin.fail()){
        cin.clear();
        cin.ignore(256,'\n');
        answer = "";
    }

    if (answer == "yes" || answer == "Yes" || answer == "YES" || answer == "y" || answer == "Y"){
        askForAirline(airportsStartingPoint, airportsDestination, airlinesList);
    }
    else if (answer == "no" || answer == "No" || answer == "NO" || answer == "n" || answer == "N"){
        showBestTrajectories(airportsStartingPoint,airportsDestination,airlinesList);
    }
    else{
        cout << "Invalid Input!\n";
        return;
    }
}


void FlightManager::askForLocationStartingPoint(){
    cout << "What is the latitude of the location that you want to start from?\n";
    double latitude;
    cin >> latitude;
    cout << "What is the longitude of the location that you want to start from?\n";
    double longitude;
    cin >> longitude;
    if (cin.fail()){
        cin.clear();
        cin.ignore(256,'\n');
        latitude = 0;
        longitude = 0;
    }
    double closestDistance = INT_MAX;
    string closestAirport;
    string airportCode;

    for(auto airport = airports.begin(); airport != airports.end(); airport++){
        double distance = airport -> distance(latitude,longitude);
        if (distance < closestDistance){
            closestDistance = distance;
            closestAirport = airport -> getName();
            airportCode = airport -> getCode();
        }
    }

    cout << "The closest airport to the location you entered is " << closestAirport << " with a distance of " << closestDistance << " km.\n";

    int n = airports.find(airportCode) ->getNode();

    list<int> airportsStartingPoint;
    airportsStartingPoint.push_back(n);

    destination(airportsStartingPoint);
}
void FlightManager::askForCityStartingPoint(){
    cout << "What is the city that you want to start from?\n";
    string city;
    cin >> city;
    if (cin.fail()){
        cin.clear();
        cin.ignore(256,'\n');
        city = "";
    }
    vector<Airport> airports_in_city;
    for (const Airport& airport : airports){
        if (airport.getCity() == city)
            airports_in_city.push_back(airport);
    }
    if (airports_in_city.empty()){
        cout << "There are no airports in that city!\n";
        return;
    }
    cout << "The airports in that city are:\n";
    for (const Airport& airport : airports_in_city){
        cout << airport.getCode() << ',' << airport.getName() << endl;
    }
    list<int> airportsStartingPoint;

    for(auto& x : airports_in_city){
        airportsStartingPoint.push_back(x.getNode());
    }
    destination(airportsStartingPoint);
}

void FlightManager::askForAirportStartingPoint(){
    cout << "What is the code of the airport you are starting from?\n";
    string code;
    cin >>code;
    if (cin.fail()){
        cin.clear();
        cin.ignore(256,'\n');
        code = "";
    }
    Airport airport(code);
    auto airport_pointer = airports.find(airport);
    if (airport_pointer == airports.end()) {
        cout << "Invalid Input!\n";
        return;
    }

    list<int> airportsStartingPoint;
    airportsStartingPoint.push_back(airport_pointer->getNode());

    destination(airportsStartingPoint);
}

void FlightManager::askForAirportDestination(list<int> airportsStartingPoint){
    cout << "What is the code of the airport you want to go to?\n";
    string code;
    cin >>code;
    if (cin.fail()){
        cin.clear();
        cin.ignore(256,'\n');
        code = "";
    }
    Airport airport(code);
    auto airport_pointer = airports.find(airport);
    if (airport_pointer == airports.end()) {
        cout << "Invalid Input!\n";
        return;
    }

    list<int> airportsDestination;
    airportsDestination.push_back(airport_pointer->getNode());

    airline(airportsStartingPoint, airportsDestination);
}

void FlightManager::askForCityDestination(list<int> airportsStartingPoint){
    cout << "What is the city that you want to go to?\n";
    string city;
    cin >> city;
    if (cin.fail()){
        cin.clear();
        cin.ignore(256,'\n');
        city = "";
    }
    vector<Airport> airports_in_city;
    for (const Airport& airport : airports){
        if (airport.getCity() == city)
            airports_in_city.push_back(airport);
    }
    if (airports_in_city.empty()){
        cout << "There is no airport in that city!\n";
        return;
    }
    cout << "The airports in that city are:\n";
    for (const Airport& airport : airports_in_city){
        cout << airport.getName() << endl;
    }

    list<int> airportsDestination;
    for (auto& x: airports_in_city){
        airportsDestination.push_back(x.getNode());
    }

    airline(airportsStartingPoint, airportsDestination);
}

void FlightManager::askForLocationDestination(list<int> airportsStartingPoint){
    cout << "What is the latitude of the location that you want to go to?\n";
    double latitude;
    cin >> latitude;
    cout << "What is the longitude of the location that you want to go to?\n";
    double longitude;
    cin >> longitude;
    if (cin.fail()){
        cin.clear();
        cin.ignore(256,'\n');
        latitude = 0;
        longitude = 0;
    }
    double closestDistance = 1000000;
    string closestAirport;
    string airportCode;

    for (const Airport& airport: airports){

        double distance = airport.distance(latitude, longitude);

        if (distance < closestDistance){
            closestDistance = distance;
            closestAirport = airport.getName();
            airportCode = airport.getCode();
        }
    }

    list<int> airportsDestination;
    airportsDestination.push_back(airports.find(airportCode)->getNode());

    cout << "The closest airport to the location you entered is " << closestAirport << " with a distance of " << closestDistance << " km.\n";

    airline(airportsStartingPoint, airportsDestination);
}

void FlightManager::askInfoToTakeFromAirport(const Airport& airport) {
    bool keepRunning = true;
    while(keepRunning){
        showAirportInfoMenu();
        int option;
        cin >> option;
        if (cin.fail()){
            cin.clear();
            cin.ignore(256,'\n');
            option = 0;
        }
        switch (option){
            case 1:
                keepRunning = numberOfFlights(airport);
                break;
            case 2:
                keepRunning = numberOfAirlines(airport);
                break;
            case 3:
                keepRunning = numberOfDestinies(airport);
                break;
            case 4:
                keepRunning = numberOfDestinyCountries(airport);
                break;
            case 5:
                keepRunning = numberReachable(airport);
                break;
            case 6:
                return;
            default :
                cout << "Invalid Input!\n";
        }

    }
}
/**
 * Shows in the standard output the number of flights that leave airport
 * @param airport
 */
bool FlightManager::numberOfFlights(const Airport& airport) {
    int node = airport.getNode();
    list<pair<int , string>> flights_and_destination = flights.getFlights(node);
    cout << "There is " << flights_and_destination.size()<< " flights that leave airport " <<
         airport.getName() << ".\n";
    /*
    for (pair<int,string> p : flights_and_destination){
        cout << " - " << p.first << "(" << p.second << ")\n";
    }
     */
    return true;
}
/**
 * Shows in the standard output the number of airlines that have flights that leave airport
 * @param airport
 */
bool FlightManager::numberOfAirlines(const Airport &airport) {
    int node = airport.getNode();
    list<pair<int , string>> flights_and_destination = flights.getFlights(node);
    set<string> airlines_codes;
    for (const pair<int,string>& p : flights_and_destination){
        airlines_codes.insert(p.second);
    }
    cout << "There is " << airlines_codes.size() << " different airlines that have flights that leave airport " <<
         airport.getName() << ".\n";
    return true;
}
/**
 * Shows in the standard output the number of different destinies one can go to from airport
 * @param airport
 */
bool FlightManager::numberOfDestinies(const Airport &airport) {
    int node = airport.getNode();
    list<pair<int , string>> flights_and_destination = flights.getFlights(node);
    set<int> airport_nodes;
    for (pair<int,string> p : flights_and_destination){
        airport_nodes.insert(p.first);
    }
    cout<< "There is " <<  airport_nodes.size() << " different destinies that the flights from airport "<<
        airport.getName() << " have.\n";
    return true;
}

bool FlightManager::numberOfDestinyCountries(const Airport &airport) {
    int node = airport.getNode();
    list<pair<int , string>> flights_and_destination = flights.getFlights(node);
    set<int> airport_nodes;
    for (pair<int,string> p : flights_and_destination){
        airport_nodes.insert(p.first);
    }
    set<string> countries;
    for (int airport_node : airport_nodes){
        string airport_code = flights.getAirport(airport_node);
        Airport temp_airport(airport_code);
        auto airport_pointer = airports.find(temp_airport);
        countries.insert(airport_pointer->getCountry());
    }
    cout<< "There is " <<  countries.size() << " different countries that the flights from airport "<<
        airport.getName() << " go to.\n";
    return true;
}
/**
 * Shows in the standard output the number of airports one can go to from airport with at most a number provided by the user
 * @param airport
 * @return
 */
bool FlightManager::numberReachable(const Airport &airport) {
    cout << "What is the maximum amount of flights that we should consider?\n";
    int amount_of_flights;
    cin >>amount_of_flights;
    if (cin.fail()){
        cin.clear();
        cin.ignore(256,'\n');
        amount_of_flights = -1;
    }
    if (amount_of_flights <= 0 ) {
        cout << "Invalid Input!\n";
        return true;
    }
    bool keepRunning = true;
    while(keepRunning){
        askWhichPlaceMenu();
        int option;
        cin >> option;
        if (cin.fail()){
            cin.clear();
            cin.ignore(256,'\n');
            option = 0;
        }
        switch (option){
            case 1:
                keepRunning = airportsReachable(airport , amount_of_flights);
                break;
            case 2:
                keepRunning = citiesReachable(airport,amount_of_flights);
                break;
            case 3:
                keepRunning = countriesReachable(airport,amount_of_flights);
                break;
            case 4:
                return true;
            default :
                cout << "Invalid input!\n";
        }
    }
    return true;
}

bool FlightManager::airportsReachable(const Airport &airport, int number_of_flights) {
    int node = airport.getNode();
    list<string> airports_reachable = flights.getAirportsReachable(node , number_of_flights);
    cout << "There are " << airports_reachable.size() << " airports reachable from airport " << airport.getName() <<
         " with, at max, " <<number_of_flights << " flights!\n";
    return true;
}
bool FlightManager::citiesReachable(const Airport &airport, int number_of_flights) {
    int node = airport.getNode();
    list<string> airports_reachable = flights.getAirportsReachable(node , number_of_flights);
    set<pair<string,string>> cities_reachable;
    for (const string& airport_code : airports_reachable){
        Airport temp_airport(airport_code);
        auto airport_pointer = airports.find(temp_airport);
        cities_reachable.insert({airport_pointer->getCity(),airport_pointer->getCountry()});
    }
    cout << "There are " << cities_reachable.size() << " cities reachable from airport " << airport.getName() <<
         " with, at max, " <<number_of_flights << " flights!\n";
    return true;
}
bool FlightManager::countriesReachable(const Airport &airport, int amount_of_flights) {
    int node = airport.getNode();
    list<string> airports_reachable = flights.getAirportsReachable(node , amount_of_flights);
    set<string> countries_reachable;
    for (const string& airport_code : airports_reachable){
        Airport temp_airport(airport_code);
        auto airport_pointer = airports.find(temp_airport);
        countries_reachable.insert(airport_pointer->getCountry());
    }
    cout << "There are " << countries_reachable.size() << " countries reachable from airport " << airport.getName() <<
         " with, at max, " <<amount_of_flights << " flights!\n";
    return true;
}

void FlightManager::askForOtherInfo(){
    bool KeepRunning = true;
    while(KeepRunning){
        askForOtherInfoMenu();
        int option;
        cin >> option;
        if (cin.fail()){
            cin.clear();
            cin.ignore(256,'\n');
            option = 0;
        }
        switch (option){
            case 1:
                numberOfCountries();
                break;
            case 2:
                numberOfCities();
                break;
            case 3:
                numberOfFlights();
                break;
            case 4:
                averageAirportsByCountry();
                break;
            case 5:
                airportMostFlights();
                break;
            case 6:
                airportMostAirlines();
                break;
            case 7:
                diameter();
                break;
            case 8:
                articulationPoints();
                break;
            case 9:
                KeepRunning = false;
                break;
            default:
                cout << "Invalid input!\n";
        }
    }
}
/**
 * Calculates and prints the number of countries that are reachable by plane.
 *
 * Complexity : O(|V|) where |V| is the cardinality of set of airports.
 */
void FlightManager::numberOfCountries(){
    cout << "These are the countries reachable by plane:";
    unordered_set<string> unique_countries;
    for (auto& x : airports){
        if (unique_countries.find(x.getCountry()) == unique_countries.end()) {
            cout << x.getCountry() << "\n";
            unique_countries.insert(x.getCountry());
        }
    }
    cout << "Total: " << unique_countries.size() << " Countries\n";
}


void FlightManager::numberOfCities(){
    cout << "These are the cities reachable by plane:";
    unordered_set<string> unique_countries_cities;
    for (auto& x : airports){
        string city_country = x.getCity() + " , " + x.getCountry();
        if (unique_countries_cities.find(city_country) == unique_countries_cities.end()) {
            cout << city_country << "\n";
            unique_countries_cities.insert(city_country);
        }
    }
    cout << "Total: " << unique_countries_cities.size() << " Cities\n";
}

void FlightManager::numberOfFlights(){
    cout << "There are " << flights.getNumFlightsTotal() << " flights in the database!\n";
}
/**
 * Calculates and prints the average amount of airports per country.
 *
 * Complexity : O(|V|) where |V| is the cardinality of set of airports.
 */
void FlightManager::averageAirportsByCountry(){
    unordered_set<string> unique_countries;
    for (auto& x : airports){
        if (unique_countries.find(x.getCountry()) == unique_countries.end())
            unique_countries.insert(x.getCountry());
    }
    float average = (float) airports.size() / unique_countries.size();

    cout << "The average number of airports in a country is: " << average << "\n";
}

/**
 * Calculates and prints the airport with the most flights.
 *
 * Complexity : O(|V|*(|V|+|E|)) where |V| and |E| are the number of airports and flights respectively.
 */
void FlightManager::airportMostFlights(){
    int maxFlights = 0;
    int currentFlights = 0;
    string maxAirport;
    for (auto& x : airports){
        currentFlights = flights.getNumFlightsAirport(x.getNode());

        if (currentFlights > maxFlights){
            maxFlights = currentFlights;
            maxAirport = x.getName();
        }
    }
    cout << "The airport with the most flights is " << maxAirport << " with " << maxFlights << " flights either arriving or leaving it" << "\n";
}
/**
 * Calculates the airline with the most amount of flights of the network of flights and shows the result on the standard
 * output.
 *
 * Complexity : O(|V|*(|V|+|E|)) where |V| and |E| are the number of airports and flights respectively.
 */
void FlightManager::airportMostAirlines(){
    int maxAirlines = 0;
    int currentAirlines = 0;
    string maxAirport;
    for (auto& x : airports){
        currentAirlines = flights.getNumAirlinesAirport(x.getNode());

        if (currentAirlines > maxAirlines){
            maxAirlines = currentAirlines;
            maxAirport = x.getName();
        }
    }
    cout << "The airport with the most airlines is " << maxAirport << " with flights from " << maxAirlines << " airlines either arriving or leaving it" << "\n";
}
/**
 * Prints the diameter of the graph.
 */
void FlightManager::diameter(){
    pair<pair<string, string>, int> diameter = flights.diameter();

    cout << "The diameter of the graph is " << diameter.second << ", the starting flight is from " << airports.find(diameter.first.first)->getName() << " airport \nand the last flight lands in " << airports.find(diameter.first.second)->getName() << " airport.\n";
}

/**
 * Prints the airports which are articulation points on the graph and the amount of said articulation points.
 */
void FlightManager::articulationPoints() {
    list<int> articulationPoints = flights.articulationPoints();
    cout << "\nHere they are: \n";
    articulationPoints.sort();
    for (int i : articulationPoints) {
        cout << flights.getAirport(i) << "| " << airports.find(flights.getAirport(i))->getName() << '\n'; //flights.getAirport(i);
    }
    cout << "There are " << articulationPoints.size() << " articulation points.\n\n";
}

//------------------------Menus-------------------------------
void FlightManager::showAirportInfoMenu() {
    cout << "======================================\n";
    cout << "| Options :                          |\n";
    cout << "| 1- Number of flights               |\n";
    cout << "| 2- Number of airlines with         |\n";
    cout << "|    flights from this airport       |\n";
    cout << "| 3- Number of different destinies   |\n";
    cout << "| 4- Number of different destiny     |\n";
    cout << "|    countries                       |\n";
    cout << "| 5- Number of places reachable      |\n";
    cout << "| 6- Go back                         |\n";
    cout << "======================================\n";
    cout << "Pick an option:";
}

void FlightManager::askWhichPlaceMenu() {
    cout << "======================================\n";
    cout << "| Options :                          |\n";
    cout << "| 1- Number of airports reachable    |\n";
    cout << "| 2- Number of cities reachable      |\n";
    cout << "| 3- Number of countries reachable   |\n";
    cout << "| 4- Go back                         |\n";
    cout << "======================================\n";
    cout << "Pick an option:";
}

void FlightManager::showStartingPointMenu() {
    cout << "========================\n";
    cout << "| Starting Point :     |\n";
    cout << "| 1- Airport           |\n";
    cout << "| 2- City              |\n";
    cout << "| 3- Location          |\n";
    cout << "| 4- Go back           |\n";
    cout << "========================\n";
    cout << "Pick an option:";
}

void FlightManager::showDestinationMenu(){
    cout << "========================\n";
    cout << "| Destination :        |\n";
    cout << "| 1- Airport           |\n";
    cout << "| 2- City              |\n";
    cout << "| 3- Location          |\n";
    cout << "| 4- Go back           |\n";
    cout << "========================\n";
    cout << "Pick an option:";
}

void FlightManager::showAirlineMenu() {
    cout << "========================\n";
    cout << "| Airline :            |\n";
    cout << "| 1- Choose One        |\n";
    cout << "| 2- Ignore            |\n";
    cout << "| 3- Go back           |\n";
    cout << "========================\n";
    cout << "Pick an option:";
}

void FlightManager::askForOtherInfoMenu(){
    cout << "==========================================\n";
    cout << "| Other Info :                           |\n";
    cout << "| 1- Number of countries                 |\n";
    cout << "| 2- Number of cities                    |\n";
    cout << "| 3- Number of Flights                   |\n";
    cout << "| 4- Avg number of Airports in a Country |\n";
    cout << "| 5- Airport with the most flights       |\n";
    cout << "| 6- Airport with the most airlines      |\n";
    cout << "| 7- Diameter of the graph               |\n";
    cout << "| 8- Articulation Points                 |\n";
    cout << "| 9- Go back                             |\n";
    cout << "==========================================\n";
    cout << "Pick an option:";
}

Airline FlightManager::getAirline(const string& code) {
    for (Airline a : airlines){
        if (a.getCode() == code) return a;
    }
    return {""};
}

/**
 * Uses the lists provided to calculate the best trajectories and shows them on the standard output
 * @param s list of the starting airports
 * @param d list of the destination airports
 * @param a list of the airlines to be used in the trajectory
 * Note : if a is empty, all airlines will be considered when calculating trajectories
 */
void FlightManager::showBestTrajectories(const list<int>& s, const list<int>& d, const list<string>& a) {
    auto l = flights.calculateBestTrajectory(s, d, a);
    l.unique();
    cout << "There are " <<  l.size() << " minimal trajectories!\n";
    for (auto p : l){
        cout << "One possible path would be:\n";
        auto pointer = p.begin();
        auto nodeIterator = airports.find({*pointer});
        cout << nodeIterator->getName() << ',' << nodeIterator->getCity() << "\n";
        auto tempP = pointer;
        pointer++;
        while (pointer != p.end()){
            auto tempNode = airports.find({*tempP});
            nodeIterator = airports.find({*pointer});
            string airline_code = flights.getAirline(tempNode->getNode(),nodeIterator->getNode(),a);
            Airline airline1 = getAirline(airline_code);
            cout << "   |   " << airline1.getName() <<  "\n";
            cout << "   v   \n";
            cout << nodeIterator->getName() << ',' << nodeIterator->getCity() << "\n";
            pointer++;
            tempP++;
        }
        cout << "Do you want to see another trajectory?(y/n)";
        string answer;
        cin >> answer;
        if (cin.fail()){
            cin.clear();
            cin.ignore(256,'\n');
            answer = "y";
        }
        if (answer == "no" || answer == "No" || answer == "NO" || answer == "n" || answer == "N") break;
    }
}
