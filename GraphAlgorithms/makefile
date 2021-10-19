
INCLUDES=graph.h

all: graph

graph: graph.cpp utility.o $(INCLUDES)
	g++ -I. graph.cpp -o graph utility.o

utility.o: utility.cpp $(INCLUDES)
	g++ -I. utility.cpp -c

remove:
	-rm graph

clean:
	-rm *.o
	-rm graph

