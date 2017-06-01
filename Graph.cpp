#include "BipartiteGraph.h"
#include "Graph.h"

Graph::Graph(std::size_t n): n(n), graph(n) {}

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

void setRemoved(int v, bool setTo, std::vector<bool>& removed, std::vector<int>& degree,
                std::vector<std::vector<int>>& graph, std::set<std::pair<int,int>>& vertices) {
    removed[v] = setTo;

    if (setTo) {
        vertices.erase({degree[v], v});
    } else {
        vertices.insert({degree[v], v});
    }

    for (int u : graph[v]) {
        int newDeg = degree[u] + (setTo ? -1 : 1);

        if (!removed[u]) {
            vertices.erase({degree[u], u});
            vertices.insert({newDeg, u});
        }

        degree[u] = newDeg;
    }
}

void updateCover(std::vector<bool>& currentCover, std::vector<bool>& bestCover) {
    auto countTrue = [](auto& vec) { return std::count(vec.begin(), vec.end(), true); };

    if (countTrue(currentCover) < countTrue(bestCover)) {
        bestCover = currentCover;
    }
}

void Graph::subgraphVertexCover(bool useLinearProgramming, std::vector<bool>& taken, std::vector<bool>& removed,
                                std::vector<int>& degree, std::set<std::pair<int, int>>& vertices,
                                std::vector<bool>& bestCover) {
    std::vector<int> verticesToRestore;

    auto removeVertex = [&](int v) {
        setRemoved(v, true, removed, degree, graph, vertices);
        verticesToRestore.push_back(v);
    };

    if (useLinearProgramming) {
        auto lpSolution = solveHalfIntegralLinearProgramming(graph, removed);

        for (int v = 0; v < n; v++) {
            if (!removed[v] && lpSolution[v] != 1) {
                bool take = lpSolution[v] == 2;

                taken[v] = take;
                removeVertex(v);
            }
        }
    }

    if (vertices.empty()) {
        updateCover(taken, bestCover);
    } else if (vertices.begin()->first <= 1) {
        int v = vertices.begin()->second;

        taken[v] = false;
        removeVertex(v);

        if (degree[v] == 1) {
            int u = *std::find_if(graph[v].begin(), graph[v].end(), [&](int i) -> bool { return !removed[i]; });

            taken[u] = true;
            removeVertex(u);
        }

        subgraphVertexCover(useLinearProgramming, taken, removed, degree, vertices, bestCover);
    } else {
        int v = vertices.rbegin()->second;
        removeVertex(v);

        taken[v] = true;
        subgraphVertexCover(useLinearProgramming, taken, removed, degree, vertices, bestCover);

        taken[v] = false;
        for (int u : graph[v]) {
            if (!removed[u]) {
                taken[u] = true;
                removeVertex(u);
            }
        }

        subgraphVertexCover(useLinearProgramming, taken, removed, degree, vertices, bestCover);
    }

    for (int i : verticesToRestore) {
        setRemoved(i, false, removed, degree, graph, vertices);
    }
}

std::vector<bool> Graph::findVertexCover(bool useLinearProgramming) {
    std::vector<bool> taken(n), removed(n, false), bestCover(n, true);
    std::vector<int> degree(n, 0);
    std::set<std::pair<int,int>> vertices;

    for (int i = 0; i < n; i++) {
        degree[i] = (int) graph[i].size();
        vertices.insert({degree[i], i});
    }

    subgraphVertexCover(useLinearProgramming, taken, removed, degree, vertices, bestCover);
    return bestCover;
}
