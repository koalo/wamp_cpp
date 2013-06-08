LDLIBS+=-ljsoncpp -lboost_system
BOOSTROOT=boost
WEBSOCKETPPROOT=websocketpp

LIBOBJ=Server.o MessageHandler.o Directory.o Json.o EventManager.o

CXXFLAGS+=-std=c++0x -I$(WEBSOCKETPPROOT) -I$(BOOSTROOT)/include/ -L$(BOOSTROOT)/lib/

wamp_cpp.a: $(LIBOBJ)
	ar rcs wamp_cpp.a $(LIBOBJ)

main: main.o Example.o 

run: main
	LD_LIBRARY_PATH=$(BOOSTROOT)/lib/ ./main

clean:
	rm *.o main
