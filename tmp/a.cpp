#include <Eigen/Dense>
#include <iostream>

using namespace Eigen;
using namespace std;

int main() {
    MatrixXd m = MatrixXd::Random(3, 3);
    MatrixXd n = MatrixXd::Random(3, 3);
    std::cout << m * n << std::endl;
    std::cout << m.row(1) * n.col(0)<< std::endl;
    VectorXd v1 = n.col(0);
    VectorXd v2 = m.row(1);
    std::cout << v1 << std::endl;
    std::cout << v2 << std::endl;
    std::cout << v1.adjoint() * v2 << std::endl;
    std::cout << ((v1 - v2).norm()) << std::endl;
    m.row(2) = v2;
    std::cout << m << std::endl;
    std::cout << sizeof(unsigned) << std::endl;
    std::cout << sizeof(size_t) << std::endl;
}