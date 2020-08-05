#include <cstddef>
#include <fstream>
#include <iostream>
#include <mat_print.hpp>
#include <sys/types.h>
#include <vector>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

void load_fvecs(const char *filename, float *&data, unsigned &num, unsigned &dim) {
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
    data = new float[(size_t)num * (size_t)dim];

    in.seekg(0, std::ios::beg); //光标定位到起始处
    for (size_t i = 0; i < num; i++) {
        in.seekg(4, std::ios::cur); //光标向右移动4个字节
        in.read((char *)(data + i * dim), dim * 4); //读取数据到一维数据data中
    }
    in.close();
}

void mmap_load(const char *filename, std::vector<float*> &data, unsigned &num, unsigned &dim) {
    int fd = open(filename, 0);
    struct stat statbuf;
    char *start;
    unsigned p = 0;

    fstat(fd, &statbuf);
    start = (char *)mmap(NULL, statbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    dim = *((unsigned *) start);
    num = (unsigned)(statbuf.st_size / (dim + 1) / 4);
    do {
        data.push_back((float *)(start + p + 4));
        p += 4 * (dim + 1);
    } while (p < statbuf.st_size);

    // munmap(start, statbuf.st_size);
}

int main(int argc, char **argv) {
    float *data_load = NULL;
    unsigned points_num, dim;
    load_fvecs("./sift/sift_base.fvecs", data_load, points_num, dim);
    std::vector<float*> data_load2;
    mmap_load("./sift/sift_base.fvecs", data_load2, points_num, dim);

    std::cout << "points_num：" << points_num << std::endl
              << "data dimension：" << dim << std::endl;
    std::vector<float> v(data_load, data_load + 10);
    std::cout << showpoint << (v | vw::all) << std::endl;

    std::vector<float> v2(data_load2[0], data_load2[0] + 10);
    std::cout << showpoint << (v2 | vw::all) << std::endl;
    return 0;
}