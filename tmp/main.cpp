#include <chrono>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>

#include "graph_search.hpp"

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

void load_fvecs(const char *filename, MatrixXf *&data, unsigned &num,
                unsigned &dim) {
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) {
        std::cout << "open file error" << std::endl;
        exit(-1);
    }
    in.read((char *)&dim, 4);           //读取向量维度
    in.seekg(0, std::ios::end);         //光标定位到文件末尾
    std::ios::pos_type ss = in.tellg(); //获取文件大小（多少字节）
    size_t fsize = (size_t)ss;
    num = (unsigned)(fsize / (dim + 1) / 4); //数据的个数
    data = new MatrixXf(num, dim);

    in.seekg(0, std::ios::beg); //光标定位到起始处
    float entry;
    for (size_t i = 0; i < num; i++) {
        in.seekg(4, std::ios::cur); //光标向右移动4个字节
        for (size_t j = 0; j < dim; j++) {
            in.read((char *)(&entry), 4); //读取数据到一维数据data中
            (*data)(i, j) = entry;
        }
    }
    in.close();
}

void load_ivecs(const char *filename, MatrixXi *&data, unsigned &num,
                unsigned &dim) {
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) {
        std::cout << "open file error" << std::endl;
        exit(-1);
    }
    in.read((char *)&dim, 4);           //读取向量维度
    in.seekg(0, std::ios::end);         //光标定位到文件末尾
    std::ios::pos_type ss = in.tellg(); //获取文件大小（多少字节）
    size_t fsize = (size_t)ss;
    num = (unsigned)(fsize / (dim + 1) / 4); //数据的个数
    data = new MatrixXi(num, dim);

    in.seekg(0, std::ios::beg); //光标定位到起始处
    unsigned entry;
    for (size_t i = 0; i < num; i++) {
        in.seekg(4, std::ios::cur); //光标向右移动4个字节
        for (size_t j = 0; j < dim; j++) {
            in.read((char *)(&entry), 4); //读取数据到一维数据data中
            (*data)(i, j) = entry;
        }
    }
    in.close();
}

void graph_search_(MatrixXf &database, VectorXf &query, Graph &graph, size_t start_idx,
                  size_t k, size_t pool_size, VectorXi &neighbors, unsigned points_num,
                  unsigned queries_num, unsigned dim) {
    std::set<Candidate> pool;
    std::set<Candidate>::iterator it, it_;
    std::map<size_t, float> dists; 
    size_t i;   // to count the size of pool
    bool checked[MAXN];
    bool inset[MAXN];
    memset(checked, false, MAXN);
    memset(inset, false, MAXN);
    pool.insert(Candidate(start_idx, query, database, dists)); // insert start point

    // beam search
    while (true) {
        i = 0;
        // find first unchecked point
        for (it = pool.begin() ; it != pool.end(); it++) {
            if (!checked[i]) {
                checked[i] = true; // mark as checked
                break;
            }
            i++;
        }
        if (it != pool.end()) {          // fail to find, so all checked.
            size_t p = it->idx;          // index of point finded
            for (size_t idx:graph[p]) {  // insert all neighbors
                if (!inset[idx]) {
                    pool.insert(Candidate(idx, query, database, dists));
                    inset[idx] = true;
                }
                i++;
            }
            // erase extra candidates
            while (i > pool_size) {
                // std::cout << i << std::endl;
                it_ = --pool.end();
                // inset[it_->idx] = false;
                pool.erase(it_);
                i--;
            }
        } else {
            break;
        }
    }
    
    // collect neighbors
    it = pool.begin();
    for (size_t i = 0; i < k; i++) {
        neighbors(i) = it->idx;
        it++;
    }
}

