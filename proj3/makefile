CXX = g++
CXXFLAGS = -Wall -g
driver: rqueue.o  driver.cpp
	$(CXX) $(CXXFLAGS) rqueue.o driver.cpp -o driver
rqueue.o: rqueue.h rqueue.cpp
	$(CXX) $(CXXFLAGS) -c rqueue.cpp
run:
	./driver
clean:
	-rm -f *~ *.o
val:
	valgrind ./driver
