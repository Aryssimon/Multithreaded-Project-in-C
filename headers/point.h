#include <inttypes.h>

#ifndef POINT_H
#define POINT_H


/**
 * structure of a point with its coordinates and its dimension
 */
typedef struct {
    int64_t *coord;
    uint32_t dimension;
} point_t;

#endif //POINT_H
