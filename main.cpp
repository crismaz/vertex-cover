#include "Graph.h"
#include <iostream>

Graph readGraph(std::istream& stream = std::cin) {
    std::size_t  n;
    stream >> n;

    Graph graph(n);

    int m;
    stream >> m;

    while (m--) {
        int u, v;
        stream >> u >> v;

        graph.addEdge(u, v);
    }

    return graph;
}

int main() {
    std::ios_base::sync_with_stdio(false);

    Graph graph = readGraph();
    auto vertexCover = graph.findVertexCover(true);

    std::cout << std::count(vertexCover.begin(), vertexCover.end(), true) << '\n';
    for (int i = 0; i < vertexCover.size(); i++) {
        if (vertexCover[i]) {
            std::cout << i << ' ';
        }
    }

    return 0;
}
