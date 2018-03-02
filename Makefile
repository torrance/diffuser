CXX = g++-6
CXXFLAGS = -Wall -Wextra -pedantic -g -O3 -std=c++17 \
					 -I /usr/local/include -L /usr/local/lib \
           -I ~/CSIRO/DNest4/code -L ~/CSIRO/DNest4/code \
					 -lgsl -lgslcblas -ldnest4

default:
	$(CXX) $(CXXFLAGS) -c *.cpp
	$(CXX) $(CXXFLAGS) -o diffuser *.o
	rm *.o
