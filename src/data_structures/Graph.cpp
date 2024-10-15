#include "Graph.h"

int Graph::getNumVertex() const {
    return vertexSet.size();
}

std::vector<Vertex *> Graph::getVertexSet() const {
    return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
Vertex * Graph::findVertex(const int &id) const {
    for (auto v : vertexSet)
        if (v->getId() == id)
            return v;
    return nullptr;
}

Vertex * Graph::findVertex(std::string name) const {
    for (auto v : vertexSet)
        if (v->getName() == name)
            return v;
    return nullptr;
}

/*
 * Finds the index of the vertex with a given content.
 */
int Graph::findVertexIdx(const int &id) const {
    for (unsigned i = 0; i < vertexSet.size(); i++)
        if (vertexSet[i]->getId() == id)
            return i;
    return -1;
}
/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
bool Graph::addVertex(const int &id) {
    if (findVertex(id) != nullptr)
        return false;
    vertexSet.push_back(new Vertex(id));
    return true;
}

bool Graph::addVertex(const int &id, std::string name, std::string district, std::string municipality, std::string township, std::string railLine) {
    if (findVertex(name) != nullptr)
        return false;
    vertexSet.push_back(new Vertex(id, name, district, municipality, township, railLine));
    return true;
}

bool Graph::removeVertex(const int &id) {
    for (auto it = vertexSet.begin(); it != vertexSet.end(); it++) {
        if ((*it)->getId() == id) {
            auto v = *it;
            for (auto u : vertexSet) {
                u->removeEdge(v->getId());
                v->removeEdge(u->getId());
            }
            vertexSet.erase(it);
            delete v;
            return true;
        }
    }
    return false;
}

/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
bool Graph::addEdge(const int &sourc, const int &dest, double w) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    v1->addEdge(v2, w);
    return true;
}

bool Graph::addBidirectionalEdge(Vertex *v1, Vertex *v2, std::string service, double w) {
    if (v1 == nullptr || v2 == nullptr)
        return false;
    auto e1 = v1->addEdge(v2, service, w);
    auto e2 = v2->addEdge(v1, service, w);
    if (e1 == nullptr || e2 == nullptr) {
        return false;
    }
    e1->setReverse(e2);
    e2->setReverse(e1);
    return true;
}

void Graph::testAndVisit(std::queue< Vertex*> &q, Edge *e, Vertex *w, double residual) {
    if (! w->isVisited() && residual > 0) {
        w->setVisited(true);
        w->setPath(e);
        q.push(w);
    }
}

bool Graph::findAugmentingPath(Vertex *s, Vertex *t) {
    for(auto v : vertexSet) {
        v->setVisited(false);
    }
    s->setVisited(true);
    std::queue<Vertex *> q;
    q.push(s);
    while( ! q.empty() && ! t->isVisited()) {
        auto v = q.front();
        q.pop();
        for(auto e: v->getAdj()) {
            testAndVisit(q, e, e->getDest(), e->getWeight() - e->getFlow());
        }
        for(auto e: v->getIncoming()) {
            testAndVisit(q, e, e->getOrig(), e->getFlow());
        }
    }
    return t->isVisited();
}

double Graph::findMinResidualAlongPath(Vertex *s, Vertex *t) {
    double f = INF;
    for (auto v = t; v != s; ) {
        auto e = v->getPath();
        if (e->getDest() == v) {
            f = std::min(f, e->getWeight() - e->getFlow());
            v = e->getOrig();
        }
        else {
            f = std::min(f, e->getFlow());
            v = e->getDest();
        }
    }
    return f;
}

void Graph::augmentFlowAlongPath(Vertex *s, Vertex *t, double f) {
    for (auto v = t; v != s; ) {
        auto e = v->getPath();
        double flow = e->getFlow();
        if (e->getDest() == v) {
            e->setFlow(flow + f);
            v = e->getOrig();
        }
        else {
            e->setFlow(flow - f);
            v = e->getDest();
        }
    }
}

int Graph::edmondsKarp(Vertex* s, Vertex* t) {
    int maxFlow = 0;

    if (s == nullptr || t == nullptr || s == t) {
        return -1;
    }

    // Reset the flows
    for (auto v : vertexSet) {
        for (auto e: v->getAdj()) {
            e->setFlow(0);
        }
    }
    // Loop to find augmentation paths
    while( findAugmentingPath(s, t) ) {
        double f = findMinResidualAlongPath(s, t);
        maxFlow += f;
        augmentFlowAlongPath(s, t, f);
    }

    return maxFlow;
}

void Graph::relax(Edge* e) {
    int cost;
    if (e->getService().compare(0, 8,"STANDARD") == 0) {
        cost = e->getOrig()->getDist() + 2;
        if (e->getDest()->getDist() > cost) {
            e->getDest()->setDist(cost);
            e->getDest()->setPath(e);
        }
    } else {
        cost = e->getOrig()->getDist() + 4;
        if (e->getDest()->getDist() > cost) {
            e->getDest()->setDist(cost);
            e->getDest()->setPath(e);
        }
    }
}

std::pair<int, int> Graph::edmondsDijkstra(Vertex* s, Vertex* t) {
    int maxFlow = 0;
    int cost = 0;

    if (s == nullptr || t == nullptr || s == t) {
        return std::make_pair(-1, -1);
    }

    // Reset the flows
    for (auto v : vertexSet) {
        for (auto e: v->getAdj()) {
            e->setFlow(0);
        }
    }

    // Loop to find augmentation paths
    while( findAugmentingPath(s, t) ) {
        dijkstra(s);

        double f = findMinResidualAlongPath(s, t);
        maxFlow += f;

        Edge *e = t->getPath();
        while (e != nullptr) {
            if (e->getService().compare(0, 8,"STANDARD") == 0) {
                cost += f * 2;
                /*
                std::cout << f << ' ' << e->getService() << '\n';
                std::cout << e->getOrig()->getName() << ' ' << e->getDest()->getName() << '\n';
                */
            } else {
                cost += f * 4;
                /*
                std::cout << f << ' ' << e->getService() << '\n';
                std::cout << e->getOrig()->getName() << ' ' << e->getDest()->getName() << '\n';
                */
            }

            e = e->getOrig()->getPath();
        }

        augmentFlowAlongPath(s, t, f);
    }

    return std::make_pair(maxFlow, cost);
}

void Graph::dijkstra(Vertex *source) {
    MutablePriorityQueue<Vertex> q;

    for (auto v : vertexSet) {
        v->setDist(INT_MAX);
        v->setPath(nullptr);
        v->setVisited(false);
        q.insert(v);
    }

    source->setDist(0);
    q.decreaseKey(source);

    while (!q.empty()) {
        Vertex *v = q.extractMin();
        v->setVisited(true);

        for (auto e : v->getAdj()) {
            if (!e->getDest()->isVisited() && e->getWeight() > e->getFlow()) {
                relax(e);
                q.decreaseKey(e->getDest());
            }
        }
    }
}



void deleteMatrix(int **m, int n) {
    if (m != nullptr) {
        for (int i = 0; i < n; i++)
            if (m[i] != nullptr)
                delete [] m[i];
        delete [] m;
    }
}

void deleteMatrix(double **m, int n) {
    if (m != nullptr) {
        for (int i = 0; i < n; i++)
            if (m[i] != nullptr)
                delete [] m[i];
        delete [] m;
    }
}

Graph::~Graph() {
    deleteMatrix(distMatrix, vertexSet.size());
    deleteMatrix(pathMatrix, vertexSet.size());
}