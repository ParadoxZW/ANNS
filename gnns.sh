# format:
# executable_file_name k poo_size path_to_database path_to_queries path_to_gt path_to_graph
DATASET=gist
f1=graph_search_stlset
f2=graph_search_avx
f3=graph_search_sse
f4=graph_search_mmap
f5=graph_search_fixheap
f6=graph_search_insertion
# echo run on $DATASET
# ./graph_search_v1 100 500 \
# ./$DATASET/$DATASET"_base.fvecs" \
# ./$DATASET/$DATASET"_query.fvecs" \
# ./$DATASET/$DATASET"_groundtruth.ivecs" \
# ./$DATASET"_100NN_100.graph" # 0.506
f=$f1
g++ -std=c++11 -o $f $f.cpp
for i in 100 200 400 600 800 1000
do
    echo ""
    echo run on $DATASET:$i:$f
    ./$f 100 $i \
    ./$DATASET/$DATASET"_base.fvecs" \
    ./$DATASET/$DATASET"_query.fvecs" \
    ./$DATASET/$DATASET"_groundtruth.ivecs" \
    ./$DATASET/$DATASET"_100NN_100.graph" # 0.344472
done

f=$f1
g++ -std=c++11 -O2 -o $f $f.cpp
for i in 100 200 400 600 800 1000
do
    echo ""
    echo run on $DATASET:$i:$f
    ./$f 100 $i \
    ./$DATASET/$DATASET"_base.fvecs" \
    ./$DATASET/$DATASET"_query.fvecs" \
    ./$DATASET/$DATASET"_groundtruth.ivecs" \
    ./$DATASET/$DATASET"_100NN_100.graph" # 0.344472
done


f=$f2
g++ -std=c++11 -O2 -mavx2 -o $f $f.cpp
for i in 100 200 400 600 800 1000
do
    echo ""
    echo run on $DATASET:$i:$f
    ./$f 100 $i \
    ./$DATASET/$DATASET"_base.fvecs" \
    ./$DATASET/$DATASET"_query.fvecs" \
    ./$DATASET/$DATASET"_groundtruth.ivecs" \
    ./$DATASET/$DATASET"_100NN_100.graph" # 0.344472
done

f=$f3
g++ -std=c++11 -O2 -msse4.2 -o $f $f.cpp
for i in 100 200 400 600 800 1000
do
    echo ""
    echo run on $DATASET:$i:$f
    ./$f 100 $i \
    ./$DATASET/$DATASET"_base.fvecs" \
    ./$DATASET/$DATASET"_query.fvecs" \
    ./$DATASET/$DATASET"_groundtruth.ivecs" \
    ./$DATASET/$DATASET"_100NN_100.graph" # 0.344472
done

f=$f4
g++ -std=c++11 -O2 -o $f $f.cpp
for i in 100 200 400 600 800 1000
do
    echo ""
    echo run on $DATASET:$i:$f
    ./$f 100 $i \
    ./$DATASET/$DATASET"_base.fvecs" \
    ./$DATASET/$DATASET"_query.fvecs" \
    ./$DATASET/$DATASET"_groundtruth.ivecs" \
    ./$DATASET/$DATASET"_100NN_100.graph" # 0.344472
done

f=$f5
g++ -std=c++11 -O2 -o $f $f.cpp
for i in 100 200 400 600 800 1000
do
    echo ""
    echo run on $DATASET:$i:$f
    ./$f 100 $i \
    ./$DATASET/$DATASET"_base.fvecs" \
    ./$DATASET/$DATASET"_query.fvecs" \
    ./$DATASET/$DATASET"_groundtruth.ivecs" \
    ./$DATASET/$DATASET"_100NN_100.graph" # 0.344472
done

f=$f6
g++ -std=c++11 -O2 -o $f $f.cpp
for i in 100 200 400 600 800 1000
do
    echo ""
    echo run on $DATASET:$i:$f
    ./$f 100 $i \
    ./$DATASET/$DATASET"_base.fvecs" \
    ./$DATASET/$DATASET"_query.fvecs" \
    ./$DATASET/$DATASET"_groundtruth.ivecs" \
    ./$DATASET/$DATASET"_100NN_100.graph" # 0.344472
done