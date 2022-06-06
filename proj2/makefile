CXX = g++
CXXFLAGS = -Wall -g
driver: dtree.o utree.o driver.cpp
	$(CXX) $(CXXFLAGS) dtree.o utree.o driver.cpp -o driver
dtree.o: dtree.h dtree.cpp
	$(CXX) $(CXXFLAGS) -c dtree.cpp
run:
	./driver
clean:
	-rm -f *~ *.o
val:
	valgrind ./driver
