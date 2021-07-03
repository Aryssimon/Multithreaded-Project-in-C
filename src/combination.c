#include "../headers/combination.h"

void combination(int64_t n, int64_t k, int64_t *list_of_int, int64_t  *temp_combination, int64_t start, int64_t current, int64_t *counter, int64_t *list_of_combinations){
    if (current == k){   
      for (int64_t j = 0;  j < k; j++){
        list_of_combinations[counter[0]] = temp_combination[j];
        counter[0] += 1;
      }               
      return;
    } 
    int64_t i = start;
    while(i <= n && ((n - i) + 1) >= (k - current)){
      temp_combination[current] = list_of_int[i];
      combination(n, k, list_of_int, temp_combination, i + 1, current + 1, counter, list_of_combinations);
      i += 1;
    }
    return;
}


int64_t get_nbr_combinations(int64_t k, int64_t n){
    if (k == 0 || k == n) {
    	return 1;
    } else {
    	return get_nbr_combinations(k-1,n-1) + get_nbr_combinations(k,n-1);
    } 
}

int8_t compare_int_list(int64_t *first, int64_t *second, int64_t len){
    for (int64_t i=0;i<len;i++){
        if (first[i] != second[i]){
            return 1;
        }
    }
    return 0;
}
