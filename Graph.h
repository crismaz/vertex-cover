#ifndef VERTEX_COVER_GRAPH_H
#define VERTEX_COVER_GRAPH_H

#include <vector>

class Graph {
    std::size_t n;
    std::vector<std::vector<int>> graph;

public:
    Graph(std::size_t);

    void addEdge(int, int);
    std::vector<bool> findVertexCover(bool);
};


#endif //VERTEX_COVER_GRAPH_H
