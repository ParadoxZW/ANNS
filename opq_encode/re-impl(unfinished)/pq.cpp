#include <Eigen/Dense>
#include <iostream>
#define DBL_MAX 1.0e99
using namespace std;
using namespace Eigen;

double distance(MatrixXf &vecA, MatrixXf &vecB) {
    return ((vecA - vecB) * (vecA - vecB).transpose())(0, 0);
}

MatrixXf randCent(MatrixXf &data, int k) {
    int n = data.cols();
    MatrixXf centroids = MatrixXf::Zero(k, n);
    double min, range;
    for (int i = 0; i < n; i++) {
        min = data.col(i).minCoeff();
        range = data.col(i).maxCoeff() - min;
        centroids.col(i) = min * MatrixXf::Ones(k, 1) +
                           MatrixXf::Random(k, 1).cwiseAbs() * range;
    }
    return centroids;
}

VectorXi kmeans(MatrixXf &data, int k, MatrixXf &centroids, unsigned max_iters, float &sum_dist) {
    int m = data.rows();
    int n = data.cols();
    unsigned cnt = 0;
    MatrixXf subCenter = VectorXi::Zero(m);
    bool change = true;
    while (change) {
        change = false;
        sum_dist = 0;
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

            if (subCenter(i) != minIndex) {
                change = true;
                subCenter(i) = minIndex;
            }
            // subCenter(i, 1) = minDist;
            sum_dist += minDist;
        }

        for (int i = 0; i < k; i++) {
            MatrixXf sum_all = MatrixXf::Zero(1, n);
            int r = 0;
            for (int j = 0; j < m; j++) {
                if (subCenter(j) == i) {
                    sum_all += data.row(j);
                    r++;
                }
            }
            centroids.row(i) = sum_all.row(0) / r;
        }
        cnt++;
        if (cnt >= max_iters) {
            break;
        }
    }
    return subCenter;
}

typedef std::vector<MatrixXf> CenterTable;

void train_pq(MatrixXf &X, unsigned M, int num_iter, CenterTable &ct, MatrixXi &idx_table) {
    unsigned k = 256;
    unsigned dim = X.cols();
    unsigned d = dim / M;
    float distortion = 0;
    float dist;
    for (unsigned m = 0; m < M; m++) {
        std::cout << "subplace: " << m;
        Xsub = X.middleCols(m*d, d);
        MatrixXf centroids = randCent(Xsub, k);
        VectorXi idx = kmeans(Xsub, k, centroids, num_iter, dist);
        ct.push_back(centroids);
        idx_table.col(m) = idx;
        std::cout << " subplace distortion " << dist << std::endl;
        distortion += dist;
    }
    std::cout << "distortion: " << distortion << std::endl;
}

void train_opq_np(MatrixXf &X, MatrixXf &R, unsigned M, int num_iter_inner, int num_iter_outer, CenterTable &ct, MatrixXi &idx_table) {
    unsigned k = 256;
    unsigned dim = X.cols();
    unsigned d = dim / M;
    float distortion = 0;
    float dist;
    for (int i_out = 0; i_out < num_iter_outer; i_out++) {
        auto Y = X;
        Xproj = X * R;
        distortion = 0;
        for (unsigned m = 0; m < M; m++) {
            // std::cout << "subplace: " << m;
            Xsub = Xproj.middleCols(m*d, d);
            MatrixXf &centroids = ct[m];
            VectorXi idx = kmeans(Xsub, k, centroids, num_iter_inner, dist);
            // ct.push_back(centroids);
            idx_table.col(m) = idx;
            // std::cout << " subplace distortion " << dist << std::endl;
            distortion += dist;
            auto Ysub = Xsub;
            for(int i = 0; i < idx.size(); i++){
                Ysub.row(i) = centroids.row(idx(i));
            }
            
        }
        std::cout << i_out << " iters, distortion: " << distortion << std::endl;
    }
}

int main() {
    MatrixXf data(12, 2);
    data << 1, 1, 101, 100, 2, 2, 50, 51, 100, 100, 2, 1, 100, 101, 51, 51, 101,
        101, 50, 50, 1, 2, 51, 50;

    cout << "数据集：" << endl << data << endl;
    MatrixXf centroids = randCent(data, 3);
    MatrixXf subCenter = kmeans(data, 3, centroids);
    cout << "中心：" << endl << centroids << endl;
    cout << "分类和距离：" << endl << subCenter << endl;
    return 0;
}
