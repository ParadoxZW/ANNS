#include <cstddef>
#include <fstream>
#include <iostream>
#include <mat_print.hpp>
#include <vector>
const int maxn = 1000100;
typedef std::vector<std::vector<unsigned>> Graph;

int father[maxn];
bool isRoot[maxn];

int findFather(int v) {
    if (father[v] == v)
        return v;
    else {
        int f = findFather(father[v]);
        father[v] = f;
        return f;
    }
}

void Union(int a, int b) {
    int faA = findFather(a);
    int faB = findFather(b);
    if (faA != faB) {
        father[faB] = faA;
    }
}

void init(int N) {
    for (int i = 0; i < N; i++) {
        father[i] = i;
    }
}

int calBlock(int N) {
    int block = 0;
    for (int i = 0; i < N; i++) {
        isRoot[findFather(i)] = true; // 不能直接用father[i]
    }
    for (int i = 0; i < N; i++) {
        block += isRoot[i];
    }
    return block;
}

void load_graph(const char *filename, Graph &final_graph_) {
    /**
     * load graph file
     * the format of file (every number is usigned):
     * k_1 id_{11} id_{12} ... id_{1k_1}
     * k_2 id_{21} id_{22} ... id_{2k_2}
     * ...
     * (there should be 1M lines, and every k equals 100).
     */
    std::ifstream in(filename, std::ios::binary);
    while (!in.eof()) {
        unsigned k;
        in.read((char *)&k, sizeof(unsigned));
        if (in.eof())
            break;
        std::vector<unsigned> tmp(k);
        in.read((char *)tmp.data(), k * sizeof(unsigned));
        final_graph_.push_back(tmp);
    }
}

int main(int argc, char **argv) {
    Graph graph;
    int N = 1000000;
    init(N);
    load_graph("./sift/sift_100NN_100.graph", graph);
    // std::cout << graph.size() << std::endl;
    // std::cout << graph[0].size() << std::endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < 100; j++) {
            Union(i, graph[i][j]);
        }
    }
    int n = calBlock(N);
    std::cout << n << std::endl;

    return 0;
}