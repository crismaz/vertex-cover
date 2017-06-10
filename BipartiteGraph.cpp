#include "BipartiteGraph.h"

BipartiteGraph::BipartiteGraph(std::size_t n, std::vector<bool>& side): n(n), graph(n), side(side) {
    assert(side.size() == n);
}

void BipartiteGraph::addEdge(int u, int v) {
    assert(side[u] != side[v]);

    graph[u].push_back(v);
    graph[v].push_back(u);
}

bool BipartiteGraph::findAugmentingPathFrom(int v, std::vector<int>& mate, std::vector<bool>& visited) {
    if (visited[v]) {
        return false;
    }

    visited[v] = true;

    for (int u : graph[v]) if (mate[u] == -1 || findAugmentingPathFrom(mate[u], mate, visited)) {
            mate[v] = u;
            mate[u] = v;
            return true;
        }

    return false;
}

bool BipartiteGraph::findAugmentingPath(std::vector<int>& mate) {
    std::vector<bool> visited(n, false);

    bool found = false;
    for (int v = 0; v < n; v++) {
        if (mate[v] == -1 && findAugmentingPathFrom(v, mate, visited)) {
            found = true;
        }
    }

    return found;
}

std::vector<int> BipartiteGraph::findMatching() {
    std::vector<int> mate(n, -1);
    while (findAugmentingPath(mate)) {}

    return mate;
}

std::vector<bool> BipartiteGraph::findVertexCoverFromMatching(std::vector<int>& mate) {
    std::vector<bool> visited(n, false);
    std::queue <int> q;
    for (int i = 0; i < n; i++) {
        if (side[i] && mate[i] == -1) {
            visited[i] = true;
            q.push(i);
        }
    }

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : graph[u]) if (!visited[v] && v != mate[u]) {
                visited[v] = true;

                int x = mate[v];
                if (x != -1 && !visited[x]) {
                    visited[x] = true;
                    q.push(x);
                }
            }
    }

    std::vector<bool> inCover(n, false);
    for (int i = 0; i < n; i++) {
        inCover[i] = visited[i] xor side[i];
    }

    return inCover;
}
