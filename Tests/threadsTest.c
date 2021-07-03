#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include "../headers/threads.h"
#include <stdlib.h>
#include <stdio.h>

cluster_init_cent_t *buffer;
int64_t len, *last, *first;
cluster_t *c1,*c2,*c3,*c4,clust1[2],clust2[2];
point_t icent1[2], icent2[2];
point_t points1[2], points2[2], points3[3], points4[4], *p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8;
int64_t coord1[2],coord2[2], coord3[2], coord4[2], coord5[2],coord6[2], coord7[2], coord8[2];

int setup(void){
    len = 3;
    last = (int64_t *)malloc(sizeof(int64_t));
    if (last == NULL) fprintf(stderr, "malloc failed (allocating last)");
    first = (int64_t *)malloc(sizeof(int64_t));
    if (first == NULL) fprintf(stderr, "malloc failed (allocating first)");
    *last = 0;
    *first = 0;
    buffer = (cluster_init_cent_t *) malloc(sizeof(cluster_init_cent_t) * len);
    if (buffer == NULL) fprintf(stderr, "malloc failed (allocating buffer)");
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
    p7 = (point_t*)malloc(sizeof(point_t));
    if (p7 == NULL) fprintf(stderr, "malloc failed (allocating point 7)");
    p8 = (point_t*)malloc(sizeof(point_t));
    if (p8 == NULL) fprintf(stderr, "malloc failed (allocating point 8)");
    c1 = (cluster_t*)malloc(sizeof(cluster_t));
    if (c1 == NULL) fprintf(stderr, "malloc failed (allocating cluster 1)");
    c2 = (cluster_t*)malloc(sizeof(cluster_t));
    if (c2 == NULL) fprintf(stderr, "malloc failed (allocating cluster 2)");
    c3 = (cluster_t*)malloc(sizeof(cluster_t));
    if (c3 == NULL) fprintf(stderr, "malloc failed (allocating cluster 3)");
    c4 = (cluster_t*)malloc(sizeof(cluster_t));
    if (c4 == NULL) fprintf(stderr, "malloc failed (allocating cluster 4)");
    coord1[0] = 1;coord1[1] = 1;
    coord2[0] = 2;coord2[1] = 2;
    coord3[0] = 3;coord3[1] = 3;
    coord4[0] = 4;coord4[1] = 4;
    coord5[0] = 5;coord5[1] = 5;
    coord6[0] = 6;coord6[1] = 6;
    coord7[0] = 7;coord7[1] = 7;
    coord8[0] = 8;coord8[1] = 8;
    p1->coord = coord1;p2->coord = coord2;
    p3->coord = coord3;p4->coord = coord4;
    p5->coord = coord5;p6->coord = coord6;
    p7->coord = coord7;p8->coord = coord8;
    p1->dimension=2;p2->dimension=2;p3->dimension=2;p4->dimension=2;
    p5->dimension=2;p6->dimension=2;p7->dimension=2;p8->dimension=2;
    points1[0]=*p1;points1[1]=*p2;
    points2[0]=*p3;points2[1]=*p4;
    points3[0]=*p5;points3[1]=*p6;
    points4[0]=*p7;points4[1]=*p8;
    icent1[0] = *p1;icent1[1] = *p2;
    icent2[0] = *p5;icent2[1] = *p6;
    c1->centroid = *p3;c2->centroid = *p4;
    c3->centroid = *p7;c4->centroid = *p8;
    c1->points = points1;c2->points = points2;
    c3->points = points3;c4->points = points4;
    c1->size = 2;c2->size = 2;c3->size = 2;c4->size = 2;
    clust1[0] = *c1;clust1[1] = *c2;
    clust2[0] = *c3;clust2[1] = *c4;
    return 0;
}

int teardown(void){
    free(p1);free(p2);free(p3);free(p4);free(p5);free(p6);free(p7);free(p8);
    free(c1);free(c2);free(c3);free(c4);
    free(first);free(last);
    free(buffer);

    return 0;
}

void test_push_and_get(void){
    push(buffer,len,last,clust1,icent1);
    for (uint64_t i = 0;i<2;i++){
        for (uint64_t j = 0;j<2;j++){
            CU_ASSERT_EQUAL(buffer[0].clusters[0].points[i].coord[j],points1[i].coord[j]);
            CU_ASSERT_EQUAL(buffer[0].init_centroids[i].coord[j],icent1[i].coord[j]);

        }
    }
    for (uint64_t i = 0;i<2;i++){
        for (uint64_t j = 0;j<2;j++){
            CU_ASSERT_EQUAL(buffer[0].clusters[1].points[i].coord[j],points2[i].coord[j]);
        }
    }

    push(buffer,len,last,clust2,icent2);
    for (uint64_t i = 0;i<2;i++){
        for (uint64_t j = 0;j<2;j++){
            CU_ASSERT_EQUAL(buffer[1].clusters[0].points[i].coord[j],points3[i].coord[j]);
            CU_ASSERT_EQUAL(buffer[1].init_centroids[i].coord[j],icent2[i].coord[j]);

        }
    }
    for (uint64_t i = 0;i<2;i++){
        for (uint64_t j = 0;j<2;j++){
            CU_ASSERT_EQUAL(buffer[1].clusters[1].points[i].coord[j],points4[i].coord[j]);
        }
    }
    cluster_init_cent_t result = get(buffer,len,first);
    for (uint64_t i = 0;i<2;i++){
        for (uint64_t j = 0;j<2;j++){
            CU_ASSERT_EQUAL(result.clusters[0].points[i].coord[j],points1[i].coord[j]);
            CU_ASSERT_EQUAL(result.init_centroids[i].coord[j],icent1[i].coord[j]);

        }
    }
    for (uint64_t i = 0;i<2;i++){
        for (uint64_t j = 0;j<2;j++){
            CU_ASSERT_EQUAL(result.clusters[1].points[i].coord[j],points2[i].coord[j]);
        }
    }
}


int main(){

    if (CUE_SUCCESS != CU_initialize_registry()){
        return CU_get_error();
    }


    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("threads_suite",setup,teardown);
    if(pSuite == NULL){
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite,"test_push_and_get",test_push_and_get))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_run_tests();
    CU_basic_show_failures(CU_get_failure_list());

    CU_cleanup_registry();
    return CU_get_error();


}
