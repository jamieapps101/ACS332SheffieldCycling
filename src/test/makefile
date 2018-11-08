all: exec
	./exec

exec: main.o
	g++ -o exec main.o

main.o: main.cpp
	gcc -c main.cpp -o main.o
