#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>
#include <errno.h>
#include <inttypes.h>
#include "headers/args.h"
#include <endian.h>
#include <pthread.h>
#include <semaphore.h>
#include "headers/distance.h"
#include "headers/combination.h"
#include "headers/iofiles.h"
#include "headers/kmeans.h"
#include "headers/threads.h"
#include <sys/time.h>


void usage(char *prog_name) {
    fprintf(stderr, "USAGE:\n");
    fprintf(stderr, "    %s [-p n_combinations_points] [-n n_threads] [input_filename]\n", prog_name);
    fprintf(stderr, "    -k n_clusters (default value: 2): the number of clusters to compute\n");
    fprintf(stderr, "    -p n_combinations (default value: equal to k): consider the n_combinations first points present in the input to generate possible initializations for the k-means algorithm\n");
    fprintf(stderr, "    -n n_threads (default value: 4): sets the number of computing threads that will be used to execute the k-means algorithm\n");
    fprintf(stderr, "    -f output_file (default value: stdout): sets the filename on which to write the csv result\n");
    fprintf(stderr, "    -q quiet mode: does not output the clusters content (the \"clusters\" column is simply not present in the csv)\n");
    fprintf(stderr, "    -d distance (manhattan by default): can be either \"euclidean\" or \"manhattan\". Chooses the distance formula to use by the algorithm to compute the distance between the points\n");
}

int parse_args(args_t *args, int argc, char *argv[]) {
    memset(args, 0, sizeof(args_t));    // set everything to 0 by default
    // the default values are the following, they will be changed depending on the arguments given to the program
    args->k = 2;
    args->n_first_initialization_points = args->k;
    args->n_threads = 4;
    args->output_stream = stdout;
    args->quiet = false;
    args->squared_distance_func = squared_manhattan_distance;
    int opt;
    while ((opt = getopt(argc, argv, "n:p:k:f:d:q")) != -1) {
        switch (opt)
        {
            case 'n':
                args->n_threads = atoi(optarg);
                if (args->n_threads <= 0) {
                    fprintf(stderr, "Wrong number of threads. Needs a positive integer, received \"%s\"\n", optarg);
                    return -1;
                }
                break;
            case 'p':
                args->n_first_initialization_points = atoi(optarg);
                if (args->n_first_initialization_points <= 0) {
                    fprintf(stderr, "Wrong number of initialization points. Needs a positive integer, received \"%s\"\n", optarg);
                    return -1;
                }
                break;
            case 'k':
                args->k = atoi(optarg);
                if (args->k <= 0) {
                    fprintf(stderr, "Wrong k. Needs a positive integer, received \"%s\"\n", optarg);
                    return -1;
                }
                break;
            case 'f':
                args->output_stream = fopen(optarg, "w");
                if (!args->output_stream) {
                    fprintf(stderr, "could not open output file %s: %s\n", optarg, strerror(errno));
                    return -1;
                }
                break;
            case 'q':
                args->quiet = true;
                break;
            case 'd':
                if (strcmp("euclidean", optarg) == 0) {
                    args->squared_distance_func = squared_euclidean_distance;
                }
                break;
            case '?':
                usage(argv[0]);
                return 1;
            default:
                usage(argv[0]);
        }
    }

    if (optind == argc) {
        args->input_stream = stdin;
    } else {
        args->input_stream = fopen(argv[optind], "r");
        if (!args->input_stream) {
            fprintf(stderr, "could not open file %s: %s\n", argv[optind], strerror(errno));
            return -1;
        }
    }

    return 0;
}