void graph_search(MatrixXf &database, MatrixXf &querytable, Graph &graph,
                  size_t k, size_t pool_size, MatrixXi &predicts, unsigned points_num,
                  unsigned queries_num, unsigned dim) {
    std::cout << "\nStart searching!" << std::endl;
    auto t1 = std::chrono::high_resolution_clock::now();
    auto t2 = std::chrono::high_resolution_clock::now();
    for (size_t qid = 0; qid < queries_num; qid++) {
        if (!(qid % 100)) {
            t2 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> diff = t2 - t1;
            printf("%d/%d | Queries Per Second: %.4f\n", qid, queries_num, 100 / diff.count());
            t1 = t2;
        }
        VectorXf query = querytable.row(qid);
        size_t start_idx = rand() % points_num;
        VectorXi neighbors(k);
        graph_search_(database, query, graph, start_idx, k, 
            pool_size, neighbors, points_num, queries_num, dim);
        predicts.row(qid) = neighbors;

    }
}

double average_recall(MatrixXi &predicts, MatrixXi &groundtruth, unsigned queries_num, unsigned k) {
    double s = 0;
    unsigned cnt, gid;
    bool flag;
    for (size_t i = 0; i < queries_num; i++) {
    size_t tt = 0;
        cnt = 0;
        for (size_t j = 0; j < k; j++) {
            gid = groundtruth(i, j);
            flag = false;
            for (size_t t = 0; t < k; t++) {
                if (predicts(i, t) == gid) {
                    if (t < tt) {
                        // qid now_pid old_pid tid
                        std::cout << "error " << i << " " << t << " " << tt << " " << j << std::endl;
                    }
                    tt = t;
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
    MatrixXf *database = NULL;
    MatrixXf *querytable = NULL;
    MatrixXi *groundtruth = NULL;
    double acc;
    unsigned points_num, queries_num, dim, k_max;
    size_t k = 100, pool_size = 200;
    load_fvecs("./sift/sift_base.fvecs", database, points_num, dim);
    load_fvecs("./sift/sift_query.fvecs", querytable, queries_num, dim);
    load_ivecs("./sift/sift_groundtruth.ivecs", groundtruth, queries_num, k_max);

    // VectorXf = (*querytable).row(16);
    // (*groundtruth)(16, )

    std::cout << "points_num: " << points_num << std::endl
              << "queries_num: " << queries_num << std::endl
              << "k_max: " << k_max << std::endl
              << "data dimension: " << dim << std::endl;
    // std::cout << (*database).rows() << std::endl;
    // std::cout << (*database).cols() << std::endl;
    Graph graph;
    load_graph("./sift_100NN_100.graph", graph);
    std::cout << "Graph loaded." << std::endl;
    std::cout << graph.size() << std::endl;
    std::cout << graph[0].size() << std::endl;

    // VectorXf v1 = (*database).row((*groundtruth)(20, 13));
    // VectorXf v2 = (*database).row((*groundtruth)(20, 14));
    // VectorXf v3 = (*querytable).row(20);
    // std::cout << v1.adjoint() << std::endl << std::endl;
    // std::cout << v2.adjoint() << std::endl << std::endl;
    // std::cout << v3.adjoint() << std::endl << std::endl;
    // std::cout << (v1 - v3).norm() << std::endl;
    // std::cout << (v2 - v3).norm() << std::endl;

    // VectorXf r = (*database).row(10);
    // std::cout << 1 << std::endl;
    // auto g = (*database);
    // g.rowwise() -= r.adjoint();
    // std::cout << 2 << std::endl;
    // VectorXf v = g.array().square().rowwise().sum();
    // std::cout << v.size() << std::endl;
    // typedef std::pair<float, size_t> kv;
    // typedef std::vector<kv> kvs;
    // kvs arr;
    // for (size_t j = 0; j < points_num; j++) {
    //     arr.push_back(std::make_pair(v(j), j));
    // }
    // sort(arr.begin(), arr.end());
    // for (size_t j = 0; j < 100; j++) {
    //     std::cout << arr[j+1].second << " " << graph[10][j] << " " << v(graph[10][j]) << std::endl;
    // }


    MatrixXi predicts(queries_num, k);
    srand((unsigned) time(NULL));
    graph_search(*database, *querytable, graph, k, pool_size, predicts,
                 points_num, 500, dim);
    acc = average_recall(predicts, *groundtruth, 500, k);
    std::cout << acc << std::endl;
    std::cout << predicts.row(0) << std::endl;
    return 0;
}