cmake_minimum_required(VERSION 3.23)
project(Railway)

set(CMAKE_CXX_STANDARD 11)

add_executable(Railway main.cpp src/Menu.cpp src/Menu.h src/Data.cpp src/Data.h src/data_structures/Graph.cpp src/data_structures/Graph.h src/data_structures/VertexEdge.cpp src/data_structures/VertexEdge.h src/data_structures/MutablePriorityQueue.h src/BasicServiceMetrics.cpp src/BasicServiceMetrics.h)
