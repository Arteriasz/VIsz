all: compile

compile:
	g++ -std=c++17 visz.cpp -o main
	./main