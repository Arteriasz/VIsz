all: compile

compile:
	g++ -std=c++17 src/visz.cpp -o main-app
	./main-app