
fragen : fragen.c
	gcc -Wall -g -o fragen fragen.c `pkg-config gtk+-3.0 --cflags --libs`

clean :
	rm -f fragen
