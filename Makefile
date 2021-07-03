CC=gcc
CFLAGS=-Wall -Werror -g -std=gnu99
LDFLAGS += -lcunit -lpthread
SRC = $(wildcard main.c src/*.c)
OBJ = $(SRC:.c=.o)


INCLUDE_HEADERS_DIRECTORY=-Iheaders
KMEANS = kmeans
TEST1 = Tests/combinationTest
TEST2 = Tests/distanceTest
TEST3 = Tests/iofilesTest
TEST4 = Tests/kmeansTest
TEST5 = Tests/threadsTest


all: $(KMEANS) 

kmeans: $(OBJ) 

	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o $@ $^ $(LDFLAGS)    

Tests/combinationTest: Tests/combinationTest.o src/combination.o
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o $@ $^ $(LDFLAGS)  

Tests/distanceTest: Tests/distanceTest.o src/distance.o
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o $@ $^ $(LDFLAGS)

Tests/iofilesTest: Tests/iofilesTest.o src/iofiles.o
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o $@ $^ $(LDFLAGS)

Tests/kmeansTest: Tests/kmeansTest.o src/kmeans.o src/distance.o
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	
Tests/threadsTest: Tests/threadsTest.o src/threads.o src/kmeans.o
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c               
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o $@ -c $<

clean:
	rm -f *.o
	rm -f src/*.o
	rm -f Tests/*.o
	rm -f kmeans
	rm -f Tests/combinationTest
	rm -f Tests/distanceTest
	rm -f Tests/iofilesTest
	rm -f Tests/kmeansTest
	rm -f Tests/threadsTest
	rm -f Tests/test_input.bin
	rm -f Tests/test_output.csv
	rm -f Tests/test_output_quiet.csv
	rm -f Tests/manhattan_solution.csv
	rm -f Tests/euclidean_solution.csv
	rm -f python_resources/instance_input/final_test.json
	rm -f python_resources/input_binary/final_test.bin
	rm -f python_resources/output_csv/manhattan_solution_py.csv
	rm -f python_resources/output_csv/euclidean_solution_py.csv
	rm -f manhattan_plot.png
	rm -f euclidean_plot.png
		

tests: $(TEST1) $(TEST2) $(TEST3) $(TEST4) $(TEST5) kmeans # Cunit tests and executable test using .py files given from 'LEPL1503' team to compare solutions.
	echo 'Cunit tests'
	Tests/./combinationTest       
	Tests/./distanceTest
	Tests/./iofilesTest
	Tests/./kmeansTest
	Tests/./threadsTest
	echo 'Generate points'
	python3 python_resources/generate_points.py -f python_resources/instance_input/ambiguous_cluster_example.json -n 300 > python_resources/instance_input/final_test.json
	echo 'Generate .bin input file'
	python3 python_resources/json_to_input.py python_resources/instance_input/final_test.json python_resources/input_binary/final_test.bin
	echo 'Running k-means.py with manhattan distance'
	python3 python_resources/k-means.py -p 8 -k 3 -f python_resources/output_csv/manhattan_solution_py.csv -d manhattan python_resources/input_binary/final_test.bin
	echo 'Running k-means.py with euclidean distance'
	python3 python_resources/k-means.py -p 8 -k 3 -f python_resources/output_csv/euclidean_solution_py.csv -d euclidean python_resources/input_binary/final_test.bin
	echo 'running ./kmeans with manhattan distance'
	./kmeans -n 4 -p 8 -k 3 -f Tests/manhattan_solution.csv -d manhattan python_resources/input_binary/final_test.bin
	echo 'running ./kmeans with euclidean distance'
	./kmeans -n 4 -p 8 -k 3 -f Tests/euclidean_solution.csv -d euclidean python_resources/input_binary/final_test.bin
	echo 'Comparison of csv results with manhattan distance'
	python3 python_resources/compare_solutions.py Tests/manhattan_solution.csv python_resources/output_csv/manhattan_solution_py.csv
	echo 'Comparison of csv results with euclidean distance'
	python3 python_resources/compare_solutions.py Tests/euclidean_solution.csv python_resources/output_csv/euclidean_solution_py.csv
	

# a .PHONY target forces make to execute the command even if the target already exists
.PHONY: clean tests
