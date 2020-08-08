#include <bits/stdc++.h>

typedef std::vector<std::vector<unsigned>> MatrixXi;
typedef std::vector<std::vector<char>> MatrixXc;
typedef MatrixXi Graph;
typedef std::vector<std::vector<float>> MatrixXf;
typedef std::vector<unsigned> VectorXi;
typedef std::vector<float> VectorXf;
typedef std::vector<char> VectorXc;
typedef std::vector<MatrixXf> CenterTable;

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
        // std::cout << tmp.size() << std::endl;
    }
    in.close();
}

void save_fvecs(const char *filename, MatrixXf &data, unsigned &num,
                 unsigned &dim) {
    std::ofstream out(filename, std::ios::binary);
    if (!out.is_open()) {
        std::cout << "Error: cannot open " << filename << std::endl;
        exit(-1);
    };
    // out.((char *)&dim, 4);           //读取向量维度
    // out.seekg(0, std::ios::end);         //光标定位到文件末尾
    // std::ios::pos_type ss = in.tellg(); //获取文件大小（多少字节）
    // size_t fsize = (size_t)ss;
    // num = (unsigned)(fsize / (dim + 1) / 4); //数据的个数

    // in.seekg(0, std::ios::beg); //光标定位到起始处
    float entry;
    for (unsigned i = 0; i < num; i++) {
        // in.seekg(4, std::ios::cur); //光标向右移动4个字节
        out.write((char *)&dim, 4);
        VectorXf &tmp = data[i];
        // std::cout << i << tmp.size() << std::endl;
        out.write((char *)tmp.data(), dim * sizeof(float));
        // data.push_back(tmp);
    }
    out.close();
}

void load_cvecs(const char *filename, MatrixXc &data, unsigned &num,
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
    num = (unsigned)(fsize / (dim + 4)); //数据的个数

    in.seekg(0, std::ios::beg); //光标定位到起始处
    float entry;
    for (unsigned i = 0; i < num; i++) {
        in.seekg(4, std::ios::cur); //光标向右移动4个字节
        VectorXc tmp(dim);
        in.read((char *)tmp.data(), dim * sizeof(char));
        data.push_back(tmp);
        // std::cout << tmp.size() << std::endl;
    }
    in.close();
}

void save_cvecs(const char *filename, MatrixXc &data, unsigned &num,
                unsigned &dim) {
    std::ofstream out(filename, std::ios::binary);
    if (!out.is_open()) {
        std::cout << "Error: cannot open " << filename << std::endl;
        exit(-1);
    };
    // out.((char *)&dim, 4);           //读取向量维度
    // out.seekg(0, std::ios::end);         //光标定位到文件末尾
    // std::ios::pos_type ss = in.tellg(); //获取文件大小（多少字节）
    // size_t fsize = (size_t)ss;
    // num = (unsigned)(fsize / (dim + 1) / 4); //数据的个数

    // in.seekg(0, std::ios::beg); //光标定位到起始处
    float entry;
    for (unsigned i = 0; i < num; i++) {
        // in.seekg(4, std::ios::cur); //光标向右移动4个字节
        // out.write((char *)&dim, 1);
        VectorXc &tmp = data[i];
        // std::cout << i << tmp.size() << std::endl;
        out.write((char *)tmp.data(), dim * sizeof(char));
        // data.push_back(tmp);
    }
    out.close();
}

int main(void) {
    MatrixXc R, R_;
    unsigned tmp1, tmp2;
    std::string s1 = "code.cvecs";
    std::string s2 = "./opq_data/";
    load_cvecs((s2 + s1).c_str(), R, tmp1, tmp2);
    for (unsigned j=0; j < tmp2; j++) {
        VectorXc v(tmp1);
        for (unsigned i=0; i<tmp1; i++) {
            v[i] = R[i][j]; 
        }
        R_.push_back(v);
    }
    std::cout << tmp1 << " " << tmp2 << std::endl;
    save_cvecs(s1.c_str(), R_, tmp2, tmp1);

    return 0;
}