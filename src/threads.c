#include "../headers/threads.h"
#include "../headers/kmeans.h"
#include "../headers/iofiles.h"

void push(cluster_init_cent_t *buf, int64_t len, int64_t *last, cluster_t *data, point_t *init_centroids){

    //push at the index last
    buf[*last].clusters = data;
    buf[*last].init_centroids = init_centroids;
    if ((*last + 1) == len){
        *last = 0;
    } else {
        *last = *last+1;
    }
}

cluster_init_cent_t get(cluster_init_cent_t *buf, int64_t len, int64_t *first){

    //get at the index first
    cluster_init_cent_t ret = buf[*first];
    if ((*first + 1) == len){
        *first = 0;
    } else {
        *first = *first+1;
    }
    return ret;
}

int64_t compare_and_increment_counter(producer_args_t *args){
    int lock = pthread_mutex_lock(args->mutex);
    if (lock != 0) fprintf(stderr, "mutex lock failed");

    // store current counter and increment the counter
    int64_t res = *(args->counter);
    *(args->counter) += 1;

    int unlock = pthread_mutex_unlock(args->mutex);
    if (unlock != 0) fprintf(stderr, "mutex unlock failed");

    // check if current counter is not higher than nbr_combination so that threads will stop
    if (res < args->nbr_combination){
        return res;
    }
    return -1;
}

void *thread_producer(void *argument_struct){
    producer_args_t *args = (producer_args_t *) argument_struct;
    int lock;
    int unlock;
    int error;
    int64_t current = compare_and_increment_counter(args);

    while (current != -1){
        
        point_t *initial_centroids = (point_t *) malloc(sizeof(point_t) * args->nb_of_clusters);
        if (initial_centroids == NULL) fprintf(stderr, "malloc failed (allocating initial_centroids)");
        // create an intialization of points based on list_of_combination (containing indices)
        for (int64_t j = 0; j < args->nb_of_clusters; j++) {
            initial_centroids[j] = (args->points)[args->list_of_combinations[(current * args->nb_of_clusters) + j]];
        }
        
        //calculate kmeans
        cluster_t *clusters = k_means(args->points, args->nb_of_points, args->nb_of_clusters, initial_centroids, args->distance);

        //don't push if buffer is full
        error = sem_wait(args->empty);
        if (error < 0) fprintf(stderr, "sem_wait failed");


        //lock to push in the buffer
        lock = pthread_mutex_lock(args->mutex);
        if (lock != 0) fprintf(stderr, "mutex lock failed");

        push(args->buf, args->buf_len, args->last, clusters, initial_centroids);

        unlock = pthread_mutex_unlock(args->mutex);
        if (unlock != 0) fprintf(stderr, "mutex unlock failed");

        //notify consumer
        error = sem_post(args->full);
        if (error < 0) fprintf(stderr, "sem_post failed");
        
        //check condition for next iteration (if threads have consumed all initialization)
        current = compare_and_increment_counter(args);

    }
    return NULL;
}
