CC = g++
CFLAGS = -g -O2 -Wall

all: matrix.o matrix_test

clean:
	rm matrix.o matrix_test

matrix.o: matrix.cpp matrix.h
	$(CC) $(CFLAGS) -c matrix.cpp

matrix_test: matrix.o matrix_test.cpp
	$(CC) $(CFLAGS) matrix.o matrix_test.cpp -o matrix_test
