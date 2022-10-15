CC = g++
C = gcc
CFLAGS = -Wall -O2

all: prodcon

prodcon: prodcon.o utility.o tands.o
	$(CC) prodcon.o utility.o tands.o -o prodcon

prodcon.o: prodcon.cpp header.h tands.c
	$(CC) -c prodcon.cpp -o prodcon.o

utility.o: utility.cpp header.h
	$(CC) -c utility.cpp -o utility.o

tands.o: tands.c
	$(C) -c tands.c -o tands.o

clean: 
	rm -f prodcon.o utility.o tands.o prodcon
