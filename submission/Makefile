CC = g++ -O

# optimization 
execute: prodcon.o utility.o tands.o header.h
	$(CC)  -o prodcon prodcon.o utility.o tands.o -lpthread

prodcon.o: prodcon.cpp tands.o utility.o header.h 
	$(CC) -c prodcon.cpp 

utility.o: utility.cpp header.h
	$(CC) -c utility.cpp 

tands.o: tands.cpp
	$(CC) -c tands.cpp 

# Clean
clean: 
	rm -f *.o *.log prodcon