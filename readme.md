# ANNS

## Introduction

It's a project to re-implement the C++ program of graph-based ANNS and explore all kinds of
variants to improve the performance of the algorithm.

## Requirements & Preparations

+ OS: Ubuntu/Debian
+ Compiler: g++ >= 4.9

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

## Get Started

### Graph-based Searh

You can find all compile instructions and program parameter format in scripts
`gnns.sh` and `gnns-opq.sh`. 
Running scripts can conduct the experiments automatically.
You can also compile the code and execute it manually as you want.

### OPQ encoding

You can find programs to apply OPQ encoding process:
+ `matlab_OPQ_release_v1.1/encode.m` matlab version (recommended)
+ `encode.py` faiss version
+ `re-impl/` C++ version (unfinished)

## TODO

- [ ] OPQ re-implement