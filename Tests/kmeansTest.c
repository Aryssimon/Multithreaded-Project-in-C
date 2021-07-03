#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include "../headers/distance.h"
#include <stdlib.h>
#include <stdio.h>
#include "../headers/kmeans.h"

point_t *p1, *p2, *p3, *p4, *p5, *p6, *p7, *cent1, *cent2, *test_centroids;
cluster_t *test_clusters;

int setup(void) {
    // initialize points
    p1 = (point_t *) malloc(sizeof(point_t));
    if (p1 == NULL) fprintf(stderr, "malloc failed (allocating point 1)");
    p2 = (point_t *) malloc(sizeof(point_t));
    if (p2 == NULL) fprintf(stderr, "malloc failed (allocating point 2)");
    p3 = (point_t *) malloc(sizeof(point_t));
    if (p3 == NULL) fprintf(stderr, "malloc failed (allocating point 3)");
    p4 = (point_t *) malloc(sizeof(point_t));
    if (p4 == NULL) fprintf(stderr, "malloc failed (allocating point 4)");
    p5 = (point_t *) malloc(sizeof(point_t));
    if (p5 == NULL) fprintf(stderr, "malloc failed (allocating point 5)");
    p6 = (point_t *) malloc(sizeof(point_t));
    if (p6 == NULL) fprintf(stderr, "malloc failed (allocating point 6)");
    p7 = (point_t *) malloc(sizeof(point_t));
    if (p7 == NULL) fprintf(stderr, "malloc failed (allocating point 7)");
    p1->dimension = 2;
    p2->dimension = 2;
    p3->dimension = 2;
    p4->dimension = 2;
    p5->dimension = 2;
    p6->dimension = 2;
    p7->dimension = 2;
    p1->coord = (int64_t *) malloc(sizeof(int64_t) * 2);
    if (p1->coord == NULL) fprintf(stderr, "malloc failed (allocating p1->coord)");
    p2->coord = (int64_t *) malloc(sizeof(int64_t) * 2);
    if (p2->coord == NULL) fprintf(stderr, "malloc failed (allocating p2->coord)");
    p3->coord = (int64_t *) malloc(sizeof(int64_t) * 2);
    if (p3->coord == NULL) fprintf(stderr, "malloc failed (allocating p3->coord)");
    p4->coord = (int64_t *) malloc(sizeof(int64_t) * 2);
    if (p4->coord == NULL) fprintf(stderr, "malloc failed (allocating p4->coord)");
    p5->coord = (int64_t *) malloc(sizeof(int64_t) * 2);
    if (p5->coord == NULL) fprintf(stderr, "malloc failed (allocating p5->coord)");
    p6->coord = (int64_t *) malloc(sizeof(int64_t) * 2);
    if (p6->coord == NULL) fprintf(stderr, "malloc failed (allocating p6->coord)");
    p7->coord = (int64_t *) malloc(sizeof(int64_t) * 2);
    if (p7->coord == NULL) fprintf(stderr, "malloc failed (allocating p7->coord)");
    p1->coord[0] = 1;
    p1->coord[1] = 1;
    p2->coord[0] = 2;
    p2->coord[1] = 2;
    p3->coord[0] = 3;
    p3->coord[1] = 4;
    p4->coord[0] = 5;
    p4->coord[1] = 7;
    p5->coord[0] = 3;
    p5->coord[1] = 5;
    p6->coord[0] = 5;
    p6->coord[1] = 5;
    p7->coord[0] = 4;
    p7->coord[1] = 5;


    // initialize centroids
    cent1 = (point_t *) malloc(sizeof(point_t));
    if (cent1 == NULL) fprintf(stderr, "malloc failed (allocating cent1)");
    cent2 = (point_t *) malloc(sizeof(point_t));
    if (cent2 == NULL) fprintf(stderr, "malloc failed (allocating cent2)");
    cent1->coord = (int64_t *) malloc(sizeof(int64_t) * 2);
    if (cent1->coord == NULL) fprintf(stderr, "malloc failed (allocating cent1->coord)");
    cent2->coord = (int64_t *) malloc(sizeof(int64_t) * 2);
    if (cent2->coord == NULL) fprintf(stderr, "malloc failed (allocating cent2->coord)");
    cent1->coord[0] = 5;
    cent1->coord[1] = 7;
    cent2->coord[0] = 5;
    cent2->coord[1] = 5;
    cent1->dimension = 2;
    cent2->dimension = 2;
    return 0;
}

int teardown(void) {
    free(p1->coord);free(p2->coord);free(p3->coord);free(p4->coord);free(p5->coord);free(p6->coord);free(p7->coord);
    free(p1);free(p2);free(p3);free(p4);free(p5);free(p6);free(p7);
    free(cent1->coord);
    free(cent2->coord);
    free(cent1);
    free(cent2);
    return 0;
}

