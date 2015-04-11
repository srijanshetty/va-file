CC=g++ -std=c++11
CFLAGS=-Wall -c
DEBUG=-g

.PHONY: all clean

# Default to driver.o
all:

# Build the tree
driver.o: driver.cpp vafile.o point.o
	$(CC) $(DEBUG) driver.cpp vafile.o point.o -o tree.out

# Build the vafile library
vafile.o: point.h vafile.h vafile.cpp config.h
	$(CC) $(CFLAGS) $(DEBUG) vafile.cpp

# Build the point library
point.o: point.h point.cpp config.h
	$(CC) $(CFLAGS) $(DEBUG) point.cpp

clean:
	rm -f *.o *.out *.gch
