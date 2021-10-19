
INCLUDES=graph.h

all: graph

graph: graph.cpp utility.o
	g++ graph.cpp -o graph utility.o

utility.o: utility.cpp $(INCLUDES)
	g++ utility.cpp -c

remove:
	-rm graph

clean:
	-rm *.o
	-rm graph

