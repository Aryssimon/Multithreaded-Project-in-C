# LEPL1503 - Projet 3
#### Group A4



## Installation

1. Open a terminal in the root folder
2. Compile the code using `make`



## Utilisation
In the **root folder**:
1. Execute command `./kmeans` with the following parameters:
    * -n : _with the number of threads_
    * -k : _with the number of clusters_
    * -p : _with the number of points_
    * -f : _with the name of the output file (.csv)_
    * -d : _with the name of the distance function (either manhattan or euclidean)_
    * (-q : _to enable quiet mode_)
    * followed by the input file's name

> Example : ./kmeans -n 4 -k 5 -p 10 -f <output file>.csv -d manhattan <input file>.bin
>> using 4 threads, 10 points and the manhattan distance function to find 5 clusters from the input.bin and write it to output.csv

2. (Execute command `make clean`  to clean the folders.)
3. (Execute command `make tests` to compile and execute the unit tests.)



## Execution times
These times have been recorded on the Raspberry Pi running on 2.000 points.\
`time ./kmeans -q -n <thread> -p 10 -k 5 -f output.csv -d manhattan input.bin`

Time associated with the number of threads used:
1. Time: 10.391 seconds
2. Time: 5.248 seconds
3. Time: 3.518 seconds
4. Time: 2.702 seconds


## Code Organisation

### Structures
* **point_t** : Represents a point
    * coord : array of int64_t
    * dimension : uint32_t

* **cluster_t** : Represents a cluster
    * points : array of point_t
    * size : int64_t
    * centroid : point_t

* **cluster_init_cent_t** : Combine an array of clusters from kmeans with the initial centroids used in the algorithm
    * clusters : array of cluster_t
    * init_centroids : array of initial centroids (point_t)

* **args_t** : Represents the program's parameters

* **producer_args_t** : Used to pass all the needed parameters to the producer's thread function

### Threads
A thread receives a pointer to a producer_args_t as parameter which contains all the needed variables.\
The thread executes the kmeans algorithm with his given input and stores the output in a buffer.\
This buffer is an array of cluster_init_cent_t.


### Concurrency
A main thread executes the main() function, it produces the list of possible combinations of points.\
The n threads use this list to produce a kmeans result and feed the buffer (size = 2x number of threads).\
Simultaneously, the main thread consumes a result from the buffer and writes it to the output file.\
To avoid two threads writing at the same time in the buffer, we use a __mutex to lock and unlock__ when pushing.\
To avoid pushing when the buffer is full, we use a _semaphore_ to check if the __buffer is full__.\
We have a _second semaphore_ that checks, on the opposite, if the __buffer is not empty__ when the consumer is\
trying to retrieve one result


### Main functions
* **main()**  calls:
    * **parse_args()** parses the program parameters.
    * **parse_input_file()** parses the input to create the points.
    * **combination()** creates all the possible combinations.
    * _For each possible combination_:
        * **kmeans()** executes the algorithm to calculate the clusters and centroids.
        * **distortion()** calculates the distortion based on the result of kmeans().
        * **write_line_csv()** writes a line containing these results and the current combination to the output.


### Folders and files
* **headers/**
    * _args.h_ : Structure and includes used to represent the program parameters.
    * _clusters.h_ : Structures and includes used to define the Cluster and the Cluster/Initial_Centroids structures.
    * _combination.h_ : Definitions of functions and includes used to create all the possible combinations of points.
    * _distance.h_ : Definitions of distance and distortion functions and includes needed.
    * _iofiles.h_ : Definitions of functions and includes used by iofiles.
    * _kmeans.h_ : Definitions of functions and includes used by kmeans algorithm.
    * _point.h_ : Structure and include used to define a Point.
    * _threads.h_ : Structure, definitions of functions and includes used by threads.
* **python_resources/** Contains all the files from the exemple repository (these are useful for the tests)
* **src/**
    * _combination.c_ : The functions used to create all the possible points combinations.
    * _distance.c_ : The manhattan, euclidean and distortion functions.
    * _iofiles.c_ : The function used  to parse the input and write the output.
    * _kmeans.c_ : The K-Means algorithm, divided into 4 functions.
    * _threads.c_ : The functions used by the producer threads.
* **Tests/**
    * _combinationTest.c_ : Test suite for the functions stored in the combination.c file.
    * _distanceTest.c_ : Test suite for the functions stored in the distance.c file.
    * _iofilesTest.c_ : Test suite for the functions stored in the iofiles.c file.
    * _kmeansTest.c_ : Test suite for the functions stored in the kmeans.c file.
    * _threadsTest.c_ : Test suite for the functions stored in the threads.c file.
    * _output_quiet_solution.csv_ : File used by iofilesTest to verify the output when using quiet.
    * _output_solution.csv : File used by iofilesTest to verify the output.


* _main.c_ : The main file, controller of the other files.
* _Makefile_ : The file used to compile all the code and to compile and execute all the unit tests and executable tests.
* _README.md_ : This explanation file.



## Authors

* Alsteens Louis
* Arys Simon
* Bastenier Pierre
* El Ouilinti Aymen
* Sirjacobs Noam
* Thirifay Louis



## References

* Example project in python from [jadinm GitHub repository](https://github.com/jadinm/LEPL1503-projet-2021).
* Project skeleton in C from [michelfra UCLouvain repository](https://forge.uclouvain.be/michelfra/lepl1503-2021-skeleton).
* [Lloyd's algorithm page](https://en.wikipedia.org/wiki/Lloyd\%27s\_algorithm) from Wikipedia.
* [Balena Etcher](https://www.balena.io/etcher/) used to flash Raspberry Pi OS.
