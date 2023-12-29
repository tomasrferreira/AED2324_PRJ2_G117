#include <unordered_set>
#include "graph.h"

// Constructor: nr nodes and direction (default: undirected)
Graph::Graph(int num, bool dir) : n(num), hasDir(dir), nodes(num+1) {
}

/**
 * Add edge from source to destination with a certain weight
 * @param src
 * @param dest
 * @param airline
 * @param weight
 */
void Graph::addEdge(int src, int dest, const string& airline , int weight) {
    if (src<1 || src>n || dest<1 || dest>n) return;
    nodes[src].adj.push_back({dest, weight,airline});
    if (!hasDir) nodes[dest].adj.push_back({src, weight,airline});
}


/**
 * Depth-First Search: example implementation
 * @param v
 */
void Graph::dfs(int v) {
    // show node order
    // cout << v << " ";
    nodes[v].visited = true;
    for (const auto& e : nodes[v].adj) {
        int w = e.dest;
        if (!nodes[w].visited)
            dfs(w);
    }
}

/**
 * Breadth-First Search: example implementation
 * @param v
 */
void Graph::bfs(int v) {
    for (int i=1; i<=n; i++) nodes[i].visited = false;
    queue<int> q; // queue of unvisited nodes
    q.push(v);
    nodes[v].visited = true;
    while (!q.empty()) { // while there are still unvisited nodes
        int u = q.front(); q.pop();
        // show node order
        //cout << u << " ";
        for (const auto& e : nodes[u].adj) {
            int w = e.dest;
            if (!nodes[w].visited) {
                q.push(w);
                nodes[w].visited = true;
            }
        }
    }
}

/**
 * Calculates the diameter of the graph.
 *
 * Complexity : O(|V|*(|V|+|E|)) where |V| and |E| is the cardinality of set of vertices and edges respectively.
 * @return diameter
 */
pair<pair<string ,string >, int> Graph::diameter() {
    int max_dist = 0;
    string startingAirport;
    string endingAirport;
    for(int a = 1; a <= n; a++) {
        for (int i = 1; i <= n; i++) {
            nodes[i].visited = false;
            nodes[i].distance = -1;
        }
        queue<int> q;
        q.push(a);
        nodes[a].visited = true;
        nodes[a].distance = 0;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (const auto& e: nodes[u].adj) {
                int w = e.dest;
                if (!nodes[w].visited) {
                    q.push(w);
                    nodes[w].visited = true;
                    nodes[w].distance = nodes[u].distance + 1;
                    if (nodes[w].distance > max_dist){
                        max_dist = nodes[w].distance;
                        startingAirport = nodes[a].airport;
                        endingAirport = nodes[w].airport;
                    }
                }
            }
        }
    }
    return make_pair(make_pair(startingAirport, endingAirport), max_dist);
}
/**
 * Changes the code of the airport of node v to airport
 *
 * Complexity : constant
 * @param v
 * @param airport
 */
void Graph::setAirport(int v, const string &airport) {
    nodes[v].airport = airport;
}

/**
 * Gives the flights that leave a certain airport
 * Complexity : O(n) being n the amount of flights that leave airport v
 * @param node of the airport
 * @return list of pair each containing the node of the destination airport and the code of the airline
 */
list<pair<int, string>> Graph::getFlights( int node){
    list<pair<int, string>> res ;
    for (const Edge& e : nodes[node].adj)
        res.emplace_back(e.dest,e.airline);
    return res;
}

/**
 * @param v
 * @return Code of the airport
 */
string Graph::getAirport(int v) {
    return nodes[v].airport;
}

/**
 * Calculates and returns the number of edges on the graph, that is the total number of flights.
 *
 * Complexity : O(|V|) where |V| is the cardinality of set of vertices.
 * @return number of flights
 */
int Graph::getNumFlightsTotal(){
    int count = 0;
    for(auto& x : nodes){
        count += (int)x.adj.size();
    }
    return count;
}

/**
 * Calculates the number of flights that go to and leave airport v.
 *
 * Complexity : O(|V| + |E|) where |V| and |E| is the cardinality of set of vertices and edges respectively.
 * @param v node of airport
 * @return number of flights on airport v
 */
int Graph::getNumFlightsAirport(int v){
    int count = 0;
    count += (int)nodes[v].adj.size();
    for(auto& x : nodes){
        for(auto& i : x.adj){
            if(i.dest == v) count++;
        }
    }
    return count;
}
/**
 * Calculates the number of different airlines which have flights going to or from airport v.
 *
 * Complexity : O(|V|+|E|) where |V| and |E| are the cardinality of set of vertices and edges respectively.
 * @param v node of airport
 * @return number of airlines
 */
int Graph::getNumAirlinesAirport(int v){
    unordered_set<string> airlines;
    for(auto& x : nodes){
        for(auto& i : x.adj){
            if(i.dest == v && airlines.find(i.airline) == airlines.end()) airlines.insert(i.airline);
        }
    }
    for(auto& x : nodes[v].adj){
        if(airlines.find(x.airline) == airlines.end()){
            airlines.insert(x.airline);
        }
    }
    return (int)airlines.size();
}

