# ANNS

## Introduction

It's a project to re-implement the C++ program of graph-based ANNS and explore all kinds of
variants to improve the performance of the algorithm.

## Requirements & Preparations

+ OS: Ubuntu/Debian
+ Compiler: g++ >= 4.9
+ Software/Packages: Anaconda >= 4.5.1, Matlab

Download the data from http://corpus-texmex.irisa.fr, and construct a kNN-graph.
And organize the files as follow:

```
.
├── gist
│   ├── gist_100NN_100.graph
│   ├── gist_base.fvecs
│   ├── gist_groundtruth.ivecs
│   ├── gist_learn.fvecs
│   └── gist_query.fvecs
├── sift
│   ├── sift_100NN_100.graph
│   ├── sift_base.fvecs
│   ├── sift_groundtruth.ivecs
│   ├── sift_learn.fvecs
│   └── sift_query.fvecs
```

To use OPQ encoding of python version, you need faiss. Run
```Bash
conda install faiss-cpu -c pytorch
```
Some of C++ code (almost not) need Eigen. Run
```Bash
weget https://gitlab.com/libeigen/eigen/-/archive/3.3.7/eigen-3.3.7.tar.gz
tar xzvf eigen-3.3.7.tar.gz
sudo cp -r eigen-3.3.7/Eigen /usr/local/include/Eigen
```
Then you can use `#include <Eigen>` in C++ code.

## Get Started

### Graph-based Searh

You can find all compile instructions and program parameter format in scripts
`gnns.sh` and `gnns-opq.sh`. 
Running scripts can conduct the experiments automatically.
You can also compile the code and execute it manually as you want.

### OPQ encoding

You can find programs in folder `opq_encode` to apply OPQ encoding process:
+ `matlab_OPQ_release_v1.1/encode.m` matlab version (recommended)
+ `encode.py` faiss version
+ `re-impl/` C++ version (unfinished)

After you finish encoding, run `gnns-opq.sh` to test graph-based ANNS with OPQ encoding.
You can also download my encoding files from https://pan.baidu.com/s/1DwEVBljrwY6krwnPSqynFA ,password: 6z5e.

## TODO

- [ ] re-implement OPQ encoding in C++
- [ ] re-imlement NSG
- [ ] conduct more experiments about graph-based ANNS with OPQ encoding