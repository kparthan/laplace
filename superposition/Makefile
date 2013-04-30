CFLAGS=-std=c++0x -g $(shell pkg-config --cflags liblcb-experimental) 
LDFLAGS=$(shell pkg-config --libs liblcb-experimental) -lboost_program_options 

OBJECTS = superimpose.o \
  Support.o \
  Simulation.o

all: superimpose

superimpose: $(OBJECTS)
	g++ $(OBJECTS) -o $@ $(LDFLAGS) 

superimpose.o: superimpose.cpp 
	g++ -c $(CFLAGS) $< -o $@

Support.o: Support.cpp Support.h
	g++ -c $(CFLAGS) $< -o $@

Simulation.o: Simulation.cpp Simulation.h
	g++ -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o *~ superimpose