void test_compare_centroids(void) {
    point_t *centroids_array = (point_t *) malloc(sizeof(point_t) * 2);
    if (centroids_array == NULL) fprintf(stderr, "malloc failed (allocating centroids_array)");
    centroids_array[0] = *cent1;
    centroids_array[1] = *cent2;

    point_t *inverse_centroids_array = (point_t *) malloc(sizeof(point_t) * 2);
    if (inverse_centroids_array == NULL) fprintf(stderr, "malloc failed (allocating inverse_centroids_array)");
    inverse_centroids_array[0] = *cent2;
    inverse_centroids_array[1] = *cent1;


    point_t *centroid1 = (point_t *) malloc(sizeof(point_t));
    if (centroid1 == NULL) fprintf(stderr, "malloc failed (allocating centroid1)");
    point_t *centroid2 = (point_t *) malloc(sizeof(point_t));
    if (centroid2 == NULL) fprintf(stderr, "malloc failed (allocating centroid2)");
    centroid1->dimension = cent1->dimension;
    centroid2->dimension = cent2->dimension;
    centroid1->coord = (int64_t *) malloc(sizeof(int64_t) * centroid1->dimension);
    if (centroid1->coord == NULL) fprintf(stderr, "malloc failed (allocating centroid1->coord)");
    centroid2->coord = (int64_t *) malloc(sizeof(int64_t) * centroid2->dimension);
    if (centroid2->coord == NULL) fprintf(stderr, "malloc failed (allocating centroid2->coord)");
    memcpy(centroid1->coord, cent1->coord, sizeof(int64_t) * 2);
    memcpy(centroid2->coord, cent2->coord, sizeof(int64_t) * 2);

    cluster_t *clusters = (cluster_t *) malloc(sizeof(cluster_t) * 2);
    cluster_t *clusters_inverse = (cluster_t *) malloc(sizeof(cluster_t) * 2);
    clusters[0].centroid = *centroid1;
    clusters[1].centroid = *centroid2;
    clusters_inverse[0].centroid = *centroid2;
    clusters_inverse[1].centroid = *centroid1;

    CU_ASSERT_EQUAL(compare_centroids(centroids_array, clusters, 2), 1);
    CU_ASSERT_EQUAL(compare_centroids(inverse_centroids_array, clusters_inverse, 2), 1);
    CU_ASSERT_EQUAL(compare_centroids(centroids_array, clusters_inverse, 2), 0);
    CU_ASSERT_EQUAL(compare_centroids(inverse_centroids_array, clusters, 2), 0);


    free(centroids_array);
    free(inverse_centroids_array);
    free(centroid1->coord);
    free(centroid2->coord);
    free(centroid1);
    free(centroid2);
    free(clusters);
    free(clusters_inverse);
}

void test_assign_vectors_to_centroids(void) {
    test_clusters = (cluster_t *) malloc(sizeof(cluster_t) * 2);
    if (test_clusters == NULL) fprintf(stderr, "malloc failed (allocating test_clusters)");
    test_clusters[0].size = 7;
    test_clusters[0].points = (point_t *) malloc(sizeof(point_t) * 7);
    if (test_clusters[0].points == NULL) fprintf(stderr, "malloc failed (allocating test_clusters[0].points)");
    test_clusters[0].points[0] = *p1;
    test_clusters[0].points[1] = *p2;
    test_clusters[0].points[2] = *p3;
    test_clusters[0].points[3] = *p4;
    test_clusters[0].points[4] = *p5;
    test_clusters[0].points[5] = *p6;
    test_clusters[0].points[6] = *p7;
    test_clusters[0].centroid = *cent1;
    test_clusters[1].points = (point_t *) malloc(sizeof(point_t) * 7);
    if (test_clusters[1].points == NULL) fprintf(stderr, "malloc failed (allocating test_clusters[1].points)");
    test_clusters[1].size = 0;
    test_clusters[1].centroid = *cent2;

    assign_vectors_to_centroids(test_clusters, 2, 7, squared_euclidean_distance);

    CU_ASSERT_EQUAL(test_clusters[0].size, 1);
    CU_ASSERT_EQUAL(test_clusters[1].size, 6);

    CU_ASSERT_EQUAL(test_clusters[0].points[0].coord[0], 5);
    CU_ASSERT_EQUAL(test_clusters[0].points[0].coord[1], 7);

    CU_ASSERT_EQUAL(test_clusters[1].points[0].coord[0], 1);
    CU_ASSERT_EQUAL(test_clusters[1].points[0].coord[1], 1);

    CU_ASSERT_EQUAL(test_clusters[1].points[1].coord[0], 2);
    CU_ASSERT_EQUAL(test_clusters[1].points[1].coord[1], 2);

    CU_ASSERT_EQUAL(test_clusters[1].points[2].coord[0], 3);
    CU_ASSERT_EQUAL(test_clusters[1].points[2].coord[1], 4);

    CU_ASSERT_EQUAL(test_clusters[1].points[3].coord[0], 3);
    CU_ASSERT_EQUAL(test_clusters[1].points[3].coord[1], 5);

    CU_ASSERT_EQUAL(test_clusters[1].points[4].coord[0], 5);
    CU_ASSERT_EQUAL(test_clusters[1].points[4].coord[1], 5);

    CU_ASSERT_EQUAL(test_clusters[1].points[5].coord[0], 4);
    CU_ASSERT_EQUAL(test_clusters[1].points[5].coord[1], 5);
}

