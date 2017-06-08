#include <iostream>
#include <set>
#include <vector>

void printGraph(int n, const std::vector<std::pair<int,int>>& edges, std::ostream& stream = std::cout) {
    stream << n << ' ' << edges.size() << '\n';

    for (auto& edge : edges) {
        stream << edge.first << ' ' << edge.second << '\n';
    }
}

int randomInt(int range) { return rand() % range; }

std::vector<std::pair<int,int>> getRandomGraph(int n, int vertexCoverSize, double density) {
    std::set<int> vertexCover;
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

    int n, vertexCoverSize;
    double density;

    std::cin >> n >> vertexCoverSize >> density;

    printGraph(n, getRandomGraph(n, vertexCoverSize, density));

    return 0;
}
