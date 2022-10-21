CC = g++

# optimization 
execute: prodcon.o utility.o tands.o producer.o consumer.o utility.o
	$(CC) -pthread prodcon.o utility.o tands.o producer.o consumer.o -o prodcon

prodcon.o: prodcon.cpp header.h 
	$(CC) -c prodcon.cpp -o prodcon.o

utility.o: utility.cpp header.h
	$(CC) -c utility.cpp -o utility.o

tands.o: tands.cpp
	$(CC) -c tands.cpp -o tands.o

producer.o: producer.cpp header.h
	$(CC) -c producer.cpp -o producer.o

consumer.o: consumer.cpp header.h
	$(CC) -c consumer.cpp -o consumer.o

# Clean
clean: 
	rm -f *.o prodcon