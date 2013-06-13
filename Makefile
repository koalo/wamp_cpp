LDLIBS+=-ljsoncpp -lboost_system
BOOSTROOT=../boost
WEBSOCKETPPROOT=../websocketpp
CXXFLAGS+=-g

ifndef _ARCH
  _ARCH = $(shell uname -m)
endif

LIBTARGET=_$(_ARCH)/libwamp_cpp.a

LIBOBJ=WAMPServer.o MessageHandler.o Directory.o Json.o EventManager.o Topic.o

CXXFLAGS+=-std=c++0x -I$(WEBSOCKETPPROOT) -I$(BOOSTROOT)/include/ -L$(BOOSTROOT)/lib/

.SECONDEXPANSION:

$(LIBTARGET): $(LIBOBJ) | $${@D}
	ar rcs $(LIBTARGET) $(LIBOBJ) 

main: main.o Example.o $(LIBOBJ)

run: main
	LD_LIBRARY_PATH=$(BOOSTROOT)/lib/ ./main

clean:
	rm *.o main libwamp_cpp.a

_$(_ARCH) :
	mkdir -p $@
