#ifndef KMEANS_H
#define KMEANS_H
#include "../headers/distance.h"
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

cluster_t *k_means(point_t *points, uint64_t nb_of_points, int64_t nb_of_clusters, point_t *initial_centroids,
                   int64_t (*distance)(const point_t *, const point_t *));

void update_centroids(cluster_t *clusters, int64_t nb_of_clusters, uint32_t dimension);

void assign_vectors_to_centroids(cluster_t *clusters, int64_t nb_of_clusters, uint64_t nb_of_points,
                                 int64_t (*distance)(const point_t *, const point_t *));

int8_t compare_centroids(point_t *oldcentroids, cluster_t *clusters, int64_t nb_of_centroids);

#endif //KMEANS_H
