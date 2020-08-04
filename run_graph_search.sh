# format:
# executable_file_name k poo_size path_to_database path_to_queries path_to_gt path_to_graph
DATASET=sift

# echo run on $DATASET
# ./graph_search_v1 100 500 \
# ./$DATASET/$DATASET"_base.fvecs" \
# ./$DATASET/$DATASET"_query.fvecs" \
# ./$DATASET/$DATASET"_groundtruth.ivecs" \
# ./$DATASET"_100NN_100.graph" # 0.506

echo run on $DATASET
./graph_search_v1 100 100 \
./$DATASET/$DATASET"_base.fvecs" \
./$DATASET/$DATASET"_query.fvecs" \
./$DATASET/$DATASET"_groundtruth.ivecs" \
./$DATASET/$DATASET"_100NN_100.graph" # 0.344472

echo run on $DATASET
./graph_search_v1 100 200 \
./$DATASET/$DATASET"_base.fvecs" \
./$DATASET/$DATASET"_query.fvecs" \
./$DATASET/$DATASET"_groundtruth.ivecs" \
./$DATASET/$DATASET"_100NN_100.graph" # 0.427419

echo run on $DATASET
./graph_search_v1 100 300 \
./$DATASET/$DATASET"_base.fvecs" \
./$DATASET/$DATASET"_query.fvecs" \
./$DATASET/$DATASET"_groundtruth.ivecs" \
./$DATASET/$DATASET"_100NN_100.graph" # 0.457692