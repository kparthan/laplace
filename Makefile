CFLAGS=-std=c++0x -g $(shell pkg-config --cflags liblcb-experimental) 
LDFLAGS=$(shell pkg-config --libs liblcb-experimental) -lboost_program_options

OBJECTS = laplace-fit.o \
  Support.o \
  Laplace.o \
  LaplaceGenerator.o 

all: laplace-fit 

laplace-fit: $(OBJECTS)
	g++ $(OBJECTS) -o $@ $(LDFLAGS) 

laplace-fit.o: laplace-fit.cpp Support.h 
	g++ -c $(CFLAGS) $< -o $@

Support.o: Support.cpp Support.h 
	g++ -c $(CFLAGS) $< -o $@

Laplace.o: Laplace.cpp Laplace.h 
	g++ -c $(CFLAGS) $< -o $@

LaplaceGenerator.o: LaplaceGenerator.cpp LaplaceGenerator.h 
	g++ -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o *~ laplace-fit 

