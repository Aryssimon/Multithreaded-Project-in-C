#include "point.h"
#include "cluster.h"
#include <inttypes.h>
#include <pthread.h>
#include <semaphore.h>
#include "../headers/args.h"

#ifndef THREADSARGS_H
#define THREADSARGS_H


/**
 * structure containing all the arguments that are passed to
 * the thread_producer function used when creating threads.
 * 
 * contains the distance function, semaphores, mutex,
 * a buffer for the results of kmeans, the initials centroids.
 * 
 */
typedef struct {
    point_t *points;
    uint64_t nb_of_points;
    int64_t nb_of_clusters;
    int64_t *list_of_combinations;
    int64_t (*distance)(const point_t *, const point_t *);
    pthread_mutex_t *mutex;
    int64_t *counter;
    sem_t *empty;
    sem_t *full;
    int64_t nbr_combination;
    cluster_init_cent_t *buf;
    int64_t buf_len;
    int64_t *last;
} producer_args_t;


/**
 * function that pushes the result of one kmeans into a buffer at index last
 * @param buf : the buffer containing the results of multiple kmeans
 * @param len : the length of the buffer
 * @param last : the index of the last element in the buffer
 * @param data : the resulting list of clusters from one kmeans
 * @param init_centroids : the initial centroids associated with the list of clusters
 */
void push(cluster_init_cent_t *buf, int64_t len, int64_t *last, cluster_t *data, point_t *init_centroids);


/**
 * function that gets the result of one kmeans at index first from a buffer
 * @param buf : the buffer containing the results of multiple kmeans
 * @param len : the length of the buffer
 * @param first the index of the first element in the buffer
 * 
 * @return a cluster_init_cent_t containing a list of clusters with its initial centroids
 */
cluster_init_cent_t get(cluster_init_cent_t *buf, int64_t len, int64_t *first);

/**
 * function called when creating a producer threads
 * this function will create an initialization of centroids, call kmeans once to calculate a result and push it into the buffer
 */
void *thread_producer(void *args);


/**
 * function that increments current args->counter and check if the counter is < to the number of initial combinations
 * function used in thread_producer so that the threads don't call kmeans more than the number of initial combinations
 * @param args : the arguments of the function thread_producer
 * @return current args->counter if < nbr_combinations, else -1.
 */
int64_t compare_and_increment_counter(producer_args_t *args);

#endif //THREADSARGS_H
