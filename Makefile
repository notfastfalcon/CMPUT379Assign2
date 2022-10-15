CC = g++
C = gcc
CFLAGS = -Wall -O2

all: prodcon

run: prodcon
	./prodcon

prodcon: prodcon.o tands.o
	$(CC) prodcon.o tands.o -o prodcon

prodcon.o: prodcon.cpp header.h tands.c
	$(CC) -c prodcon.cpp -o prodcon.o

tands.o: tands.c
	$(C) -c tands.c -o tands.o

clean: 
	rm -f prodcon.o tands.o prodcon
