#include "Graph.h"
#include <cassert>
#include <iostream>
#include <string>

/// Reads the graph from the input stream - see printGraph for input format
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

int main(int argc, char* argv[]) {
    std::ios_base::sync_with_stdio(false);

    bool useLinearProgramming = false, anyLpSolution = false;
    for (int index = 1; index < argc; index++) {
        std::string arg(argv[index]);
        if (arg == "-useLp") {
            useLinearProgramming = true;
        } else if (arg == "-anyLpSolution") {
            anyLpSolution = true;
        }
    }

    Graph graph = readGraph();
    auto vertexCover = graph.findVertexCover(useLinearProgramming, anyLpSolution);

    std::cout << std::count(vertexCover.begin(), vertexCover.end(), true) << '\n';
    for (int i = 0; i < vertexCover.size(); i++) {
        if (vertexCover[i]) {
            std::cout << i << ' ';
        }
    }

    assert(graph.isVertexCover(vertexCover)); // check that a valid vertex cover was found

    return 0;
}
