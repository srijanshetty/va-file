CC=g++ -std=c++11
CFLAGS=-Wall -c
DEBUG=-g

.PHONY: clean

# Build the tree
driver.o: driver.cpp vafile.o
	$(CC) $(DEBUG) driver.cpp vafile.o -o tree.out

# Build the vafile library
vafile.o: vafile.h vafile.cpp config.h
	$(CC) $(CFLAGS) $(DEBUG) vafile.cpp

clean:
	rm -f *.o *.out *.gch
