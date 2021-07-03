#ifndef ARGS_H
#define ARGS_H
#include "distance.h"
#include <stdio.h>
#include <stdbool.h>


/**
 * structure that contains all the options passed to the kmeans executable
 */
typedef struct {
    FILE *input_stream;
    FILE *output_stream;
    int32_t n_threads;
    int32_t k;
    int32_t n_first_initialization_points;
    bool quiet;
    squared_distance_func_t squared_distance_func;
} args_t;

#endif //ARGS_H
