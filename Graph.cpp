#include <iostream>
#include "BipartiteGraph.h"
#include "Graph.h"

Graph::Graph(std::size_t n): n(n), graph(n) {}

void Graph::addEdge(int u, int v) {
    graph[u].push_back(v);
    graph[v].push_back(u);
}

/// Double the vertex set of the given graph to construct a bipartite graph
///
/// \param graph   adjacency list for each vertex v
/// \param removed vector with n elements, if element i is set to true vertex i will be ignored
/// \return graph with 2n vertices constructed as follows: for each edge (u,v) in the graph with unremoved endpoints,
///         the resulting graph contains edges (u, v+n) and (v, u+n)
BipartiteGraph createDoubledGraph(const std::vector<std::vector<int>>& graph, const std::vector<bool>& removed) {
    std::size_t n = graph.size();

    std::vector<bool> side(2 * n);
    for (int i = 0; i < 2 * n; i++) {
        side[i] = i < n;
    }

    BipartiteGraph bipartiteGraph(2 * n, side);

    for (int i = 0; i < n; i++) {
        if (!removed[i]) {
            for (int j : graph[i]) {
                if (!removed[j]) {
                    bipartiteGraph.addEdge(i, j + n);
                }
            }
        }
    }

    return bipartiteGraph;
}

/// Construct the solution to the LP relaxation of the vertex cover problem in graph G,
/// given the vertex cover in the bipartite graph G' corresponding to G
std::vector<int> lpSolutionFromCover(std::size_t n, const std::vector<bool>& inCover) {
    std::vector<int> lpSolution(n);
    for (int i = 0; i < n; i++) {
        lpSolution[i] = inCover[i] + inCover[i + n];
    }

    return lpSolution;
}

/// Solves the linear programming relaxation of the vertex cover problem
///
/// \param graph       adjacency list for each vertex v
/// \param removed     see Graph::subgraphVertexCover
/// \param anySolution see Graph::subgraphVertexCover
/// \return vector with n elements, element i is equal to 2x(i) where x is the LP solution
std::vector<int> solveHalfIntegralLinearProgramming(const std::vector<std::vector<int>>& graph,
                                                    std::vector<bool>& removed, bool anySolution) {
    std::size_t n = graph.size();
    auto bipartiteGraph = createDoubledGraph(graph, removed);

    auto mate = bipartiteGraph.findMatching();
    auto lpSolution = lpSolutionFromCover(n, bipartiteGraph.findVertexCoverFromMatching(mate));

    int willBeRemoved = 0;
    for (int i = 0; i < n; i++) {
        willBeRemoved += !removed[i] && lpSolution[i] != 1;
    }

    if (!anySolution && willBeRemoved == 0) { // the obtained solution assigns x(v) = 1/2 for all unremoved vertices v
        for (int i = 0; i < n; i++) {
            // check if there exists an optimal LP solution that assigns x(i) = 1
            // which is equivalent to checking if after removing i, the optimal LP cost drops by 1
            if (mate[i] != -1 && mate[i + n] != -1) {
                // copy the maximum matching to use as a starting point
                // when finding maximum matching with vertex i removed
                auto mateCpy = mate;
                mateCpy[i] = mateCpy[i + n] = -1;

                std::replace(mateCpy.begin(), mateCpy.end(), i, -1);
                std::replace(mateCpy.begin(), mateCpy.end(), (int) (i + n), -1);

                removed[i] = true; // temporarily mark i as removed
                auto bipartiteGraphCpy = createDoubledGraph(graph, removed);

                removed[i] = false;

                if (bipartiteGraphCpy.isMatchingOptimal(mateCpy)) {
                    lpSolution = lpSolutionFromCover(n, bipartiteGraphCpy.findVertexCoverFromMatching(mateCpy));
                    lpSolution[i] = 2;

                    break;
                }
            }
        }
    }

    return lpSolution;
}

/// Helper to update removed[v] along with auxiliary data structures - also see Graph::subgraphVertexCover
void setRemoved(int v, bool setTo, std::vector<bool>& removed, std::vector<int>& degree,
                const std::vector<std::vector<int>>& graph, std::set<std::pair<int,int>>& vertices) {
    removed[v] = setTo;

    if (setTo) {
        vertices.erase({degree[v], v});
    } else {
        vertices.insert({degree[v], v});
    }

    for (int u : graph[v]) { // update degrees for all neighbours of v
        int newDeg = degree[u] + (setTo ? -1 : 1);

        if (!removed[u]) {
            vertices.erase({degree[u], u});
            vertices.insert({newDeg, u});
        }

        degree[u] = newDeg;
    }
}

