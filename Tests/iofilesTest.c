#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include <stdlib.h>
#include <stdio.h>
#include "../headers/iofiles.h"


FILE *input, *solution, *solution_quiet;
point_t *points;
uint32_t dimensions;
uint64_t nbr_points;
int64_t distortions[2];
int64_t *coordinates;
point_t *p1, *p2, *p3, *p4;
args_t *args1, *args2;
int64_t coord1[2],coord2[2], coord3[2], coord4[2];
cluster_t* clusters[2];
point_t* icent[2];
cluster_t *c1,*c2,*c3,*c4,clust1[2],clust2[2];
point_t icent1[2], icent2[2], cent1[2], cent2[2];

int setup(void){

    input = fopen("Tests/test_input.bin", "w");
    if (input == NULL) fprintf(stderr, "fopen failed (opening Tests/test_input.bin in Write mode)");
    uint32_t dim = htobe32(2);
    if (fwrite(&dim,sizeof(uint32_t),1,input) != 1) fprintf(stderr, "fwrite error");

    uint64_t nbr = 3;
    nbr = htobe64(nbr);
    if (fwrite(&nbr,sizeof(uint64_t),1,input) != 1) fprintf(stderr, "fwrite error");
    coordinates = (int64_t*)malloc(sizeof(int64_t)*6);
    for (uint64_t i = 0;i<6;i++){
        int64_t coordinate = i;
        coordinate = htobe64(coordinate);
        coordinates[i] = coordinate;
        if (fwrite(&(coordinates[i]),sizeof(int64_t),1,input) != 1) fprintf(stderr, "fwrite error");
    }
    if (fclose(input) != 0) fprintf(stderr, "fclose failed");
    input = fopen("Tests/test_input.bin","r");
    if (input == NULL) fprintf(stderr, "fopen failed (opening Tests/test_input.bin in Read mode)");
    args1 = (args_t*)malloc(sizeof(args_t));
    if (args1 == NULL) fprintf(stderr, "malloc failed (allocating args1)");
    args1->quiet = false;
    args1->output_stream = fopen("Tests/test_output.csv","w");
    if (args1->output_stream == NULL) fprintf(stderr, "fopen failed (opening Tests/test_output.csv in Write mode)");
    args1->k = 2;
    args2 = (args_t*)malloc(sizeof(args_t));
    if (args2 == NULL) fprintf(stderr, "malloc failed (allocating args2)");
    args2->quiet = true;
    args2->output_stream = fopen("Tests/test_output_quiet.csv","w");
    if (args2->output_stream == NULL) fprintf(stderr, "fopen failed (opening Tests/test_output_quiet.csv in Write mode)");
    args2->k = 2;
    distortions[0] = 11;
    distortions[1] = 11;
    p1 = (point_t*)malloc(sizeof(point_t));
    if (p1 == NULL) fprintf(stderr, "malloc failed (allocating point 1)");
    p2 = (point_t*)malloc(sizeof(point_t));
    if (p2 == NULL) fprintf(stderr, "malloc failed (allocating point 2)");
    p3 = (point_t*)malloc(sizeof(point_t));
    if (p3 == NULL) fprintf(stderr, "malloc failed (allocating point 3)");
    p4 = (point_t*)malloc(sizeof(point_t));
    if (p4 == NULL) fprintf(stderr, "malloc failed (allocating point 4)");
    coord1[0] = 1;
    coord1[1] = 1;
    p1->coord = coord1;
    coord2[0] = 2;
    coord2[1] = 2;
    p2->coord = coord2;
    coord3[0] = 3;
    coord3[1] = 4;
    p3->coord = coord3;
    coord4[0] = 4;
    coord4[1] = 5;
    p4->coord = coord4;
    p1->dimension = 2;
    p2->dimension = 2;
    p3->dimension = 2;
    p4->dimension = 2;
    icent1[0] = *p1;
    icent1[1] = *p2;
    icent2[0] = *p1;
    icent2[1] = *p3;
    icent[0] = icent1;
    icent[1] = icent2;
    cent1[0] = *p1;
    cent1[1] = *p4;
    cent2[0] = *p1;
    cent2[1] = *p4;
    c1 = (cluster_t*)malloc(sizeof(cluster_t));
    if (c1 == NULL) fprintf(stderr, "malloc failed (allocating cluster 1)");
    c2 = (cluster_t*)malloc(sizeof(cluster_t));
    if (c2 == NULL) fprintf(stderr, "malloc failed (allocating cluster 2)");
    c3 = (cluster_t*)malloc(sizeof(cluster_t));
    if (c3 == NULL) fprintf(stderr, "malloc failed (allocating cluster 3)");
    c4 = (cluster_t*)malloc(sizeof(cluster_t));
    if (c4 == NULL) fprintf(stderr, "malloc failed (allocating cluster 4)");
    c1->points = icent1;
    c1->size = 2;
    c1->centroid = *p1;
    c2->points = icent2;
    c2->size = 2;
    c2->centroid = *p4;
    c3->points = cent1;
    c3->size = 2;
    c3->centroid = *p1;
    c4->points = cent2;
    c4->size = 2;
    c4->centroid = *p4;
    clust1[0] = *c1;
    clust1[1] = *c2;
    clust2[0] = *c3;
    clust2[1] = *c4;
    clusters[0] = clust1;
    clusters[1] = clust2;
    return 0;
}

