CC = g++

# optimization 
execute: prodcon.o utility.o tands.o utility.o
	$(CC) -pthread prodcon.o utility.o tands.o -o prodcon

prodcon.o: prodcon.cpp header.h 
	$(CC) -c prodcon.cpp -o prodcon.o

utility.o: utility.cpp header.h
	$(CC) -c utility.cpp -o utility.o

tands.o: tands.cpp
	$(CC) -c tands.cpp -o tands.o

# Clean
clean: 
	rm -f *.o *.log prodcon