PROG = ../dfs
CC = g++
CPPFLAGS = -g -Wall -std=c++17 -I./source 
OBJS = main.o Graph.o

$(PROG) : $(OBJS)
	$(CC) -o $(PROG) $(OBJS) -lstdc++ -O3 	



main.o : Graph.o
	$(CC) $(CPPFLAGS) -c main.cpp Graph.o -lstdc++

Graph.o : Graph.hpp
	$(CC) $(CPPFLAGS) -c Graph.cpp -lstdc++


clean:
	rm -f $(PROG) $(OBJS)
