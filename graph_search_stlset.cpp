#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <utility>
#include <vector>

#define MAXN 1000100

typedef std::vector<unsigned> VectorXi;
typedef std::vector<float> VectorXf;
typedef std::vector<VectorXi> MatrixXi;
typedef std::vector<VectorXf> MatrixXf;
typedef MatrixXi Graph;

struct Candidate { // TODO: class
    size_t idx;
    float dist;
    Candidate(size_t idx, VectorXf &query, MatrixXf &base) {
        this->idx = idx;
        // this->dist = (query - base.row(idx).adjoint()).array().square().sum();
        VectorXf &target = base[idx];
        float d = 0;
        for (unsigned i = 0; i < query.size(); i++) {
            d += (query[i] - target[i]) * (query[i] - target[i]);
        }
        this->dist = d;
    }
    bool friend operator < (Candidate a, Candidate b) {
        // avoid a.dist == b.dist and a!=b, but b was ignored.
        if (a.dist != b.dist) {
            return a.dist < b.dist;
        } else {
            return a.idx < b.idx;
        }
    }
};

// template <class T>
// void make_shape(std::vector<T> &data, unsigned n, unsigned m) {
//     for (unsigned i = 0; i < n; i++) {
//         T tmp(k);
//         data.push_back(tmp);
//     }
// }

void load_graph(const char *filename, Graph &graph_) {
    /**
     * load graph file
     * the format of file (every number is usigned):
     * k_1 id_{11} id_{12} ... id_{1k_1}
     * k_2 id_{21} id_{22} ... id_{2k_2}
     * ...
     * (there should be 1M lines, and every k equals 100).
     */
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) {
        std::cout << "Error: cannot open " << filename << std::endl;
        exit(-1);
    };
    while (!in.eof()) {
        unsigned k;
        in.read((char *)&k, sizeof(unsigned));
        if (in.eof())
            break;
        std::vector<unsigned> tmp(k);
        in.read((char *)tmp.data(), k * sizeof(unsigned));
        graph_.push_back(tmp);
    }
}

void load_fvecs(const char *filename, MatrixXf &data, unsigned &num,
                unsigned &dim) {
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) {
        std::cout << "Error: cannot open " << filename << std::endl;
        exit(-1);
    };
    in.read((char *)&dim, 4);           //读取向量维度
    in.seekg(0, std::ios::end);         //光标定位到文件末尾
    std::ios::pos_type ss = in.tellg(); //获取文件大小（多少字节）
    size_t fsize = (size_t)ss;
    num = (unsigned)(fsize / (dim + 1) / 4); //数据的个数

    in.seekg(0, std::ios::beg); //光标定位到起始处
    float entry;
    for (unsigned i = 0; i < num; i++) {
        in.seekg(4, std::ios::cur); //光标向右移动4个字节
        VectorXf tmp(dim);
        in.read((char *)tmp.data(), dim * sizeof(float));
        data.push_back(tmp);
    }
    in.close();
}

void load_ivecs(const char *filename, MatrixXi &data, unsigned &num,
                unsigned &dim) {
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) {
        std::cout << "Error: cannot open " << filename << std::endl;
        exit(-1);
    };
    in.read((char *)&dim, 4);           //读取向量维度
    in.seekg(0, std::ios::end);         //光标定位到文件末尾
    std::ios::pos_type ss = in.tellg(); //获取文件大小（多少字节）
    size_t fsize = (size_t)ss;
    num = (unsigned)(fsize / (dim + 1) / 4); //数据的个数

    in.seekg(0, std::ios::beg); //光标定位到起始处
    unsigned entry;
    for (unsigned i = 0; i < num; i++) {
        in.seekg(4, std::ios::cur); //光标向右移动4个字节
        VectorXi tmp(dim);
        in.read((char *)tmp.data(), dim * sizeof(unsigned));
        data.push_back(tmp);
    }
    in.close();
}

