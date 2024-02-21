≈çCXX = g++
CXXFLAGS = -Wall -g
OBJECTS = pqueue.o mytest.o

mytest: $(OBJECTS) mytest.cpp
        $(CXX) $(CXXFLAGS) $(OBJECTS) -o mytest

pqueue.o: pqueue.h pqueue.cpp
        $(CXX) $(CXXFLAGS) -c pqueue.cpp

clean:
        rm *.o*
        rm *~

run:
        ./mytest

val:
        valgrind ./mytest

debug:
        gdb gdbarg1 --args ./mytest
