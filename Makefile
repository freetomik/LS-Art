CC = g++

HOSTNAME = $(shell hostname)
ifeq "$(HOSTNAME)" "eva.fit.vutbr.cz"
CC = g++6
endif

CXXOPTIONS = -std=c++11 -static-libstdc++ -Wall -O -fno-builtin -g
all: main

.PHONY: clean

fragen.o : fragen.cpp
	$(CC) $(CXXOPTIONS) -c fragen.cpp -o fragen.o `pkg-config gtk+-3.0 --cflags --libs`
ini.o : ini.cpp
	$(CC) $(CXXOPTIONS) -c ini.cpp -o ini.o `pkg-config gtk+-3.0 --cflags --libs`
lsystem.o : lsystem.cpp lsystem.hpp
	$(CC) $(CXXOPTIONS) -c lsystem.cpp -o lsystem.o `pkg-config gtk+-3.0 --cflags --libs`
turtle.o : turtle.cpp turtle.hpp
	$(CC) $(CXXOPTIONS) -c turtle.cpp -o turtle.o `pkg-config gtk+-3.0 --cflags --libs`
main: ini.o lsystem.o turtle.o fragen.o
	$(CC) $(CXXOPTIONS) ini.o lsystem.o turtle.o fragen.o -o fragen `pkg-config gtk+-3.0 --cflags --libs`

clean :
	rm -f fragen *.o
