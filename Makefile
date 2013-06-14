# Makefile for wamp_cpp

LIBOBJ=WAMPServer.o MessageHandler.o Directory.o Json.o EventManager.o Topic.o json_reader.o json_value.o json_writer.o

ifndef _ARCH
  _ARCH = $(shell uname -m)
endif


# Cross Compile
ifeq ($(TARGET),arm)
  CROSS_COMPILE_PREFIX=arm-linux-gnueabihf-
  CXX=$(CROSS_COMPILE_PREFIX)g++
  CC=$(CROSS_COMPILE_PREFIX)g++
  CXXFLAGS+=-marm -mfpu=vfp 
  _ARCH = armv6l
endif

#LDLIBS+=-lboost_system
#BOOSTROOT=../boost/_$(_ARCH)
BOOSTROOT=../boost
WEBSOCKETPPROOT=../websocketpp
CXXFLAGS+=-g

LIBTARGET=_$(_ARCH)/libwamp_cpp.a

LIBOBJTARGET=$(patsubst %,_$(_ARCH)/%, $(LIBOBJ))

CXXFLAGS+=-std=c++0x -I$(WEBSOCKETPPROOT) -I$(BOOSTROOT) -Ijsoncpp/ -I. 
#-D_WEBSOCKETPP_CPP11_SYSTEM_ERROR_ -D_WEBSOCKETPP_CPP11_THREAD_
# -D_WEBSOCKETPP_CPP11_STL_

.SECONDEXPANSION:

$(LIBTARGET): $(LIBOBJTARGET) | $${@D}
	ar rcs $(LIBTARGET) $(LIBOBJTARGET) 
	ranlib $(LIBTARGET)

main: main.o Example.o $(LIBOBJTARGET)

run: main
	LD_LIBRARY_PATH=$(BOOSTROOT)/lib/ ./main

clean:
	rm _$(_ARCH)/*.o main $(LIBTARGET)

jsoncpp: $(JSONOBJTARGET)

_$(_ARCH)/json_%.o : jsoncpp/src/json_%.cpp
	$(COMPILE.cpp) $< -o $@

_$(_ARCH)/%.o : %.cpp
	$(COMPILE.cpp) $< -o $@

_$(_ARCH) :
	mkdir -p $@
