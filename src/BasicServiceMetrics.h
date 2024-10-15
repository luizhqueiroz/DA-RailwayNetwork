#ifndef RAILWAY_BASIC_SERVICE_METRICS_H
#define RAILWAY_BASIC_SERVICE_METRICS_H

#include <list>
#include <vector>
#include <map>

#include "data_structures/Graph.h"

class BasicServiceMetrics {
public:
    BasicServiceMetrics(Graph *railwayGraph);
    int findMaxFlow(Vertex *source, Vertex *target);
    std::list<std::pair<Vertex*, Vertex*>> pairsMaxFlow();
    std::list<std::list<std::string>> topkMunicDist(int k);
    int maxTrainArriving(Vertex *v);
    std::pair<int, int> maxFlowMinCost(Vertex *source, Vertex *target);
    std::vector<std::string> topkMostAffectStations(Vertex *v1, Vertex *v2, int k);
    bool reduceAugmConectivity(Vertex *source, Vertex *target, bool reduce);

private:
    Graph *railwayGraph;
};

#endif //RAILWAY_BASIC_SERVICE_METRICS_H