void graph_search_(MatrixXf &database, VectorXf &query, Graph &graph, size_t start_idx,
                  size_t k, size_t pool_size, VectorXi &neighbors, unsigned points_num,
                  unsigned queries_num, unsigned dim) {
    std::set<Candidate> pool;
    std::set<Candidate>::iterator it, it_;
    size_t cnt;   // to count the size of pool
    bool checked[MAXN];
    bool inset[MAXN];
    memset(checked, false, MAXN);
    memset(inset, false, MAXN);
    pool.insert(Candidate(start_idx, query, database)); // insert start point
    inset[start_idx] = true;

    // beam search
    while (true) {
        // find first unchecked point
        for (it = pool.begin() ; it != pool.end(); it++) {
            if (!checked[it->idx]) {
                checked[it->idx] = true; // mark as checked
                break;
            }
        }
        if (it != pool.end()) {          // fail to find, so all checked.
            size_t p = it->idx;          // index of point finded
            cnt = pool.size();
            for (unsigned i:graph[p]) {    // insert all neighbors
                if (!inset[i]) {
                    pool.insert(Candidate(i, query, database));
                    inset[i] = true;
                    cnt++;
                }
            }
            // std::cout << cnt << pool.size() << std::endl;
            // erase extra candidates
            while (cnt > pool_size) {
                pool.erase(--pool.end());
                cnt--;
            }
            // std::cout << pool.size() << std::endl;
        } else {
            break;
        }
    }
    
    // collect neighbors
    it = pool.begin();
    for (unsigned i = 0; i < k; i++) {
        neighbors[i] = it->idx;
        it++;
    }
}

double graph_search(MatrixXf &database, MatrixXf &querytable, Graph &graph,
                  size_t k, size_t pool_size, MatrixXi &predicts, unsigned points_num,
                  unsigned queries_num, unsigned dim) {
    std::cout << "\nStart searching!" << std::endl;
    auto t2 = std::chrono::high_resolution_clock::now();
    auto t1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> total = t2 - t1;
    for (unsigned qid = 0; qid < queries_num; qid++) {
        if (!(qid % 100)) {
            t2 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> diff = t2 - t1;
            total += diff;
            printf("[%05d/%05d] Queries Per Second: %.4f", qid, queries_num, 100 / diff.count());
            std::cout << std::endl;
            t1 = t2;
        }
        VectorXf& query = querytable[qid];
        size_t start_idx = rand() % points_num;
        VectorXi neighbors(k);
        graph_search_(database, query, graph, start_idx, k, 
            pool_size, neighbors, points_num, queries_num, dim);
        predicts.push_back(neighbors);
    }
    return queries_num / total.count();
}

double average_recall(MatrixXi &predicts, MatrixXi &groundtruth, unsigned queries_num, unsigned k) {
    double s = 0;
    unsigned cnt, gid;
    bool flag;
    for (unsigned i = 0; i < queries_num; i++) {
        cnt = 0;
        for (unsigned j = 0; j < k; j++) {
            gid = groundtruth[i][j];
            flag = false;
            for (unsigned t = 0; t < k; t++) {
                if (predicts[i][t] == gid) {
                    flag = true;
                    break;
                }
            }
            if (flag) {
                cnt++;
            }
        }
        s += cnt * 1. / k;
    }
    return s / queries_num;
}

int main(int argc, char **argv) {
    MatrixXf database;
    MatrixXf querytable;
    MatrixXi groundtruth;
    double acc, qps;
    unsigned points_num, queries_num, dim, k_max;
    if (argc != 7) {
        std::cout << "The program needs 6 parameters!" << std::endl;
        exit(-1);
    }
    size_t k = atoi(argv[1]), pool_size = atoi(argv[2]);
    if (k > pool_size) {
        std::cout << "k should be smaller than pool_size." << std::endl;
        exit(-1);
    }
    load_fvecs(argv[3], database, points_num, dim);
    load_fvecs(argv[4], querytable, queries_num, dim);
    load_ivecs(argv[5], groundtruth, queries_num, k_max);


    std::cout << "points_num: " << points_num << std::endl
              << "queries_num: " << queries_num << std::endl
              << "k_max: " << k_max << std::endl
              << "data dimension: " << dim << std::endl;
    Graph graph;
    load_graph(argv[6], graph);
    std::cout << "Graph loaded." << std::endl;

    MatrixXi predicts;
    // make_shape(predicts, queries_num, k);
    srand((unsigned) time(NULL));
    qps = graph_search(database, querytable, graph, k, pool_size, predicts,
                    points_num, queries_num, dim);
    acc = average_recall(predicts, groundtruth, queries_num, k);
    std::cout << "Average recall: " << acc << std::endl;
    std::cout << "Queries per second: " << qps << std::endl;
    return 0;
}