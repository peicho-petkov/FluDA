LIBFLUDA_BASE_DIR=libFluDA/local
CXX=g++
CXXFLAGS= -O3 -I. -g $(shell root-config --cflags) -I$(LIBFLUDA_BASE_DIR)/include
LD=g++
LDFLAGS= $(shell root-config --libs) -L$(LIBFLUDA_BASE_DIR)/lib -lFluDA

srcs= simpleanalyzer.cpp dEdx_Calculator.cpp
objs=$(srcs:.cpp=.o)

exename=example

all: $(objs)
	$(LD) -o $(exename) $(objs) $(LDFLAGS)

clean:
	rm -f $(objs) $(exename)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<
