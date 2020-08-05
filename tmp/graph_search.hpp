#pragma once

#include <cmath>
#include <vector>
#include <set>
#include <map>
#include <utility>
#include <algorithm>
#include <Eigen/Dense>

#define MAXN 1000100

typedef std::vector<std::vector<unsigned> > Graph; //
using Eigen::MatrixXi;
using Eigen::MatrixXf;
using Eigen::VectorXi;
using Eigen::VectorXf;

struct Candidate { // TODO: class
    size_t idx;
    float dist;
    // Candidate(size_t idx, VectorXf &query, MatrixXf &base, std::map<size_t, float> &dists) {
    //     this->idx = idx;
    //     auto it = dists.find(idx);
    //     if (it == dists.end()) {
    //         this->dist = (query - base.row(idx).adjoint()).array().square().sum();
    //         dists[idx] = this->dist;
    //     } else {
    //         this->dist = it->second;
    //     }
    // }
    Candidate(size_t idx, VectorXf &query, MatrixXf &base) {
        this->idx = idx;
        this->dist = (query - base.row(idx).adjoint()).array().square().sum();
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


