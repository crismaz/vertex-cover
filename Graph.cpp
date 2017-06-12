#include <iostream>
#include "BipartiteGraph.h"
#include "Graph.h"

Graph::Graph(std::size_t n): n(n), graph(n) {}

void Graph::addEdge(int u, int v) {
    graph[u].push_back(v);
    graph[v].push_back(u);
}

BipartiteGraph createDoubledGraph(std::vector<std::vector<int>>& graph, std::vector<bool>& removed) {
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

std::vector<int> lpSolutionFromCover(std::size_t n, const std::vector<bool>& inCover) {
    std::vector<int> lpSolution(n);
    for (int i = 0; i < n; i++) {
        lpSolution[i] = inCover[i] + inCover[i + n];
    }

    return lpSolution;
}

std::vector<int> solveHalfIntegralLinearProgramming(std::vector<std::vector<int>>& graph, std::vector<bool>& removed,
                                                    bool anySolution) {
    std::size_t n = graph.size();
    auto bipartiteGraph = createDoubledGraph(graph, removed);

    auto mate = bipartiteGraph.findMatching();
    auto lpSolution = lpSolutionFromCover(n, bipartiteGraph.findVertexCoverFromMatching(mate));

    int willBeRemoved = 0;
    for (int i = 0; i < n; i++) {
        willBeRemoved += !removed[i] && lpSolution[i] != 1;
    }

    if (!anySolution && willBeRemoved == 0) {
        for (int i = 0; i < n; i++) {
            if (mate[i] != -1 && mate[i + n] != -1) {
                auto mateCpy = mate;
                mateCpy[i] = mateCpy[i + n] = -1;

                std::replace(mateCpy.begin(), mateCpy.end(), i, -1);
                std::replace(mateCpy.begin(), mateCpy.end(), (int) (i + n), -1);

                removed[i] = true;
                auto bipartiteGraphCpy = createDoubledGraph(graph, removed);

                removed[i] = false;

                if (!bipartiteGraphCpy.findAugmentingPath(mateCpy)) {
                    lpSolution = lpSolutionFromCover(n, bipartiteGraphCpy.findVertexCoverFromMatching(mateCpy));
                    lpSolution[i] = 2;

                    break;
                }
            }
        }
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

void Graph::subgraphVertexCover(bool useLinearProgramming, bool anyLpSolution, std::vector<bool>& taken,
                                std::vector<bool>& removed, std::vector<int>& degree,
                                std::set<std::pair<int, int>>& vertices, std::vector<bool>& bestCover) {
    std::vector<int> verticesToRestore;

    auto removeVertex = [&](int v) {
        setRemoved(v, true, removed, degree, graph, vertices);
        verticesToRestore.push_back(v);
    };

    if (useLinearProgramming) {
        bool changed;

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

        taken[v] = true;
        subgraphVertexCover(useLinearProgramming, anyLpSolution, taken, removed, degree, vertices, bestCover);

        taken[v] = false;
        for (int u : graph[v]) {
            if (!removed[u]) {
                taken[u] = true;
                removeVertex(u);
            }
        }

        subgraphVertexCover(useLinearProgramming, anyLpSolution, taken, removed, degree, vertices, bestCover);
    }

    for (int i : verticesToRestore) {
        setRemoved(i, false, removed, degree, graph, vertices);
    }
}

std::vector<bool> Graph::findVertexCover(bool useLinearProgramming, bool anyLpSolution) {
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

bool Graph::isVertexCover(std::vector<bool>& cover) {
    for (int v = 0; v < n; v++) {
        for (int u : graph[v]) {
            if (!cover[v] && !cover[u]) {
                return false;
            }
        }
    }

    return true;
}
