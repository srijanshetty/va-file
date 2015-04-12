CC=g++ -std=c++11
CFLAGS=-Wall -c
DEBUG=-g

.PHONY: clean

# Build the tree
driver.o: driver.cpp vafile.o linear.o
	$(CC) $(DEBUG) driver.cpp vafile.o linear.o -o tree.out

# Build the vafile library
vafile.o: vafile.h vafile.cpp config.h
	$(CC) $(CFLAGS) $(DEBUG) vafile.cpp

# Build the linear library
linear.o: linear.h linear.cpp config.h vafile.h
	$(CC) $(CFLAGS) $(DEBUG) linear.cpp

clean: clean-files
	@rm -f *.o *.out *.gch

clean-files:
	rm -rf objects
	rm -f .vafile
	mkdir objects

