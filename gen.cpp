#include <iostream>
#include <set>
#include <vector>

/// Prints the graph to the output stream
///
/// If n and m are the number of vertices and edges respectively, then the output format is:
///     n m
///     u1 v1
///     u2 v2
///      ...
///     um vm
void printGraph(int n, const std::vector<std::pair<int,int>>& edges, std::ostream& stream = std::cout) {
    stream << n << ' ' << edges.size() << '\n';

    for (auto& edge : edges) {
        stream << edge.first << ' ' << edge.second << '\n';
    }
}

/// Returns a random integer from the set [0, 1, ..., range-1]
int randomInt(int range) { return rand() % range; }

/// Creates a random undirected graph with the given parameters
///
/// \param n               number of vertices
/// \param vertexCoverSize target vertex cover size (the actual vertex cover size can be smaller!)
/// \param density         required density of the graph
/// \return                a vector of pairs, each pair being an undirected edge in the resulting graph
std::vector<std::pair<int,int>> getRandomGraph(int n, int vertexCoverSize, double density) {
    std::set<int> vertexCover; // vertices that will form a valid vertex cover in the constructed graph
    while (vertexCover.size() < vertexCoverSize) {
        vertexCover.insert(randomInt(n));
    }

    std::vector<int> vertexCoverVec(vertexCover.begin(), vertexCover.end());

    int numEdges = (int) (n * (n - 1) / 2 * density);
    int maxEdges = vertexCoverSize * (vertexCoverSize - 1) / 2 + vertexCoverSize * (n - vertexCoverSize);

    if (numEdges > maxEdges) {
        throw new std::invalid_argument("Density too high");
    }

    std::set<std::pair<int,int>> edges;
    while (edges.size() < numEdges) {
        int u = vertexCoverVec[randomInt((int) vertexCoverVec.size())];
        int v = randomInt(n);

        if (u != v) {
            if (u > v) {
                std::swap(u, v);
            }

            edges.insert({u, v});
        }
    }

    return std::vector<std::pair<int,int>> (edges.begin(), edges.end());
};

int main() {
    std::ios_base::sync_with_stdio(false);
    srand(time(0));

    int n, vertexCoverSize;
    double density;

    std::cin >> n >> vertexCoverSize >> density;

    printGraph(n, getRandomGraph(n, vertexCoverSize, density));

    return 0;
}
