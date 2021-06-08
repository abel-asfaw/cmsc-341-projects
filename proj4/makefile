CXX = g++
CXXFLAGS = -Wall -g

mytest: jqueue.o mytest.cpp
	$(CXX) $(CXXFLAGS) jqueue.o mytest.cpp -o mytest

jqueue.o: jqueue.h jqueue.cpp
	$(CXX) $(CXXFLAGS) -c jqueue.cpp

run:
	./mytest

val:
	valgrind ./mytest

gdb:
	gdb ./mytest
 
clean:
	rm *.o
	# rm *.cpp~