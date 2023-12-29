#ifndef AED2324_PRJ2_G117_GRAPH_H
#define AED2324_PRJ2_G117_GRAPH_H

#include <list>
#include <vector>
#include <iostream>
#include <queue>
#include <algorithm>
#include <stack>

using namespace std;

typedef stack<int> si;
typedef list<int> li;
typedef list<li> lli;

class Graph {
    struct Edge {
        int dest;   // Destination node
        int weight; // An integer weight
        string airline;
    };

    struct Node {
        list<Edge> adj; // The list of outgoing edges (to adjacent nodes)
        bool visited;   // As the node been visited on a search?
        string airport;
        int distance;
        int low;
        int num;
        bool inStack;
    };

    int n;              // Graph size (vertices are numbered from 1 to n)
    bool hasDir;        // false: undirected; true: directed
    vector<Node> nodes; // The list of nodes being represented

public:
    /**
     * Constructor : nr nodes and direction (Default : undirected)
     * @param nodes
     * @param dir
     */
    Graph(int nodes, bool dir = false);

    /**
     * Add edge from source to destination with a certain weight
     * @param src
     * @param dest
     * @param airline
     * @param weight
     */
    void addEdge(int src, int dest,const string& airline, int weight = 1);
    void dfs(int v);
    void bfs(int v);
    void setAirport(int v , const string& airport);
    list<pair<int , string>> getFlights(int node);
    string getAirport(int v);
    list<string> getAirportsReachable(int v, int f);
    list<list<string>> calculateBestTrajectory(const list<int>& startup,const list <int>& end,const list<string>& airlines);
    string getAirline(int a , int b , list<string> air);
    int getNumFlightsTotal();
    int getNumFlightsAirport(int v);
    pair<pair<std::string, std::string>, int> diameter();
    list<int> articulationPoints();
    void dfs_articulation_points(int v, int &order, li &l);
    int getNumAirlinesAirport(int v);
};

#endif //AED2324_PRJ2_G117_GRAPH_H
