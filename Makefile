LIBFLUDA_BASE_DIR=libFluDA/local
CXX=g++
CXXFLAGS= -O3 -I. -g $(shell root-config --cflags) -I$(LIBFLUDA_BASE_DIR)/include
LD=g++
LDFLAGS= $(shell root-config --libs) -L$(LIBFLUDA_BASE_DIR)/lib -lFluDA

srcs= simpleanalyzer.cpp dEdx_Calculator.cpp
exmple_src= example.cpp
led_src= simple_dedx_plotter.cpp
objs=$(srcs:.cpp=.o)
exmple_obj=$(exmple_src:.cpp=.o)
led_obj=$(led_src:.cpp=.o)

exename=analyze

all: $(objs) $(exmple_obj) $(led_obj)
	$(LD) -o $(exename) $(objs) $(LDFLAGS)
	$(LD) -o example $(exmple_obj) $(LDFLAGS)
	$(LD) -o simple_led_plotter $(led_obj) $(LDFLAGS)

clean:
	rm -f $(objs) $(exename) $(exmple_obj) $(led_obj)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<
