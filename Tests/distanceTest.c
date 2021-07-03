#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include "../headers/distance.h"
#include <stdlib.h>
#include <stdio.h>


point_t *p1, *p2, *p3, *p4, *p5, *p6, *cent1, *cent2, *cent3, **centroids;
cluster_t *c1, *c2, *c3, *clusters;


int setup(void){
    p1 = (point_t*)malloc(sizeof(point_t));
    if (p1 == NULL) fprintf(stderr, "malloc failed (allocating point 1)");
    p2 = (point_t*)malloc(sizeof(point_t));
    if (p2 == NULL) fprintf(stderr, "malloc failed (allocating point 2)");
    p3 = (point_t*)malloc(sizeof(point_t));
    if (p3 == NULL) fprintf(stderr, "malloc failed (allocating point 3)");
    p4 = (point_t*)malloc(sizeof(point_t));
    if (p4 == NULL) fprintf(stderr, "malloc failed (allocating point 4)");
    p5 = (point_t*)malloc(sizeof(point_t));
    if (p5 == NULL) fprintf(stderr, "malloc failed (allocating point 5)");
    p6 = (point_t*)malloc(sizeof(point_t));
    if (p6 == NULL) fprintf(stderr, "malloc failed (allocating point 6)");
    p1->dimension = 2;
    p2->dimension = 2;
    p3->dimension = 2;
    p4->dimension = 2;
    p5->dimension = 2;
    p6->dimension = 2;
    p1->coord = (int64_t*)malloc(sizeof(int64_t)*2);
    if (p1->coord == NULL) fprintf(stderr, "malloc failed (allocating coord of point 1)");
    p2->coord = (int64_t*)malloc(sizeof(int64_t)*2);
    if (p2->coord == NULL) fprintf(stderr, "malloc failed (allocating coord of point 2)");
    p3->coord = (int64_t*)malloc(sizeof(int64_t)*2);
    if (p3->coord == NULL) fprintf(stderr, "malloc failed (allocating coord of point 3)");
    p4->coord = (int64_t*)malloc(sizeof(int64_t)*2);
    if (p4->coord == NULL) fprintf(stderr, "malloc failed (allocating coord of point 4)");
    p5->coord = (int64_t*)malloc(sizeof(int64_t)*2);
    if (p5->coord == NULL) fprintf(stderr, "malloc failed (allocating coord of point 5)");
    p6->coord = (int64_t*)malloc(sizeof(int64_t)*2);
    if (p6->coord == NULL) fprintf(stderr, "malloc failed (allocating coord of point 6)");
    p1->coord[0] = 1;
    p1->coord[1] = 1;
    p2->coord[0] = -1;
    p2->coord[1] = -1;
    p3->coord[0] = 2;
    p3->coord[1] = 2;
    p4->coord[0] = 4;
    p4->coord[1] = 4;
    p5->coord[0] = 7;
    p5->coord[1] = 7;
    p6->coord[0] = 9;
    p6->coord[1] = 9;
    c1 = (cluster_t*)malloc(sizeof(cluster_t));
    if (c1 == NULL) fprintf(stderr, "malloc failed (allocating cluster 1)");
    c2 = (cluster_t*)malloc(sizeof(cluster_t));
    if (c2 == NULL) fprintf(stderr, "malloc failed (allocating cluster 2)");
    c3 = (cluster_t*)malloc(sizeof(cluster_t));
    if (c3 == NULL) fprintf(stderr, "malloc failed (allocating cluster 3)");
    c1->size = 2;
    c2->size = 2;
    c3->size = 2;
    c1->points = (point_t*)malloc(sizeof(point_t)*2);
    if (c1->points == NULL) fprintf(stderr, "malloc failed (allocating points array of cluster 1)");
    c2->points = (point_t*)malloc(sizeof(point_t)*2);
    if (c2->points == NULL) fprintf(stderr, "malloc failed (allocating points array of cluster 2)");
    c3->points = (point_t*)malloc(sizeof(point_t)*2);
    if (c3->points == NULL) fprintf(stderr, "malloc failed (allocating points array of cluster 3)");
    c1->points[0] = *p1;
    c1->points[1] = *p2;
    c2->points[0] = *p3;
    c2->points[1] = *p4;
    c3->points[0] = *p5;
    c3->points[1] = *p6;
    clusters = (cluster_t *) malloc(sizeof(cluster_t)*3);
    if (clusters == NULL) fprintf(stderr, "malloc failed (allocating clusters array)");
    cent1 = (point_t*)malloc(sizeof(point_t));
    if (cent1 == NULL) fprintf(stderr, "malloc failed (allocating centroid 1)");
    cent2 = (point_t*)malloc(sizeof(point_t));
    if (cent2 == NULL) fprintf(stderr, "malloc failed (allocating centroid 1)");
    cent3 = (point_t*)malloc(sizeof(point_t));
    if (cent3 == NULL) fprintf(stderr, "malloc failed (allocating centroid 1)");
    cent1->coord = (int64_t*)malloc(sizeof(int64_t)*2);
    if (cent1->coord == NULL) fprintf(stderr, "malloc failed (allocating coord of centroid 1)");
    cent2->coord = (int64_t*)malloc(sizeof(int64_t)*2);
    if (cent2->coord == NULL) fprintf(stderr, "malloc failed (allocating coord of centroid 2)");
    cent3->coord = (int64_t*)malloc(sizeof(int64_t)*2);
    if (cent3->coord == NULL) fprintf(stderr, "malloc failed (allocating coord of centroid 3)");
    cent1->coord[0] = 0;
    cent1->coord[1] = 0;
    cent2->coord[0] = 3;
    cent2->coord[1] = 3;
    cent3->coord[0] = 8;
    cent3->coord[1] = 8;
    cent1->dimension = 2;
    cent2->dimension = 2;
    cent3->dimension = 2;
    c1->centroid = *cent1;
    c2->centroid = *cent2;
    c3->centroid = *cent3;
    clusters[0] = *c1;
    clusters[1] = *c2;
    clusters[2] = *c3;
    return 0;
}
int teardown(void){
    free(p1->coord);free(p2->coord);free(p3->coord);free(p4->coord);free(p5->coord);free(p6->coord);
    free(p1);free(p2);free(p3);free(p4);free(p5);free(p6);
    free(c1->points);free(c2->points);free(c3->points);
    free(c1);free(c2);free(c3);
    free(cent1->coord);free(cent2->coord);free(cent3->coord);
    free(cent1);free(cent2);free(cent3);
    free(clusters);
    return 0;
}

