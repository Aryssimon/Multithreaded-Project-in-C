#include "../headers/iofiles.h"
#include "../headers/args.h"
#include <stdlib.h>
#include <inttypes.h>
#include "../headers/threads.h"
#include <stdbool.h>


int64_t parse_input_file(FILE *stream, uint32_t *dimensions, uint64_t *nbr_points, point_t **points){
    int err;
    //read dimensions and convert from big endian to home
    err = fread(dimensions,sizeof(uint32_t),1,stream);
    if(err<0){
        fprintf(stderr,"Erreur fread parse input\n");
        return -1;
    }
    *dimensions = be32toh(*dimensions);
    
    //read number of points and convert from big endian to home
    err = fread(nbr_points,sizeof(uint64_t),1,stream);
    if(err<0){
        fprintf(stderr,"Erreur fread parse input\n");
        return -1;
    }
    *nbr_points = be64toh(*nbr_points);
    
    *points = (point_t*)malloc(sizeof(point_t)*(*nbr_points));
    if (*points == NULL) fprintf(stderr, "malloc failed (allocating points array)");
    
    //read and create every point_t and convert every coordinate from big endian to home
    for (uint64_t i = 0;i<(*nbr_points);i++){
        
        point_t new_point;
        new_point.dimension = *dimensions;
        new_point.coord = (int64_t*)malloc(sizeof(int64_t)*(*dimensions));
        if (new_point.coord == NULL) fprintf(stderr, "malloc failed (allocating point coord)");
        
        for (uint32_t j = 0;j<(*dimensions);j++){
            int64_t coordinate_j;
            err = fread(&coordinate_j,sizeof(int64_t),1,stream);
            if(err<0){
                fprintf(stderr,"Erreur fread parse input\n");
                return -1;
            }
            new_point.coord[j] = be64toh(coordinate_j);
        }
        (*points)[i] = new_point;
    }
    return 0;
}


void write_line_csv(args_t args, cluster_t * clusters, point_t *init_centroids, int64_t distortion){
    FILE *output = args.output_stream;
    uint32_t dimension = init_centroids[0].dimension;
    uint32_t k;
    fprintf(output,"\"[");
    
    //write the initial centroids
    for (int64_t j = 0;j<args.k;j++){
        fprintf(output,"(");
        for (k = 0;k<dimension-1;k++){
            fprintf(output,"%"PRId64", ",init_centroids[j].coord[k]);
        }
        if (j==args.k-1)    
            fprintf(output,"%"PRId64")",init_centroids[j].coord[k]);
        else
            fprintf(output,"%"PRId64"), ",init_centroids[j].coord[k]);
    }
    
    //write the distortion
    fprintf(output,"]\",%"PRId64",\"[",distortion);
    
    //write the final centroids
    for (int64_t j = 0;j<args.k;j++){
        fprintf(output,"(");
        for (k = 0;k<dimension-1;k++){
            fprintf(output,"%"PRId64", ",clusters[j].centroid.coord[k]);
        }
        if (j==args.k-1)
            fprintf(output,"%"PRId64")",clusters[j].centroid.coord[k]);
        else
            fprintf(output,"%"PRId64"), ",clusters[j].centroid.coord[k]);
    }
    fprintf(output,"]\"");
        
        // if quiet mode off, write the clusters
        if (!args.quiet){
            fprintf(output,",\"[");
            for (int64_t h = 0;h<args.k;h++){
                fprintf(output,"[");
                for (int64_t j = 0;j<clusters[h].size;j++){
                    fprintf(output,"(");
                    for (k = 0;k<dimension-1;k++){
                        fprintf(output,"%"PRId64", ",clusters[h].points[j].coord[k]);
                    }
                    if (j==clusters[h].size-1)
                        fprintf(output,"%"PRId64")",clusters[h].points[j].coord[k]);
                    else
                        fprintf(output,"%"PRId64"), ",clusters[h].points[j].coord[k]);
                }
                if (h != args.k-1)
                    fprintf(output,"], ");
                else
                    fprintf(output,"]");
            }
            fprintf(output,"]\"\n");
        } else {
            fprintf(output,"\n");
        }
}

int64_t compare_files(FILE *file1, FILE *file2){
    signed char c1 = getc(file1);
    signed char c2 = getc(file2);

    //compare each char of file1 and file2
    while(c1 != EOF && c2 != EOF){
        if (c1 != c2){
            return 1;
        } else {
            c1 = getc(file1);
            c2 = getc(file2);
        }
    }
    if (c1 == EOF && c2 == EOF)
        return 0;
    else
        return 1;
}
