CXX = g++
CXXFLAGS = -Wall -g

driver1: EntryList.o Graph.o mytest.cpp 
	$(CXX) $(CXXFLAGS) EntryList.o Graph.o mytest.cpp -o mytest

Graph.o: Graph.h Graph.cpp EntryList.o
	$(CXX) $(CXXFLAGS) -c Graph.cpp

EntryList.o: EntryList.h EntryList.cpp
	$(CXX) $(CXXFLAGS) -c EntryList.cpp

clean:
	rm *~
	rm *.o

run:
	./driver1

run1:
	./driver2

run2:
	./mytest

val:
	valgrind ./driver1

val1:
	valgrind ./driver2

val2:
	valgrind ./mytest
