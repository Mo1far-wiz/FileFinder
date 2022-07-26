output: main.o Finder.o
	g++ -pthread main.o Finder.o -o output -std=c++17

Finder.o: Finder.cpp Finder.h
	g++ -c Finder.cpp

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm *.o output
