
INCLUDES= machine.h products.h tbd.h customkey.h

machine: machine.cpp tests.o customkey.o classfactory.h 
	g++ machine.cpp -o machine tests.o customkey.o

tests.o: tests.cpp tests.h $(INCLUDES)
	g++ tests.cpp -c

customkey.o: customkey.cpp customkey.h $(INCLUDES)
	g++ customkey.cpp -c

clean:
	rm *.o
	rm machine

