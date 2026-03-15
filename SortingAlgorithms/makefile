
INCLUDES= machine.h products.h tbd.h customkey.h mutants.h

all: remove machine

machine: machine.cpp tests.o customkey.o classfactory.h 
	g++ machine.cpp -o machine tests.o customkey.o

tests.o: tests.cpp tests.h $(INCLUDES)
	g++ tests.cpp -c

customkey.o: customkey.cpp customkey.h $(INCLUDES)
	g++ customkey.cpp -c

remove:
	-rm machine

clean:
	rm *.o
	rm machine

