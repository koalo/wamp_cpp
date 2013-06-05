LDLIBS+=-ljsoncpp -lboost_system
BOOSTROOT=boost
WEBSOCKETPPROOT=websocketpp

CXXFLAGS+=-std=c++0x -I$(WEBSOCKETPPROOT) -I$(BOOSTROOT)/include/ -L$(BOOSTROOT)/lib/

main: main.o Example.o Server.o MessageHandler.o Directory.o Json.o EventManager.o

run: main
	LD_LIBRARY_PATH=$(BOOSTROOT)/lib/ ./main

clean:
	rm *.o main
