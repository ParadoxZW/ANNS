#include <cstddef>
#include <fstream>
#include <iostream>
#include <Eigen/Dense>
#include <vector>
#include <mat_print.hpp>

using Eigen::MatrixXd;


void load_fvecs(const char *filename, MatrixXd *&data, unsigned &num, unsigned &dim) {
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
    data = new MatrixXd(num, dim);

    in.seekg(0, std::ios::beg);                      //光标定位到起始处
    unsigned entry;
    for (size_t i = 0; i < num; i++) {
        in.seekg(4, std::ios::cur);                  //光标向右移动4个字节
        for (size_t j = 0; j < dim; j++) {
            in.read((char *)(&entry), 4);            //读取数据到一维数据data中
            (*data)(i, j) = entry;
        }

    }
    in.close();
}

int main(int argc, char **argv) {
    MatrixXd *data_load = NULL;
    unsigned points_num, dim;
    load_fvecs("./sift/sift_base.fvecs", data_load, points_num, dim);
    std::cout << "points_num：" << points_num << std::endl
              << "data dimension：" << dim << std::endl;
    // std::vector<float> v(data_load, data_load + 10);
    // std::cout << showpoint << (v | vw::all) << std::endl;
    std::cout << (*data_load).rows() << std::endl;
    std::cout << (*data_load).cols() << std::endl;
    
    return 0;
}