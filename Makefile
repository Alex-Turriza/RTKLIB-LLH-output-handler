all:GPSHandler

CXXFLAGS=-g -I ./LLH -std=c++11

GPSHandler: objs/sdev.o objs/llh.o objs/gps.o objs/GPSHandler.o
	$(CXX) $(CXXFLAGS) objs/GPSHandler.o objs/gps.o objs/llh.o objs/sdev.o -o GPSHandler

objs/sdev.o: 
	$(CXX) $(CXXFLAGS) -c -o objs/sdev.o LLH/sdev.cpp

objs/llh.o: 
	$(CXX) $(CXXFLAGS) -c -o objs/llh.o LLH/llh.cpp

objs/gps.o: 
	$(CXX) $(CXXFLAGS) -c -o objs/gps.o LLH/gps.cpp

objs/GPSHandler.o: 
	$(CXX) $(CXXFLAGS) -c -o objs/GPSHandler.o GPSHandler.cpp

clean:
	-rm objs/*.o GPSHandler
