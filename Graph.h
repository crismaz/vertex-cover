#ifndef VERTEX_COVER_GRAPH_H
#define VERTEX_COVER_GRAPH_H

#include <algorithm>
#include <vector>
#include <set>

class Graph {
    std::size_t n;
    std::vector<std::vector<int>> graph;

    void subgraphVertexCover(bool, std::vector<bool>&, std::vector<bool>&, std::vector<int>&,
                             std::set<std::pair<int,int>>&, std::vector<bool>&);

public:
    Graph(std::size_t);

    void addEdge(int, int);
    std::vector<bool> findVertexCover(bool);

    bool isVertexCover(std::vector<bool>&);
};


#endif //VERTEX_COVER_GRAPH_H
