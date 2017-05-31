#include "BipartiteGraph.h"

BipartiteGraph::BipartiteGraph(std::size_t n, std::vector<bool>& side): n(n), graph(n), side(side) {
    assert(side.size() == n);
}

void BipartiteGraph::addEdge(int u, int v) {
    assert(side[u] != side[v]);

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

std::vector<bool> BipartiteGraph::findVertexCover() {
    auto mate = findMatching();

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
