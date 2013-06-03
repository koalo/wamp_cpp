LDLIBS+=-ljsoncpp
CXXFLAGS+=-std=c++0x

main: main.o Example.o RPCallable.o

