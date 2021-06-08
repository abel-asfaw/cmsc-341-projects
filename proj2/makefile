CXX = g++
CXXFLAGS = -Wall -g
VALFLAGS = -s

mytest: mytest.o mytest.cpp rmqlist.h
	$(CXX) $(CXXFLAGS) mytest.cpp rmqlist.h -o mytest

run:
	./mytest

val:
	valgrind ./mytest

valful:
	valgrind $(VALFLAGS) ./mytest

clean:
	rm *.o*