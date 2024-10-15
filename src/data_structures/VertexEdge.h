#ifndef RAILWAY_VERTEX_EDGE_H
#define RAILWAY_VERTEX_EDGE_H

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <limits>
#include <algorithm>
#include "MutablePriorityQueue.h"

class Edge;

#define INF std::numeric_limits<double>::max()

/************************* Vertex  **************************/

class Vertex {
public:
    Vertex(int id);
    Vertex(int id, std::string name, std::string district, std::string municipality, std::string township, std::string line);
    bool operator<(Vertex & vertex) const; // // required by MutablePriorityQueue

    int getId() const;
    std::string getName() const;
    std::string getDistrict() const;
    std::string getMunicipality() const;
    std::vector<Edge *> getAdj() const;
    bool isVisited() const;
    bool isProcessing() const;
    unsigned int getIndegree() const;
    double getDist() const;
    Edge * getPath() const;
    std::vector<Edge *> getIncoming() const;

    void setId(int info);
    void setVisited(bool visited);
    void setProcesssing(bool processing);
    void setIndegree(unsigned int indegree);
    void setDist(double dist);
    void setPath(Edge *path);
    Edge * addEdge(Vertex *dest, double w);
    Edge * addEdge(Vertex *d, std::string s, double w);
    bool removeEdge(int destID);
    void removeOutgoingEdges();

    friend class MutablePriorityQueue<Vertex>;
protected:
    int id;                // identifier
    std::string name;       // station name
    std::string district;  // station distric
    std::string municipality; // station municipality
    std::string township;    // station township
    std::string line;         // station line
    std::vector<Edge *> adj;  // outgoing edges

    // auxiliary fields
    bool visited = false; // used by DFS, BFS, Prim ...
    bool processing = false; // used by isDAG (in addition to the visited attribute)
    unsigned int indegree; // used by topsort
    double dist = 0;
    Edge *path = nullptr;

    std::vector<Edge *> incoming; // incoming edges

    int queueIndex = 0; 		// required by MutablePriorityQueue and UFDS

    void deleteEdge(Edge *edge);
};

/********************** Edge  ****************************/

class Edge {
public:
    Edge(Vertex *orig, Vertex *dest, double w);
    Edge(Vertex *orig, Vertex *dest, std::string service, double w);

    Vertex * getDest() const;
    double getWeight() const;
    std::string getService() const;
    bool isDisconected() const;
    Vertex * getOrig() const;
    Edge * getReverse() const;
    double getFlow() const;

    void setDisconected(bool selected);
    void setReverse(Edge *reverse);
    void setFlow(double flow);
protected:
    Vertex * dest; // destination vertex
    std::string service;
    double weight; // edge weight, can also be used for capacity
    int disconectedWeight = 0;

    // auxiliary fields
    bool disconected = false;

    // used for bidirectional edges
    Vertex *orig;
    Edge *reverse = nullptr;

    double flow; // for flow-related problems
};

#endif // RAILWAY_VERTEX_EDGE_H