#ifndef IOFILES_H
#define IOFILES_H
#include <stdint.h>
#include "point.h"
#include "cluster.h"
#include <stdlib.h>
#include <stdio.h>
#include "args.h"

/**
 * function that read the binary file and fill the list of points, number of points and the dimension of a point
 *
 * @param stream : the binary input file
 * @param points : a pointer to the empty list of points
 * @param dimensions : the dimension of a point
 * @return 0 if sucess, -1 if failed
 */
int64_t parse_input_file(FILE *stream, uint32_t *dimensions, uint64_t *nbr_points, point_t **points);


/**
 * function that write a line with the following content :
 * initialization centroids,distortion,centroids,clusters
 * for example : "[(1, 1), (2, 2)]",11,"[(1, 1), (4, 5)]","[[(1, 1), (2, 2)], [(1, 1), (3, 4)]]"
 * if the quiet mode is on, the last column clusters will not be written in the file
 * 
 * @param args : the program arguments containing the output file and the boolean quiet
 * @param init_centroids : list of point_t, the initial centroids for the first column
 * @param clusters : the list of cluster_t
 * @param distortion : the corresponding distortion for the clusters
 */
void write_line_csv(args_t args, cluster_t * clusters, point_t *init_centroids, int64_t distortion);


/**
 * compare the content of two files
 * @return 0 if both files have the same content, else 1
 */
int64_t compare_files(FILE *file1, FILE *file2);



#endif //IOFILES_H
