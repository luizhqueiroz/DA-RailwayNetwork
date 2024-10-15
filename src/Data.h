#ifndef RAILWAY_DATA_H
#define RAILWAY_DATA_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "data_structures/Graph.h"


class Data {
public:
    Data(std::string stations, std::string network);
    Graph *getRailwayGraph();

private:
    Graph railwayGraph;

};


#endif //RAILWAY_DATA_H
