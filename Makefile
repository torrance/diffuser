CXX = g++-6
CXXFLAGS = -Wall -Wextra -pedantic -g -O3 -std=c++17 \
					 -I /usr/local/include /usr/local/lib/libgsl.a /usr/local/lib/libgslcblas.a \
           -I ~/CSIRO/DNest4/code -L ~/CSIRO/DNest4/code \
					 -ldnest4 -lstdc++fs

default:
	$(CXX) $(CXXFLAGS) -c *.cpp
	$(CXX) $(CXXFLAGS) -o diffuser *.o
	rm *.o

