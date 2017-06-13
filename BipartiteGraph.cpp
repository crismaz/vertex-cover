#include "BipartiteGraph.h"

BipartiteGraph::BipartiteGraph(std::size_t n, const std::vector<bool>& side): n(n), graph(n), side(side) {
    assert(side.size() == n);
}

void BipartiteGraph::addEdge(int u, int v) {
    assert(side[u] != side[v]); // check that the graph is kept bipartite

    graph[u].push_back(v);
    graph[v].push_back(u);
}

/// Finds an augmenting path from the given vertex, and if found, uses it to improve the matching
bool BipartiteGraph::findAugmentingPathFrom(int v, std::vector<int>& mate, std::vector<bool>& visited) const {
    if (visited[v]) {
        return false;
    }

    visited[v] = true;

    for (int u : graph[v]) {
        if (mate[u] == -1 || findAugmentingPathFrom(mate[u], mate, visited)) {
            mate[v] = u;
            mate[u] = v;
            return true;
        }
    }

    return false;
}

/// Find (possibly many) augmenting paths in a single graph traversal
///
/// \param mate see BipartiteGraph::findMatching
/// \return true if at least one path was found
bool BipartiteGraph::findAugmentingPaths(std::vector<int>& mate) const {
    std::vector<bool> visited(n, false);

    bool found = false;
    for (int v = 0; v < n; v++) {
        if (mate[v] == -1 && findAugmentingPathFrom(v, mate, visited)) {
            found = true;
        }
    }

    return found;
}

bool BipartiteGraph::isMatchingOptimal(std::vector<int> mate) const {
    return !findAugmentingPaths(mate);
}

std::vector<int> BipartiteGraph::findMatching() const {
    std::vector<int> mate(n, -1);
    while (findAugmentingPaths(mate)) {}

    return mate;
}

std::vector<bool> BipartiteGraph::findVertexCoverFromMatching(const std::vector<int>& mate) const {
    std::vector<bool> visited(n, false);
    std::queue <int> q;
    for (int i = 0; i < n; i++) {
        if (side[i] && mate[i] == -1) {
            visited[i] = true;
            q.push(i);
        }
    }

    while (!q.empty()) { // breadth first search
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
