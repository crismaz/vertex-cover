#ifndef VERTEX_COVER_GRAPH_H
#define VERTEX_COVER_GRAPH_H

#include <algorithm>
#include <vector>
#include <set>

class Graph {
    /// Number of vertices
    std::size_t n;

    /// Adjacency list for each vertex
    std::vector<std::vector<int>> graph;

    void subgraphVertexCover(bool, bool, std::vector<bool>&, std::vector<bool>&, std::vector<int>&,
                             std::set<std::pair<int,int>>&, std::vector<bool>&) const;

public:
    /// Creates an empty graph with n vertices numbered from 0 to n-1
    Graph(std::size_t n);

    /// Adds an undirected edge between vertices u and v
    void addEdge(int u, int v);

    /// Returns the minimum vertex cover
    ///
    /// \param useLinearProgramming if true, use linear programming to reduce the graph
    ///                             (the vertex cover above LP algorithm)
    /// \param anyLpSolution        if the linear programming algorithm finds a solution with V_0 and V_1 empty,
    ///                             then if anyLpSolution is false, the algorithm checks if the solution that was found
    ///                             is the only optimal one
    /// \return vector of size n, element i set to true if vertex i belongs to the vertex cover
    std::vector<bool> findVertexCover(bool useLinearProgramming, bool anyLpSolution) const;

    /// Returns true if the given vertex cover is valid
    ///
    /// \param cover a vector of size n, element i set to true if vertex i belongs to the vertex cover
    /// \return true if every edge has at least one of it's endpoints chosen
    bool isVertexCover(const std::vector<bool>& cover) const;
};


#endif //VERTEX_COVER_GRAPH_H
