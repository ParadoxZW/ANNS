#include <Eigen/Dense>
#include <iostream>
#define DBL_MAX 1.0e99
using namespace std;
using namespace Eigen;

MatrixXd kmeans(MatrixXd, int, MatrixXd &);
double distance(MatrixXd, MatrixXd);
MatrixXd randCent(MatrixXd, int);

int main() {
    MatrixXd data(12, 2);
    data << 1, 1, 101, 100, 2, 2, 50, 51, 100, 100, 2, 1, 100, 101, 51, 51, 101,
        101, 50, 50, 1, 2, 51, 50;

    cout << "数据集：" << endl << data << endl;
    MatrixXd centroids = randCent(data, 3);
    MatrixXd subCenter = kmeans(data, 3, centroids);
    cout << "中心：" << endl << centroids << endl;
    cout << "分类和距离：" << endl << subCenter << endl;
    return 0;
}

MatrixXd randCent(MatrixXd data, int k) {
    int n = data.cols();
    MatrixXd centroids = MatrixXd::Zero(k, n);
    double min, range;
    for (int i = 0; i < n; i++) {
        min = data.col(i).minCoeff();
        range = data.col(i).maxCoeff() - min;
        centroids.col(i) = min * MatrixXd::Ones(k, 1) +
                           MatrixXd::Random(k, 1).cwiseAbs() * range;
    }
    return centroids;
}

MatrixXd kmeans(MatrixXd data, int k, MatrixXd &centroids) {
    int m = data.rows();
    int n = data.cols();
    MatrixXd subCenter = MatrixXd::Zero(m, 2);
    bool change = true;
    while (change) {
        change = false;
        for (int i = 0; i < m; i++) {
            double minDist = DBL_MAX;
            int minIndex = 0;
            for (int j = 0; j < k; j++) {
                double dist = distance(data.row(i), centroids.row(j));
                if (dist < minDist) {
                    minDist = dist;
                    minIndex = j;
                }
            }

            if (subCenter(i, 0) != minIndex) {
                change = true;
                subCenter(i, 0) = minIndex;
            }
            subCenter(i, 1) = minDist;
        }

        for (int i = 0; i < k; i++) {
            MatrixXd sum_all = MatrixXd::Zero(1, n);
            int r = 0;
            for (int j = 0; j < m; j++) {
                if (subCenter(j, 0) == i) {
                    sum_all += data.row(j);
                    r++;
                }
            }
            centroids.row(i) = sum_all.row(0) / r;
        }
    }
    return subCenter;
}

double distance(MatrixXd vecA, MatrixXd vecB) {
    return ((vecA - vecB) * (vecA - vecB).transpose())(0, 0);
}