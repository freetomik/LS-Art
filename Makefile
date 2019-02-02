CC = g++

ODIR = obj
SDIR = src
BDIR = bin
OUT = $(BDIR)/lsart

_OBJS = main.o ini.o gui.o lsystem.o turtle.o lsgen.o lsrend.o util.o
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

CFLAGS = -std=c++11 -Wall -pedantic -O
# CFLAGS = -std=c++11 -Wall -pedantic -O -fno-builtin -g
PKGCONFIG = `pkg-config gtk+-3.0 --cflags --libs cairo-svg`

$(ODIR)/%.o : $(SDIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(PKGCONFIG)

all: prepare main
.PHONY: clean

# dep:
# 	$(CC) -MM *.cpp > dep.list
#
# -include dep.list

prepare:
	mkdir -p $(ODIR) $(BDIR)

main: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(OUT) $(PKGCONFIG)

clean :
	rm -f $(ODIR)/*.o $(OUT)
