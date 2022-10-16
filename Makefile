CC = g++

all: prodcon

prodcon: prodcon.o utility.o tands.o
	$(CC) -pthread prodcon.o utility.o tands.o -o prodcon

prodcon.o: prodcon.cpp header.h 
	$(CC) -c prodcon.cpp -o prodcon.o

utility.o: utility.cpp header.h
	$(CC) -c utility.cpp -o utility.o

tands.o: tands.cpp
	$(CC) -c tands.cpp -o tands.o

clean: 
	rm -f prodcon.o utility.o tands.o prodcon
