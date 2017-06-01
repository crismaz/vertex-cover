#include "BipartiteGraph.h"
#include "Graph.h"

Graph::Graph(std::size_t n): n(n) {}

void Graph::addEdge(int u, int v) {
    graph[u].push_back(v);
    graph[v].push_back(u);
}

std::vector<int> solveHalfIntegralLinearProgramming(std::vector<std::vector<int>>& graph, std::vector<bool>& removed) {
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

    auto inCover = bipartiteGraph.findVertexCover();

    std::vector<int> lpSolution(n);
    for (int i = 0; i < n; i++) {
        lpSolution[i] = inCover[i] + inCover[i + n];
    }

    return lpSolution;
}

std::vector<bool> Graph::findVertexCover(bool useLinearProgramming) {
    return std::vector<bool>(); // TODO
}
