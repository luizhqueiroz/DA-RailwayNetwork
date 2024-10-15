#include "Data.h"


Data::Data(std::string stations, std::string network) {
    std::ifstream stationsFile(stations, std::ifstream::in);
    std::ifstream networkFile(network, std::ifstream::in);
    std::string line;
    int id = 1;

    if (!stationsFile.is_open() || !networkFile.is_open()) {
        std::cout << "Arquivo não pode ser aberto" << std::endl;
        exit(1);
    }

    getline(stationsFile, line);
    getline(networkFile, line);

    while (!stationsFile.eof()) {
        std::string name;
        std::string district;
        std::string municipality;
        std::string township;
        std::string railLine;

        getline(stationsFile, line);

        if (line == "") {
            continue;
        }

        std::stringstream sstream(line);

        getline(sstream, name, ',');
        getline(sstream, district, ',');
        getline(sstream, municipality, ',');
        getline(sstream, township, ',');
        getline(sstream, railLine);

        if (railwayGraph.addVertex(id, name, district, municipality, township, railLine)) {
            id++;
        }
    }

    while (!networkFile.eof()) {
        std::string nameA;
        std::string nameB;
        std::string tmpCapacity;
        std::string service;
        int capacity;

        getline(networkFile, line);

        if (line == "") {
            continue;
        }

        std::stringstream sstream(line);

        getline(sstream, nameA, ',');
        getline(sstream, nameB, ',');

        getline(sstream, tmpCapacity, ',');
        capacity = std::stoi(tmpCapacity);

        getline(sstream, service);

        Vertex *vSource = railwayGraph.findVertex(nameA);
        Vertex *vDest = railwayGraph.findVertex(nameB);

        if (railwayGraph.addBidirectionalEdge(vSource, vDest, service, capacity)) {
        }
    }
}

Graph *Data::getRailwayGraph() {
    return &railwayGraph;
}
