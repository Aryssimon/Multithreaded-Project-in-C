#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include "../headers/combination.h"
int64_t listofcombinations1[20], listofcombinations2[3], listofcombinations3[12], k1, k2, k3, counter1[1],listofint1[5],temp_comb1[4];
int64_t counter2[1],listofint2[3],temp_comb2[1], counter3[1],listofint3[4],temp_comb3[2];

int setup(void){
    for (int i=0;i<20;i++){
        listofcombinations1[i] = 0;
    }
    for (int i=0;i<3;i++){
        listofcombinations2[i] = 0;
    }
    for (int i=0;i<12;i++){
        listofcombinations3[i] = 0;
    }
    k1 = 4;
    k2 = 1;
    k3 = 2;
    counter1[0] = 0;
    counter2[0] = 0;
    counter3[0] = 0;
    for (int i=1;i<6;i++){
        listofint1[i-1] = i;
    }
    for (int i=1;i<4;i++){
        listofint2[i-1] = i;
    }
    for (int i=1;i<5;i++){
        listofint3[i-1] = i;
    }
    for (int i=0;i<4;i++){
        temp_comb1[i] = 0;
    }
    for (int i=0;i<1;i++){
        temp_comb2[i] = 0;
    }
    for (int i=0;i<2;i++){
        temp_comb3[i] = 0;
    }
    return 0;
}

int teardown(void){
    return 0;
}

void test_combination(void){
    int64_t correct_list1[20] = {1,2,3,4,1,2,3,5,1,2,4,5,1,3,4,5,2,3,4,5};
    int64_t correct_list2[3] = {1,2,3};
    int64_t correct_list3[12] = {1,2,1,3,1,4,2,3,2,4,3,4};
    combination(4,k1,listofint1,temp_comb1,0,0,counter1,listofcombinations1);
    combination(2,k2,listofint2,temp_comb2,0,0,counter2,listofcombinations2);
    combination(3,k3,listofint3,temp_comb3,0,0,counter3,listofcombinations3);
    CU_ASSERT_EQUAL(compare_int_list(correct_list1,listofcombinations1,20),0);
    CU_ASSERT_EQUAL(compare_int_list(correct_list2,listofcombinations2,3),0);
    CU_ASSERT_EQUAL(compare_int_list(correct_list3,listofcombinations3,12),0);

}


void test_nbr_combinations(void){
    CU_ASSERT_EQUAL(get_nbr_combinations(4,5),5);
    CU_ASSERT_EQUAL(get_nbr_combinations(2,5),10);
    CU_ASSERT_EQUAL(get_nbr_combinations(0,5),1);
    CU_ASSERT_EQUAL(get_nbr_combinations(1,3),3);


}


int main(){

    if (CUE_SUCCESS != CU_initialize_registry()){
        return CU_get_error();
    }

    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("combination_suite",setup,teardown);
    if(pSuite == NULL){
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite,"test_combination",test_combination) ||
        NULL == CU_add_test(pSuite,"test_nbr_combinations",test_nbr_combinations))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_run_tests();
    CU_basic_show_failures(CU_get_failure_list());

    CU_cleanup_registry();
    return CU_get_error();
}
