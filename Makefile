LDLIBS+=-ljsoncpp -lboost_system
CXXFLAGS+=-std=c++0x -Iwebsocketpp -Iboost_1_53_0 -Lboostbuild/lib/

main: main.o Example.o RPCallable.o Server.o MessageHandler.o

run: main
	LD_LIBRARY_PATH=boostbuild/lib/ ./main
