#include <cstddef>
#include <fstream>
#include <iostream>
#include <mat_print.hpp>
#include <vector>

typedef std::vector<std::vector<unsigned> > Graph;

void load_nsg(const char *filename, Graph &final_graph_) {
    /**
     * load nsg file
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
    Graph nsg;
    load_nsg("./sift_100NN_100.graph", nsg);
    std::cout << nsg.size() << std::endl;
    std::cout << nsg[0].size() << std::endl;
    return 0;
}