/**
 * Calculates the airports that you can go to from airport v with a maximum number of flights f.
 *
 * Complexity : O(|V| + |E|) where |V| and |E| is the cardinality of set of vertices and edges respectively.
 * @param v node of the starting airport
 * @param f maximum number of flights
 * @return list with the codes of the airports
 */
list<string> Graph::getAirportsReachable(int v, int f) {
    list<string> res;
    for (int i=1; i<=n; i++) nodes[i].visited = false;
    queue<int> q; // queue of unvisited nodes
    q.push(v);
    nodes[v].distance = 0;
    nodes[v].visited = true;
    while (!q.empty()) { // while there are still unvisited nodes
        int u = q.front(); q.pop();
        if (nodes[u].distance > f)
            break;
        res.push_back(nodes[u].airport);
        for (const auto& e : nodes[u].adj) {
            int w = e.dest;
            if (!nodes[w].visited) {
                nodes[w].distance = nodes[u].distance +1;
                q.push(w);
                nodes[w].visited = true;
            }
        }
    }
    res.pop_front();
    return res;
}

/**
 * Calculates best trajectories.
 *
 * Complexity : O(|V|*|E|) where |V| and |E| is the cardinality of set of vertices and edges respectively, because
 * in the worst case scenario we will traverse a node as many edges there are that go from other nodes to this one
 * @param startup starting airports
 * @param end destination airports
 * @param airlines airlines to be used, if empty all airlines used
 * @return trajectories
 */
list<list<string>> Graph::calculateBestTrajectory(const list<int>& startup, const list<int>& end, const list<string>& airlines) {
    for (int i=1; i<=n; i++) nodes[i].visited = false;

    list<list<string>> res;
    int max_distance = 0;

    queue<list<int>> q;
    for (int s : startup){
        list<int> initial_list;
        initial_list.push_back(s);
        q.push(initial_list);
        nodes[s].visited = true;
        nodes[s].distance = 0;
    }
    while (!q.empty()) {
        list<int> nodes_list = q.front();
        int u = nodes_list.back();
        if (max_distance != 0 && nodes[u].distance == max_distance)
            break;
        q.pop();
        for (const auto& e : nodes[u].adj) {
            int w = e.dest;
            if ((!nodes[w].visited || nodes[w].distance == nodes[u].distance +1 ) && (airlines.empty() || find(airlines.begin(),airlines.end(),e.airline)!=airlines.end())) {
                if ( max_distance == 0 && find(end.begin(),end.end(),w) != end.end()){
                    max_distance = nodes[u].distance +1;
                }
                list<int> new_list (nodes_list);
                new_list.push_back(w);
                q.push(new_list);
                nodes[w].distance = nodes[u].distance +1;
                nodes[w].visited = true;
            }
        }
    }
    if (max_distance == 0) return res;
    while (!q.empty()){
        list<int> nodes_list = q.front();
        q.pop();
        int u = nodes_list.back();
        if (find(end.begin(),end.end(),u) != end.end()){
            list<string> temp;
            for (int node : nodes_list){
                temp.push_back(nodes[node].airport);
            }
            res.push_back(temp);
        }
    }
    return res;
}

/**
 * Searches for the airline of a flight that is included in a given list
 * @param a Starting node
 * @param b Destination node
 * @param air Airline Codes
 * @return Airline code
 */
string Graph::getAirline(int a, int b, list<string> air) {
    for (const Edge& e : nodes[a].adj){
        if (e.dest != b) continue;
        if (air.empty() || find(air.begin(),air.end(),e.airline) != air.end()) return e.airline;
    }
    return {};
}

/**
 * Auxiliary function that helps to calculate the articulation points.
 *
 * Complexity : O(|V|+|E|) where |V| and |E| is the cardinality of set of vertices and edges respectively
 * @param v
 * @param order
 * @param l
 */
void Graph::dfs_articulation_points(int v, int &order, list<int> &l) {
    nodes[v].visited = true;
    nodes[v].num = nodes[v].low = order++;

    int children = 0;
    bool articulation = false;

    for (Edge e : nodes[v].adj) {
        int w = e.dest;
        if (!nodes[w].visited) {
            children++;
            dfs_articulation_points(w, order, l);
            nodes[v].low = min(nodes[v].low, nodes[w].low);
            if (nodes[w].low >= nodes[v].num)
                articulation = true;            // v is an articulation point
        }
        else
            nodes[v].low = min(nodes[v].low, nodes[w].num);
    }

    // if it is the root node (root.num = 1), it has to have more than 1 child or ...
    if ((nodes[v].num == 1 && children > 1) || (nodes[v].num > 1 && articulation))
        l.push_front(v);
}
/**
 * Function that calculates articulation points.
 *
 * Complexity : O(|V| + |E|) where |V| and |E| is the set of cardinality of vertices and edges respectively.
 * @return list of articulation points
 */
list<int> Graph::articulationPoints() {
    list<int> answer;
    for (Node& node : nodes) {
        node.visited = false;
        node.inStack = false;
        node.num = 0;
        node.low = 0;
    }

    int order = 1;
    for (int v = 1; v <= n; v++) {
        if (!nodes[v].visited) {
            dfs_articulation_points(v, order, answer);
        }
    }
    return answer;
}
