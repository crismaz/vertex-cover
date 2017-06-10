#ifndef VERTEX_COVER_BIPARTITEGRAPH_H
#define VERTEX_COVER_BIPARTITEGRAPH_H

#include <cassert>
#include <queue>
#include <vector>

class BipartiteGraph {
    std::size_t n;
    std::vector<std::vector<int>> graph;
    std::vector<bool> side;

    bool findAugmentingPathFrom(int, std::vector<int>&, std::vector<bool>&);

public:
    BipartiteGraph(std::size_t, std::vector<bool>&);

    void addEdge(int, int);

    bool findAugmentingPath(std::vector<int>&);

    std::vector<int> findMatching();
    std::vector<bool> findVertexCoverFromMatching(std::vector<int>&);
};


#endif //VERTEX_COVER_BIPARTITEGRAPH_H