int teardown(void){
    free(coordinates);
    free(p1);free(p2);free(p3);free(p4);
    free(c1);free(c2);free(c3);free(c4);
    for (int64_t i = 0; i < nbr_points; ++i) {
        free(points[i].coord);
    }
    free(points);
    if (fclose(input) != 0) fprintf(stderr, "fclose failed");
    if (fclose(solution) != 0) fprintf(stderr, "fclose failed");
    if (fclose(solution_quiet) != 0) fprintf(stderr, "fclose failed");
    if (fclose(args1->output_stream) != 0) fprintf(stderr, "fclose failed");
    free(args1);
    if (fclose(args2->output_stream) != 0) fprintf(stderr, "fclose failed");
    free(args2);
    return 0;
}

void test_parse_input(void){
    parse_input_file(input,&dimensions,&nbr_points,&points);
    CU_ASSERT_EQUAL(dimensions,2);
    CU_ASSERT_EQUAL(nbr_points,3);
    int64_t k = 0;
    for (uint64_t i = 0; i < 3;i++){
        for (uint64_t j = 0; j < 2;j++){
            CU_ASSERT_EQUAL(points[i].coord[j],be64toh(coordinates[k]));
            k++;
        }
    }
}

void test_write_output_csv(void){
    fprintf(args2->output_stream,"initialization centroids,distortion,centroids\n");
    fprintf(args1->output_stream,"initialization centroids,distortion,centroids,clusters\n");
    for (int64_t i=0;i<2;i++){
        write_line_csv(*args1, clusters[i], icent[i], distortions[i]);
        write_line_csv(*args2, clusters[i], icent[i], distortions[i]);
    }
    if (fclose(args1->output_stream) != 0) fprintf(stderr, "fclose failed");
    if (fclose(args2->output_stream) != 0) fprintf(stderr, "fclose failed");
    args1->output_stream = fopen("Tests/test_output.csv","r");
    if (args1->output_stream == NULL) fprintf(stderr, "fopen failed (opening Tests/test_output.csv in Read mode)");
    solution = fopen("Tests/output_solution.csv","r");
    if (solution == NULL) fprintf(stderr, "fopen failed (opening Tests/output_solution.csv in Read mode)");
    CU_ASSERT_EQUAL(compare_files(args1->output_stream,solution), 0);
    args2->output_stream = fopen("Tests/test_output_quiet.csv","r");
    if (args2->output_stream == NULL) fprintf(stderr, "fopen failed (opening Tests/test_output_quiet.csv in Read mode)");
    solution_quiet = fopen("Tests/output_quiet_solution.csv","r");
    if (solution_quiet == NULL) fprintf(stderr, "fopen failed (opening Tests/output_quiet_solution.csv in Read mode)");
    CU_ASSERT_EQUAL(compare_files(args2->output_stream,solution_quiet), 0);
}

int main(){

    if (CUE_SUCCESS != CU_initialize_registry()){
        return CU_get_error();
    }

    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("iofiles_suite",setup,teardown);
    if(pSuite == NULL){
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite,"test_parse_input",test_parse_input) ||
        NULL == CU_add_test(pSuite,"test_write_output_csv",test_write_output_csv))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_run_tests();
    CU_basic_show_failures(CU_get_failure_list());

    CU_cleanup_registry();
    return CU_get_error();
}