/// Helper to update the best cover seen so far
void updateCover(const std::vector<bool>& currentCover, std::vector<bool>& bestCover) {
    auto countTrue = [](auto& vec) { return std::count(vec.begin(), vec.end(), true); };

    if (countTrue(currentCover) < countTrue(bestCover)) {
        bestCover = currentCover;
    }
}

/// A subroutine to find the vertex cover, given that some vertices are temporarily removed
///
/// \param useLinearProgramming see Graph::findVertexCover
/// \param anyLpSolution        see Graph::findVertexCover
/// \param taken                vector with n elements, element i set to true if vertex i belongs to
///                             the current vertex cover
/// \param removed              vector with n elements, element i set to true if vertex i is temporarily removed
/// \param degree               vector with n elements, element i equal to the current degree of vertex i in the graph
/// \param vertices             set containing pairs (degree[v], v) for each unremoved vertex v
/// \param bestCover            smallest vertex cover found so far
void Graph::subgraphVertexCover(bool useLinearProgramming, bool anyLpSolution, std::vector<bool>& taken,
                                std::vector<bool>& removed, std::vector<int>& degree,
                                std::set<std::pair<int, int>>& vertices, std::vector<bool>& bestCover) const {
    std::vector<int> verticesToRestore; // list of vertices removed during the current call

    auto removeVertex = [&](int v) {
        setRemoved(v, true, removed, degree, graph, vertices);
        verticesToRestore.push_back(v);
    };

    if (useLinearProgramming) {
        bool changed;

        // reduce the graph using linear programming as long as it's possible
        do {
            changed = false;
            auto lpSolution = solveHalfIntegralLinearProgramming(graph, removed, anyLpSolution);

            for (int v = 0; v < n; v++) {
                if (!removed[v] && lpSolution[v] != 1) {
                    bool take = lpSolution[v] == 2;

                    taken[v] = take;
                    removeVertex(v);

                    changed = true;
                }
            }
        } while (changed);
    }

    while (!vertices.empty() && vertices.begin()->first <= 1) {
        int v = vertices.begin()->second;

        taken[v] = false;
        removeVertex(v);

        if (degree[v] == 1) {
            // find the single unremoved neighbour of v
            int u = *std::find_if(graph[v].begin(), graph[v].end(), [&](int i) -> bool { return !removed[i]; });

            taken[u] = true;
            removeVertex(u);
        }
    }

    if (vertices.empty()) {
        updateCover(taken, bestCover);
    } else {
        int v = vertices.rbegin()->second;
        removeVertex(v);

        // first branch - include v in the vertex cover
        taken[v] = true;
        subgraphVertexCover(useLinearProgramming, anyLpSolution, taken, removed, degree, vertices, bestCover);

        // second branch - don't include v in the vertex cover
        taken[v] = false;
        for (int u : graph[v]) {
            if (!removed[u]) {
                taken[u] = true;
                removeVertex(u);
            }
        }

        subgraphVertexCover(useLinearProgramming, anyLpSolution, taken, removed, degree, vertices, bestCover);
    }

    // restore vertices removed in the current call
    for (int i : verticesToRestore) {
        setRemoved(i, false, removed, degree, graph, vertices);
    }
}

std::vector<bool> Graph::findVertexCover(bool useLinearProgramming, bool anyLpSolution) const {
    std::vector<bool> taken(n), removed(n, false), bestCover(n, true);
    std::vector<int> degree(n, 0);
    std::set<std::pair<int,int>> vertices;

    for (int i = 0; i < n; i++) {
        degree[i] = (int) graph[i].size();
        vertices.insert({degree[i], i});
    }

    subgraphVertexCover(useLinearProgramming, anyLpSolution, taken, removed, degree, vertices, bestCover);
    return bestCover;
}

bool Graph::isVertexCover(const std::vector<bool>& cover) const {
    for (int v = 0; v < n; v++) {
        for (int u : graph[v]) {
            if (!cover[v] && !cover[u]) {
                return false;
            }
        }
    }

    return true;
}
