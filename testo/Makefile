CXX=g++
CXXFLAGS=-I. 
LDFLAGS= -lSDL2 -lm 

DEPS = testo.h

OBJ = testo.o

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

testo:  $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)