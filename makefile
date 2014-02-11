
INCLUDES= machine.h products.h tbd.h 

machine: machine.cpp tests.o classfactory.h 
	g++ machine.cpp -o machine tests.o

tests.o: tests.cpp tests.h $(INCLUDES)
	g++ tests.cpp -c

clean:
	rm *.o
	rm machine

