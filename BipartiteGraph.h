#ifndef VERTEX_COVER_BIPARTITEGRAPH_H
#define VERTEX_COVER_BIPARTITEGRAPH_H

#include <vector>

class BipartiteGraph {
    std::size_t n;
    std::vector<std::vector<int>> graph;

    bool findAugmentingPath(int, std::vector<int>&, std::vector<bool>&);

public:
    BipartiteGraph(std::size_t);

    void addEdge(int, int);
    std::vector<int> findMatching();
};


#endif //VERTEX_COVER_BIPARTITEGRAPH_H
