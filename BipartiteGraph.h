#ifndef VERTEX_COVER_BIPARTITEGRAPH_H
#define VERTEX_COVER_BIPARTITEGRAPH_H

#include <cassert>
#include <queue>
#include <vector>

class BipartiteGraph {
    /// Number of vertices
    std::size_t n;

    /// Adjacency list for each vertex
    std::vector<std::vector<int>> graph;

    /// Vector of size n, specifying the color (false/true) for each vertex
    /// there can be no edges between vertices with the same color
    std::vector<bool> side;

    bool findAugmentingPathFrom(int, std::vector<int>&, std::vector<bool>&);
    bool findAugmentingPaths(std::vector<int>& mate);

public:
    /// Creates an empty bipartite graph with n vertices
    BipartiteGraph(std::size_t n, const std::vector<bool>& side);

    /// Adds an undirected edge between vertices u and v
    void addEdge(int u, int v);

    /// Checks if the given matching is optimal
    ///
    /// \param mate see BipartiteGraph::findMatching
    bool isMatchingOptimal(std::vector<int> mate);

    /// Finds a maximum matching
    ///
    /// \return mate - a vector of size n, for each vertex i, mate[i] contains the id of the vertex matched to it,
    ///         or -1 if it is unmatched
    std::vector<int> findMatching();

    /// Finds a vertex cover given a maximum matching
    ///
    /// \param mate see BipartiteGraph::findMatching
    /// \return a vector of size n, containing true for vertices belonging to the vertex cover
    std::vector<bool> findVertexCoverFromMatching(const std::vector<int>& mate);
};


#endif //VERTEX_COVER_BIPARTITEGRAPH_H
