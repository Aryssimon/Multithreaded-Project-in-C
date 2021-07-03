#include <stdlib.h>
#include "../headers/distance.h"

int64_t squared_euclidean_distance(const point_t *p1, const point_t *p2) {
    int64_t dist = 0;
    for (int32_t i = 0; i < p1->dimension; i++) {
        dist += ((p1->coord)[i] - (p2->coord)[i]) * ((p1->coord)[i] - (p2->coord)[i]);
    }
    return dist;
}


int64_t squared_manhattan_distance(const point_t *p1, const point_t *p2) {
    int64_t dist = 0;
    for (int32_t i = 0; i < p1->dimension; i++) {
        dist += labs((p1->coord)[i] - (p2->coord)[i]);
    }
    dist *= dist;
    return dist;
}

int64_t distortion(cluster_t *clusters, int64_t (*squared_dist)(const point_t *p1, const point_t *p2), int64_t n_clusters) {
    int64_t current_sum = 0;
    for (int64_t i = 0; i < n_clusters; i++) {
        for (int64_t j = 0; j < clusters[i].size; j++) {
            current_sum += squared_dist(&(clusters[i].points[j]), &(clusters[i].centroid));
        }
    }
    return current_sum;
}



