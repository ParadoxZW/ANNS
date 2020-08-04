#include <cstddef>
#include <fstream>
#include <iostream>
#include <vector>
#include <mat_print.hpp>

void load_fvecs(char *filename, float *&data, unsigned &num, unsigned &dim) {
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) {
        std::cout << "open file error" << std::endl;
        exit(-1);
    }
    in.read((char *)&dim, 4);                        //读取向量维度
    in.seekg(0, std::ios::end);                      //光标定位到文件末尾
    std::ios::pos_type ss = in.tellg();              //获取文件大小（多少字节）
    size_t fsize = (size_t)ss;
    num = (unsigned)(fsize / (dim + 1) / 4);         //数据的个数
    data = new float[(size_t)num * (size_t)dim];

    in.seekg(0, std::ios::beg);                      //光标定位到起始处
    for (size_t i = 0; i < num; i++) {
        in.seekg(4, std::ios::cur);                  //光标向右移动4个字节
        in.read((char *)(data + i * dim), dim * 4);  //读取数据到一维数据data中
    }
    in.close();
}

int main(int argc, char **argv) {
    float *data_load = NULL;
    unsigned points_num, dim;
    load_fvecs("./sift/sift_base.fvecs", data_load, points_num, dim);
    std::cout << "points_num：" << points_num << std::endl
              << "data dimension：" << dim << std::endl;
    std::vector<float> v(data_load, data_load + 10);
    std::cout << showpoint << (v | vw::all) << std::endl;

        // VectorXf v1 = (*database).row((*groundtruth)(20, 13));
    // VectorXf v2 = (*database).row((*groundtruth)(20, 14));
    // VectorXi v3 = (*groundtruth).row(0);
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
    return 0;
}