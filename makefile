
machine: machine.cpp classfactory.h machine.h products.h tbd.h tests.o 
	g++ machine.cpp -o machine tests.o

tests.o: tests.cpp tests.h classfactory.h machine.h products.h tbd.h
	g++ tests.cpp -c

clean:
	rm *.o
	rm machine

