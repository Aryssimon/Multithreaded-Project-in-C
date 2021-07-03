#include <stdint.h>
#include "point.h"
#ifndef CLUSTER_H
#define CLUSTER_H


/**
 * structure of a cluster containing :
 * his points, his size and his centroid
 */
typedef struct {
    point_t *points;
    int64_t size;
    point_t centroid;
} cluster_t;


/**
 * structure containing a list of clusters with their initials centroids.
 * this structure basically contains the results of one iteration of k-means
 * for a combination of initial centroids.
 */
typedef struct {
    cluster_t *clusters;
    point_t *init_centroids;
} cluster_init_cent_t;

#endif //CLUSTER_H
