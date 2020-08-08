# format:
# executable_file_name k poo_size Msub path_to_queries path_to_gt path_to_graph
DATASET=sift
f=graph_search_opq

g++ -std=c++11 -O2 -o $f $f.cpp
for i in 100 200 400 600 800 1000
do
    echo run on $DATASET:$i:$f
    ./$f 100 $i 16 \
    ./$DATASET/$DATASET"_query.fvecs" \
    ./$DATASET/$DATASET"_groundtruth.ivecs" \
    ./$DATASET/$DATASET"_100NN_100.graph" # 0.427419
done
