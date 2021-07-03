#include "../headers/kmeans.h"
#define true 1
#define false 0

/**
 * Compare the two set of centroids in parameter.
 * @param:  oldcentroids, pointer to the old centroids array
 *          clusters, pointer to current clusters array
 *          nb_of_centroids, size of the centroid arrays
 * @return: 1 if they are the same, 0 otherwise
 */
int8_t compare_centroids(point_t *oldcentroids, cluster_t *clusters, int64_t nb_of_centroids) {
    if ((oldcentroids == NULL && clusters != NULL) || (oldcentroids != NULL && clusters == NULL)) {
        return false;
    }
    for (int64_t i = 0; i < nb_of_centroids; i++) {
        for (int32_t j = 0; j < oldcentroids[i].dimension; j++) {
            if (oldcentroids[i].coord[j] != clusters[i].centroid.coord[j]) {
                return false;
            }
        }
    }
    return true;
}


/**
 * Assign points to clusters.
 * @param:  clusters, the set of clusters
 *          nb_of_clusters, the number of clusters to determine
 *          nb_of_points, the of total points
 *          distance, the function computing the distance between two points
 */
void assign_vectors_to_centroids(cluster_t *clusters, int64_t nb_of_clusters, uint64_t nb_of_points,
                                 int64_t (*distance)(const point_t *, const point_t *)) {
    cluster_t new_clusters[nb_of_clusters];

    // allocate the memory and set size to 0 for each cluster.
    for (int64_t i = 0; i < nb_of_clusters; i++) {
        new_clusters[i].points = (point_t *) malloc(sizeof(point_t) * nb_of_points);
        if (new_clusters[i].points == NULL) fprintf(stderr, "malloc failed (allocating new points array)");
        new_clusters[i].size = 0;
    }

    // algorithm to find the closest centroid of a point.
    for (int64_t i = 0; i < nb_of_clusters; i++) {
        for (int64_t j = 0; j < clusters[i].size; j++) {
            int64_t closest_centroid_id = -1;
            int64_t closest_centroid_distance = INT64_MAX;
            for (int64_t k = 0; k < nb_of_clusters; k++) {
                int64_t dist = distance(&((clusters[i]).points[j]), &(clusters[k].centroid));
                if (dist < closest_centroid_distance) {
                    closest_centroid_id = k;
                    closest_centroid_distance = dist;
                }
            }
            new_clusters[closest_centroid_id].points[new_clusters[closest_centroid_id].size] = clusters[i].points[j];
            new_clusters[closest_centroid_id].size += 1;
        }
    }

    // copy new_clusters to clusters.
    for (int64_t i = 0; i < nb_of_clusters; i++) {
        for (int64_t j = 0; j < new_clusters[i].size; j++) {
            clusters[i].points[j] = new_clusters[i].points[j];
        }
        clusters[i].size = new_clusters[i].size;
        free(new_clusters[i].points);
    }
}


/**
 * Compute the new centroids from the current clusters.
 * @param:  clusters, the set of clusters
 *          nb_of_clusters, the number of clusters
 *          nb_of_points, the number of points
 *          dimension, the dimension of a point
 */
void update_centroids(cluster_t *clusters, int64_t nb_of_clusters, uint32_t dimension) {
    int64_t vector_sum[dimension];
    for (int64_t i = 0; i < nb_of_clusters; i++) {
        for (uint32_t m = 0; m < dimension; m++) {
            vector_sum[m] = 0;
        }
        for (int64_t j = 0; j < clusters[i].size; j++) {
            for (uint32_t k = 0; k < dimension; k++) {
                vector_sum[k] += clusters[i].points[j].coord[k];
            }
        }
        for (uint32_t m = 0; m < dimension; m++) {
            if (clusters[i].size != 0){
                vector_sum[m] /= clusters[i].size;
            }
        }
        memcpy(clusters[i].centroid.coord, vector_sum, sizeof(int64_t) * dimension);
    }
}


/**
 * Algorithm to solve k-means problem.
 * @param:  points, the set of points
 *          nb_of_points, the number of points (size of the set)
 *          nb_of_clusters, the number of clusters to determine
 *          initial_centroids, the set of initial centroids
 *          distance, the function computing the distance between two points
 * @return:   A list of the final clusters containing the final centroids
 */
cluster_t *k_means(point_t *points, uint64_t nb_of_points, int64_t nb_of_clusters, point_t *initial_centroids,
                   int64_t (*distance)(const point_t *, const point_t *)) {
    uint32_t dimension = points[0].dimension;

    point_t *oldcentroids = (point_t *) malloc(sizeof(point_t) * nb_of_clusters);
    if (oldcentroids == NULL) fprintf(stderr, "malloc failed (allocating oldcentroids)");
    for (int64_t i = 0; i < nb_of_clusters; ++i) {
        oldcentroids[i].dimension = dimension;
        oldcentroids[i].coord = (int64_t *) malloc(sizeof(int64_t) * dimension);
        if (oldcentroids[i].coord == NULL) fprintf(stderr, "malloc failed (allocating coord in oldcentroids)");
    }

    cluster_t *clusters = (cluster_t *) malloc(sizeof(cluster_t) * nb_of_clusters);
    if (clusters == NULL) fprintf(stderr, "malloc failed (allocating clusters)");

    for (int64_t i = 0; i < nb_of_clusters; i++) {
        clusters[i].points = (point_t *) malloc(sizeof(point_t) * nb_of_points);
        if (clusters[i].points == NULL) fprintf(stderr, "malloc failed (allocating points in clusters)");
        clusters[i].size = 0;
        clusters[i].centroid.dimension = dimension;
        clusters[i].centroid.coord = (int64_t *) malloc(sizeof(int64_t) * dimension);
        if (clusters[i].centroid.coord == NULL) fprintf(stderr, "malloc failed (allocating coord in centroid)");
        memcpy(clusters[i].centroid.coord, initial_centroids[i].coord, sizeof(int64_t) * dimension);

    }
    memcpy(clusters[0].points, points, sizeof(point_t) * nb_of_points);
    clusters[0].size = nb_of_points;

    do {
        for (int64_t i = 0; i < nb_of_clusters; ++i) {
            memcpy(oldcentroids[i].coord, clusters[i].centroid.coord, sizeof(int64_t) * dimension);
        }
        assign_vectors_to_centroids(clusters, nb_of_clusters, nb_of_points, distance);
        update_centroids(clusters, nb_of_clusters, dimension);
    } while (compare_centroids(oldcentroids, clusters, nb_of_clusters) != 1);

    for (int64_t i = 0; i < nb_of_clusters; i++) {
        // Fit the points array to the right size that was previously over-allocated during computing.
        clusters[i].points = (point_t *) realloc(clusters[i].points, sizeof(point_t) * clusters[i].size);
        if (clusters[i].points == NULL) fprintf(stderr, "realloc failed (fitting cluster[i].points to the right size)");

        free(oldcentroids[i].coord);

    }
    free(oldcentroids);
    return clusters;
}



