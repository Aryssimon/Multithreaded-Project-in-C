#ifndef COMBINATION_H
#define COMBINATION_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


/**
 * stores all the indices of possible combinations of C(n,k) in list_of_combination
 * for example : for C(5,4), list_of_combination = [1,2,3,4,1,2,3,5,1,2,4,5,1,3,4,5,2,3,4,5]
 * as the number of clusters is 4, there is one combination every 4 indices
 * @param k : the number of clusters
 * @param n : the n_first initialization points
 * @param list_of_int : list that contains the indices from 0 to n-1
 * @param list_of_combinations : empty list that will contain all the possible combinations
 * @param temp_combination : list of length k that stores one combination at a time
 */
void combination(int64_t n, int64_t k, int64_t *list_of_int, int64_t  *temp_combination, int64_t start, int64_t current, int64_t *counter, int64_t *list_of_combinations);


/**
 * n >= k >= 0
 * @return the binomial coefficient C(n,k) = n!/(n-k)!*k!
 */
int64_t get_nbr_combinations(int64_t k, int64_t n);


/**
 * Compare two lists of ints of length len
 * len(first) == len(second)
 * @return 0 if both lists are the same, else 1
 */
int8_t compare_int_list(int64_t *first, int64_t *second, int64_t len);
#endif //COMBINATION_H
