#include "BipartiteGraph.h"

BipartiteGraph::BipartiteGraph(std::size_t n): n(n), graph(n) {}

void BipartiteGraph::addEdge(int u, int v) {
    graph[u].push_back(v);
    graph[v].push_back(u);
}

bool BipartiteGraph::findAugmentingPath(int v, std::vector<int>& mate, std::vector<bool>& visited) {
    if (visited[v]) {
        return false;
    }

    visited[v] = true;

    for (int u : graph[v]) if (mate[u] == -1 || findAugmentingPath(mate[u], mate, visited)) {
            mate[v] = u;
            mate[u] = v;
            return true;
        }

    return false;
}

std::vector<int> BipartiteGraph::findMatching() {
    std::vector<int> mate(n, -1);
    std::vector<bool> visited(n);

    while (true) {
        bool found = false;
        std::fill(visited.begin(), visited.end(), false);

        for (int v = 0; v < n; v++) {
            if (mate[v] == -1 && findAugmentingPath(v, mate, visited)) {
                found = true;
            }
        }

        if (!found) {
            break;
        }
    }

    return mate;
}
