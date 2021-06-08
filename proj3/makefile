CXX = g++
CXXFLAGS = -Wall

bstTest: BST.o bstTest.cpp
	$(CXX) $(CXXFLAGS) BST.o bstTest.cpp -o bstTest

BST.o: BST.h BST.cpp
	$(CXX) $(CXXFLAGS) -c BST.cpp

run:
	./bstTest

val:
	valgrind ./bstTest

gdb:
	gdb ./bstTest
 
clean:
	rm *.o