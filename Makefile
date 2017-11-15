CXXOPTIONS = -std=c++11 -Wall -O -fno-builtin

fragen : fragen.cpp
	g++ $(CXXOPTIONS) -g -o fragen fragen.cpp `pkg-config gtk+-3.0 --cflags --libs`

clean :
	rm -f fragen