int main(int argc, char *argv[]) {
    
    args_t program_arguments;   // allocate the args on the stack
    if (parse_args(&program_arguments, argc, argv) == -1) return -1;

    if (program_arguments.n_first_initialization_points < program_arguments.k) {
        fprintf(stderr, "Cannot generate an instance of k-means with less initialization points than needed clusters: %"PRIu32" < %"PRIu32"\n",
                program_arguments.n_first_initialization_points, program_arguments.k);
        return -1;
    }


    // the following fprintf (and every code already present in this skeleton) can be removed, it is just an example to show you how to use the program arguments
    fprintf(stderr, "\tnumber of threads executing the LLoyd's algoprithm in parallel: %" PRIu32 "\n", program_arguments.n_threads);
    fprintf(stderr, "\tnumber of clusters (k): %" PRIu32 "\n", program_arguments.k);
    fprintf(stderr, "\twe consider all the combinations of the %" PRIu32 " first points of the input as initializations of the Lloyd's algorithm\n", program_arguments.n_first_initialization_points);
    fprintf(stderr, "\tquiet mode: %s\n", program_arguments.quiet ? "enabled" : "disabled");
    fprintf(stderr, "\tsquared distance function: %s\n", program_arguments.squared_distance_func == squared_manhattan_distance ? "manhattan" : "euclidean");


    if (program_arguments.quiet)
        fprintf(program_arguments.output_stream,"initialization centroids,distortion,centroids\n");
    else
        fprintf(program_arguments.output_stream,"initialization centroids,distortion,centroids,clusters\n");

    int64_t BUFFER_SIZE = program_arguments.n_threads*2;

    int64_t error;
    
    uint32_t *dimensions = (uint32_t *) malloc(sizeof(uint32_t));
    if (dimensions == NULL) fprintf(stderr, "malloc failed (allocating dimensions)");
    uint64_t *nbr_points = (uint64_t *) malloc(sizeof(uint64_t));
    if (nbr_points == NULL) fprintf(stderr, "malloc failed (allocating nbr_points)");
    point_t **points = (point_t**) malloc(sizeof(point_t*));
    if (points == NULL) fprintf(stderr, "malloc failed (allocating points)");

    error = parse_input_file(program_arguments.input_stream, dimensions, nbr_points, points);
    if (error < 0){
        fprintf(stderr,"error parsing the input file");
        return -1;
    }

    if (program_arguments.n_first_initialization_points > *nbr_points) {
        fprintf(stderr,"[-p n_combinations_points] needs to be lower then the number of points, in this case : %"PRIu64"\n", *nbr_points);
        return -1;
    }

    int64_t nbr_combinations = get_nbr_combinations(program_arguments.k, program_arguments.n_first_initialization_points);
    
    //list that will contain indices of all combinations of initial centroids
    int64_t *list_of_combinations = (int64_t *) malloc(sizeof(int64_t) * nbr_combinations * program_arguments.k);
    if (list_of_combinations == NULL) fprintf(stderr, "malloc failed (allocating list_of_combinations)");

    //indices of n first points to generate all possible combinations.
    int64_t *list_of_int = (int64_t *) malloc(sizeof(int64_t) * program_arguments.n_first_initialization_points);
    if (list_of_int == NULL) fprintf(stderr, "malloc failed (allocating list_of_int)");
    for (int64_t i = 0; i < program_arguments.n_first_initialization_points; i++) {
        list_of_int[i] = i;
    }

    int64_t *temp_combination = (int64_t *) malloc(sizeof(int64_t) * program_arguments.k);
    if (temp_combination == NULL) fprintf(stderr, "malloc failed (allocating temp_combination)");
    int64_t counter = 0;


    //stores all possible combinations in list_of_combinations
    combination(program_arguments.n_first_initialization_points - 1, program_arguments.k, list_of_int, temp_combination, 0, 0, &counter, list_of_combinations);

    // Threads
    pthread_mutex_t mutex_producer;
    error = pthread_mutex_init(&mutex_producer, NULL);
    if (error != 0) fprintf(stderr, "error initializing mutex");

    //semaphore to check if buffer is full or empty.
    sem_t empty;
    error = sem_init(&empty, 0 , BUFFER_SIZE);
    if (error < 0) fprintf(stderr, "error initializing semaphore");
    
    sem_t full;
    error = sem_init(&full, 0 , 0);
    if (error < 0) fprintf(stderr, "error initializing semaphore");
    
    cluster_init_cent_t *buffer = (cluster_init_cent_t *) malloc(sizeof(cluster_init_cent_t) * BUFFER_SIZE);
    if (buffer == NULL) fprintf(stderr, "malloc failed allocating buffer");

    //initializing arguments for producer thread
    producer_args_t producer_args;
    producer_args.points = *points;
    producer_args.nb_of_clusters = program_arguments.k;
    producer_args.list_of_combinations = list_of_combinations;
    producer_args.nb_of_points = *nbr_points;
    producer_args.buf = buffer;
    producer_args.buf_len = BUFFER_SIZE;
    int64_t producer_counter = 0;
    producer_args.counter = &producer_counter;
    producer_args.distance = program_arguments.squared_distance_func;
    producer_args.empty = &empty;
    producer_args.full = &full;
    int64_t buffer_last = 0;
    producer_args.last = &buffer_last;
    producer_args.mutex = &mutex_producer;
    producer_args.nbr_combination = nbr_combinations;


    //creating producer threads
    pthread_t threads[program_arguments.n_threads];
    for (int64_t i = 0; i < program_arguments.n_threads; i++) {

        error = pthread_create(&(threads[i]),NULL, &thread_producer, (void *) &producer_args);
        if (error != 0) fprintf(stderr, "pthread_create failed");
    }

    int64_t buffer_first = 0;
    int64_t consumer_counter = 0;
    
    //consumer loop
    while (consumer_counter < nbr_combinations){
        
        error = sem_wait(&full);
        if(error < 0) fprintf(stderr, "sem_wait failed");

        consumer_counter += 1;
        cluster_init_cent_t kmeans_result = get(buffer, BUFFER_SIZE, &buffer_first);
        int64_t disto = distortion(kmeans_result.clusters, program_arguments.squared_distance_func, program_arguments.k);
        write_line_csv(program_arguments, kmeans_result.clusters, kmeans_result.init_centroids, disto);

        for (int64_t i = 0; i < program_arguments.k; ++i) {
            free(kmeans_result.clusters[i].points);
            free(kmeans_result.clusters[i].centroid.coord);
        }

        free(kmeans_result.clusters);
        free(kmeans_result.init_centroids);

        error = sem_post(&empty);
        if(error<0) fprintf(stderr, "sem_post failed");
    }

    for (int64_t i = 0; i < program_arguments.n_threads; ++i) {
        error = pthread_join(threads[i], NULL);
        if (error != 0) fprintf(stderr, "pthread_join failed");
    }

    error = pthread_mutex_destroy(&mutex_producer);
    if(error != 0) fprintf(stderr, "error destroying mutex");

    error = sem_destroy(&empty);
    if(error<0) fprintf(stderr, "sem_destroy failed");

    error = sem_destroy(&full);
    if(error<0) fprintf(stderr,"sem_destroy failed");

    // FREE area
    free(dimensions);

    for (int64_t i = 0; i < *nbr_points; ++i) {
        free((*points)[i].coord);
    }
    free(*points);
    free(points);
    free(nbr_points);
    free(list_of_combinations);
    free(list_of_int);
    free(temp_combination);
    free(buffer);


    // close the files opened by parse_args
    if (program_arguments.input_stream != stdin) {
        if (fclose(program_arguments.input_stream) != 0) fprintf(stderr,"closing input file failed");
    }
    if (program_arguments.output_stream != stdout) {
        if (fclose(program_arguments.output_stream) != 0) fprintf(stderr,"closing output file");
    }
    
    return 0;
}
