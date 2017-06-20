# Vertex Cover

This project provides an implementation of a few algorithms for the [Vertex Cover](https://en.wikipedia.org/wiki/Vertex_cover) problem.

## Description
It is well known that the Vertex Cover problem is [NP-complete](https://en.wikipedia.org/wiki/NP-completeness), and thus, assuming [P is not equal to NP](https://en.wikipedia.org/wiki/P_versus_NP_problem), it does not admit a [polynomial time algorithm](https://en.wikipedia.org/wiki/Time_complexity). The algorithms implemented in this project are exact and work in exponential time.

The code in this repository allows for comparing two general ways of finding the minimum vertex cover.
- _Baseline_ - a simple algorithm that combines branching and polynomial-time graph reductions
- _AboveLP_ - a [fixed-parameter tractable](https://en.wikipedia.org/wiki/Parameterized_complexity) algorithm solving the Vertex Cover Above Linear Programming problem

The latter algorithm achieves very good results on random dense graphs with vertex cover size bounded by 3n/5.

## Usage
Compile _vertex_cover_ target using cmake (version required: >= 3.7).

For direct use, run:
```
./vertex_cover [-useLp] [-anyLpSolution]
```
and enter the graph description in the format
```
n m
u0 v0
u1 v1
...
um vm
```
where n and m are the number of vertices and edges respectively, and (ui, vi) are edges of the graph. Vertices are numbered from 0 to n-1.

### Flags
The main program takes two flags
- -useLp
If set, _AboveLP_ will be used, and _Baseline_ otherwise.
- -anyLpSolution
Relevant only if _-useLp_ is set. This flag changes the behavior of the algorithm, making it much faster in practice, while losing a theoretical guarantee on its running time.

### Test generation
To directly generate random test cases, compile _gen_ target, and run
```
./gen
```
entering
```
n vc density
```
where n, vc and density are the number of vertices, an upper bound on the size of the minimum vertex cover, and the density of the graph, respectively.

### Running in batches
Having compiled both _vertex_cover_ and _gen_, you can run a batch of tests with different parameters by running
```
python test.py ./vertex_cover ./gen [-raw]
```
and following the instructions printed to stdout. The `-raw` flag prevents the script from printing any extra information or formatting.

### Plotting the results
The output from _test.py_ with the `-raw` flag can be passed as standard input to _plot.py_, which then generates a simple plot.
