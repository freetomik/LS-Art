CC = g++

HOSTNAME = $(shell hostname)
ifeq "$(HOSTNAME)" "eva.fit.vutbr.cz"
CC = g++6
endif

OBJFILES = main.o ini.o gui.o fragen.o lsystem.o turtle.o

CFLAGS = -std=c++11 -static-libstdc++ -Wall -O -fno-builtin -g
PKGCONFIG = `pkg-config gtk+-3.0 --cflags --libs`

%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(PKGCONFIG)

all: main
.PHONY: clean

dep:
	$(CC) -MM *.cpp > dep.list

-include dep.list
# thank you David Martinek, http://www.fit.vutbr.cz/~martinek/clang/make.html

main: $(OBJFILES)
	$(CC) $(CFLAGS) $(OBJFILES) -o fragen $(PKGCONFIG)

clean :
	rm -f fragen *.o
