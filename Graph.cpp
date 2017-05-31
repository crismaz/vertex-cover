#include "Graph.h"

Graph::Graph(std::size_t n): n(n) {}

void Graph::addEdge(int u, int v) {
    graph[u].push_back(v);
    graph[v].push_back(u);
}

std::vector<bool> Graph::findVertexCover(bool useLinearProgramming) {
    return std::vector<bool>(); // TODO
}
