#include "BasicServiceMetrics.h"


BasicServiceMetrics::BasicServiceMetrics(Graph *railwayGraph): railwayGraph(railwayGraph) {}

int BasicServiceMetrics::findMaxFlow(Vertex *source, Vertex *target) {
    return railwayGraph->edmondsKarp(source,target);
}

std::list<std::pair<Vertex*, Vertex*>> BasicServiceMetrics::pairsMaxFlow() {
    std::list<std::pair<Vertex*, Vertex*>> pairs;
    int max = -1;
    int flow;
    auto vertexSet = railwayGraph->getVertexSet();

    for (int i = 0; i < vertexSet.size(); i++) {
        for (int j = i + 1; j < vertexSet.size(); j++) {
            flow = railwayGraph->edmondsKarp(vertexSet[i], vertexSet[j]);
            if (max < flow) {
                pairs.clear();
                max = flow;
                pairs.push_back(std::make_pair(vertexSet[i],vertexSet[j]));
            } else if (max == flow) {
                pairs.push_back(std::make_pair(vertexSet[i],vertexSet[j]));
            }
        }
    }

    return pairs;
}

std::list<std::list<std::string>> BasicServiceMetrics::topkMunicDist(int k) {
    std::map<std::string, int> distMap;
    std::map<std::string, int> muncMap;
    std::vector<std::pair<std::string, int>> topDist;
    std::vector<std::pair<std::string, int>> topMunc;
    std::list<std::string> topkDist;
    std::list<std::string> topkMunc;
    Vertex *v1, *v2;

    auto vertexSet = railwayGraph->getVertexSet();
    for (int i = 0; i < vertexSet.size(); i++) {
        for (int j = i + 1; j < vertexSet.size(); j++) {
            v1 = vertexSet[i];
            v2 = vertexSet[j];
            std::string dtc = v1->getDistrict();
            std::string mcp = v1->getMunicipality();

            if (dtc == v2->getDistrict() && !dtc.empty()) {
                distMap[dtc] += railwayGraph->edmondsKarp(v1, v2);
            }
            if (mcp == v2->getMunicipality() && !mcp.empty()) {
                muncMap[mcp] += railwayGraph->edmondsKarp(v1, v2);
            }
        }
    }

    for (auto &p : distMap) {
        topDist.push_back(p);
    }
    for (auto &p : muncMap) {
        topMunc.push_back(p);
    }

    sort(topDist.begin(), topDist.end(), [](const std::pair<std::string, int> &a, const std::pair<std::string, int> &b) {
        return a.second > b.second;
    });
    sort(topMunc.begin(), topMunc.end(), [](const std::pair<std::string, int> &a, const std::pair<std::string, int> &b) {
        return a.second > b.second;
    });

    for (int i = 0; i < k && i < topDist.size(); i++) {
        topkDist.push_back(topDist[i].first);
    }
    for (int i = 0; i < k && i < topMunc.size(); i++) {
        topkMunc.push_back(topMunc[i].first);
    }

    return {topkMunc, topkDist};
}

int BasicServiceMetrics::maxTrainArriving(Vertex *v) {
    railwayGraph->addVertex(0);
    Vertex *source = railwayGraph->findVertex(0);
    int max = 0;

    for (auto &vert : railwayGraph->getVertexSet()) {
        if (vert->getAdj().size() == 1 && vert->getId() != v->getId()) {
            source->addEdge(vert,INT_MAX);
        }
    }

    max = railwayGraph->edmondsKarp(source, v);

    railwayGraph->removeVertex(0);

    return max;
}

std::pair<int, int> BasicServiceMetrics::maxFlowMinCost(Vertex *source, Vertex *target) {
    return railwayGraph->edmondsDijkstra(source, target);
}

std::vector<std::string> BasicServiceMetrics::topkMostAffectStations(Vertex *v1, Vertex *v2, int k) {
    std::vector<std::pair<std::string, int>> stations;
    std::map<std::string, int> affectedStations;
    std::vector<std::string> mostAffected;

    for (auto v : railwayGraph->getVertexSet()) {
        affectedStations[v->getName()] = maxTrainArriving(v);
    }

    reduceAugmConectivity(v1, v2, true);

    for (auto v : railwayGraph->getVertexSet()) {
        affectedStations[v->getName()] = affectedStations[v->getName()] - maxTrainArriving(v);
    }

    reduceAugmConectivity(v1, v2, false);

    for (auto &s : affectedStations) {
        stations.push_back(s);
    }

    sort(stations.begin(), stations.end(), [](const std::pair<std::string, int> &a, const std::pair<std::string, int> &b) {
        return a.second > b.second;
    });

    for (int i = 0; i < k && i < stations.size(); i++) {
        if (stations[i].second > 0) {
            mostAffected.push_back(stations[i].first);
        }
    }

    return mostAffected;
}

bool BasicServiceMetrics::reduceAugmConectivity(Vertex *v1, Vertex *v2, bool reduce) {
    for (auto &e : v1->getAdj()) {
        if (e->getDest()->getName() == v2->getName()) {
            e->setDisconected(reduce);
            e->getReverse()->setDisconected(reduce);
            return true;
        }
    }

    return false;
}
