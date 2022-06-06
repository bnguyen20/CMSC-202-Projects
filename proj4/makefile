CXX = g++
CXXFLAGS = -Wall -g
mytest: hash.o person.o mytest.cpp
	$(CXX) $(CXXFLAGS) hash.o person.o mytest.cpp -o mytest
hash.o: hash.h hash.cpp
	$(CXX) $(CXXFLAGS) -c hash.cpp
run:
	./mytest
clean:
	-rm -f *~ *.o
val:
	valgrind ./mytest
