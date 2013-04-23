CFLAGS=-std=c++0x -g $(shell pkg-config --cflags liblcb-experimental) 
LDFLAGS=$(shell pkg-config --libs liblcb-experimental) -lboost_program_options

OBJECTS = laplace-fit.o \
  Support.o \
  DataGenerator.o \
  NormalDataGenerator.o \
  Normal.o \
  LaplaceDataGenerator.o \
  Laplace.o \
  Plot.o \
  Message.o

all: laplace-fit 

laplace-fit: $(OBJECTS)
	g++ $(OBJECTS) -o $@ $(LDFLAGS) 

laplace-fit.o: laplace-fit.cpp Support.h 
	g++ -c $(CFLAGS) $< -o $@

Support.o: Support.cpp Support.h 
	g++ -c $(CFLAGS) $< -o $@

DataGenerator.o: DataGenerator.cpp DataGenerator.h 
	g++ -c $(CFLAGS) $< -o $@

NormalDataGenerator.o: NormalDataGenerator.cpp NormalDataGenerator.h 
	g++ -c $(CFLAGS) $< -o $@

Normal.o: Normal.cpp Normal.h 
	g++ -c $(CFLAGS) $< -o $@

LaplaceDataGenerator.o: LaplaceDataGenerator.cpp LaplaceDataGenerator.h 
	g++ -c $(CFLAGS) $< -o $@

Laplace.o: Laplace.cpp Laplace.h 
	g++ -c $(CFLAGS) $< -o $@

Plot.o: Plot.cpp Plot.h 
	g++ -c $(CFLAGS) $< -o $@

Message.o: Message.cpp Message.h 
	g++ -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o *~ laplace-fit temp/*