void test_manhattan(void){
    CU_ASSERT_EQUAL(squared_manhattan_distance(p1,p2),16);
    CU_ASSERT_EQUAL(squared_manhattan_distance(p1,p3),4);
    CU_ASSERT_EQUAL(squared_manhattan_distance(p2,p3),36);
    CU_ASSERT_EQUAL(squared_manhattan_distance(p1,p1),0);
}

void test_euclidean(void){
    CU_ASSERT_EQUAL(squared_euclidean_distance(p1,p2),8);
    CU_ASSERT_EQUAL(squared_euclidean_distance(p1,p3),2);
    CU_ASSERT_EQUAL(squared_euclidean_distance(p2,p3),18);
    CU_ASSERT_EQUAL(squared_euclidean_distance(p1,p1),0);
}

void test_distortion(void){
    CU_ASSERT_EQUAL(distortion(clusters,squared_manhattan_distance,3),24);
    CU_ASSERT_EQUAL(distortion(clusters,squared_euclidean_distance,3),12);
}

int main(){

    if (CUE_SUCCESS != CU_initialize_registry()){
        return CU_get_error();
    }


    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("distance_suite",setup,teardown);
    if(pSuite == NULL){
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(pSuite,"test_manhattan",test_manhattan)) ||
        (NULL == CU_add_test(pSuite,"test_euclidean",test_euclidean)) ||
        (NULL == CU_add_test(pSuite,"test_distortion",test_distortion)))

    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_run_tests();
    CU_basic_show_failures(CU_get_failure_list());

    CU_cleanup_registry();
    return CU_get_error();


}