void test_update_centroids(void) {

    update_centroids(test_clusters, 2, 2);

    CU_ASSERT_EQUAL(test_clusters[0].centroid.dimension, 2);
    CU_ASSERT_EQUAL(test_clusters[1].centroid.dimension, 2);

    CU_ASSERT_EQUAL(test_clusters[0].centroid.coord[0], 5);
    CU_ASSERT_EQUAL(test_clusters[0].centroid.coord[1], 7);

    CU_ASSERT_EQUAL(test_clusters[1].centroid.coord[0], 3);
    CU_ASSERT_EQUAL(test_clusters[1].centroid.coord[1], 3);

    free(test_clusters[0].points);
    free(test_clusters[1].points);
    free(test_clusters);
}

void test_kmeans(void) {
    point_t *points_array = (point_t *) malloc(sizeof(point_t) * 7);
    if (points_array == NULL) fprintf(stderr, "malloc failed (allocating points_array)");
    points_array[0] = *p1;
    points_array[1] = *p2;
    points_array[2] = *p3;
    points_array[3] = *p4;
    points_array[4] = *p5;
    points_array[5] = *p6;
    points_array[6] = *p7;

    point_t *init_centroids = (point_t *) malloc(sizeof(point_t) * 2);
    init_centroids[0] = *cent1;
    init_centroids[1] = *cent2;

    cluster_t *result_manhattan = k_means(points_array, 7, 2, init_centroids, squared_manhattan_distance);

    CU_ASSERT_EQUAL(result_manhattan[0].size, 5);
    CU_ASSERT_EQUAL(result_manhattan[1].size, 2);


    CU_ASSERT_EQUAL(result_manhattan[0].centroid.coord[0], 4);
    CU_ASSERT_EQUAL(result_manhattan[0].centroid.coord[1], 5);
    CU_ASSERT_EQUAL(result_manhattan[1].centroid.coord[0], 1);
    CU_ASSERT_EQUAL(result_manhattan[1].centroid.coord[1], 1);

    CU_ASSERT_EQUAL(result_manhattan[0].points[0].coord[0], 5);
    CU_ASSERT_EQUAL(result_manhattan[0].points[0].coord[1], 7);
    CU_ASSERT_EQUAL(result_manhattan[0].points[1].coord[0], 5);
    CU_ASSERT_EQUAL(result_manhattan[0].points[1].coord[1], 5);
    CU_ASSERT_EQUAL(result_manhattan[0].points[2].coord[0], 4);
    CU_ASSERT_EQUAL(result_manhattan[0].points[2].coord[1], 5);
    CU_ASSERT_EQUAL(result_manhattan[0].points[3].coord[0], 3);
    CU_ASSERT_EQUAL(result_manhattan[0].points[3].coord[1], 4);
    CU_ASSERT_EQUAL(result_manhattan[0].points[4].coord[0], 3);
    CU_ASSERT_EQUAL(result_manhattan[0].points[4].coord[1], 5);

    CU_ASSERT_EQUAL(result_manhattan[1].points[0].coord[0], 1);
    CU_ASSERT_EQUAL(result_manhattan[1].points[0].coord[1], 1);
    CU_ASSERT_EQUAL(result_manhattan[1].points[1].coord[0], 2);
    CU_ASSERT_EQUAL(result_manhattan[1].points[1].coord[1], 2);

    free(points_array);
    free(init_centroids);
    free(result_manhattan[0].points);
    free(result_manhattan[1].points);
    free(result_manhattan[0].centroid.coord);
    free(result_manhattan[1].centroid.coord);
    free(result_manhattan);
}

int main() {

    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }


    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("kmeans_suite", setup, teardown);
    if (pSuite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(pSuite, "test_compare_centroids", test_compare_centroids)) ||
        (NULL == CU_add_test(pSuite, "test_assign_vectors_to_centroids", test_assign_vectors_to_centroids)) ||
        (NULL == CU_add_test(pSuite, "test_update_centroids", test_update_centroids)) ||
        (NULL == CU_add_test(pSuite, "test_kmeans", test_kmeans))) {
        CU_cleanup_registry();
        return CU_get_error();
    }



    CU_basic_run_tests();
    CU_basic_show_failures(CU_get_failure_list());

    CU_cleanup_registry();
    return CU_get_error();


}
