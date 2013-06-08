LDLIBS+=-ljsoncpp -lboost_system
BOOSTROOT=../boost
WEBSOCKETPPROOT=../websocketpp
CXXFLAGS+=-g

LIBOBJ=Server.o MessageHandler.o Directory.o Json.o EventManager.o Topic.o

CXXFLAGS+=-std=c++0x -I$(WEBSOCKETPPROOT) -I$(BOOSTROOT)/include/ -L$(BOOSTROOT)/lib/

main: main.o Example.o $(LIBOBJ)

libwamp_cpp.a: $(LIBOBJ)
	ar rcs libwamp_cpp.a $(LIBOBJ) 

run: main
	LD_LIBRARY_PATH=$(BOOSTROOT)/lib/ ./main

clean:
	rm *.o main
