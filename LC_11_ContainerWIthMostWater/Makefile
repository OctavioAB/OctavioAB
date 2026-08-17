CC = g++
DEBUG = -g -Wall -Wno-reorder -O0

test: main.o solution.o
	$(CC) $(DEBUG) -o test main.o solution.o

main.o: main.cpp
	$(CC) $(DEBUG) -c main.cpp

solution.o: solution.cpp
	$(CC) $(DEBUG) -c solution.cpp

clean:
	rm main.o solution.o test