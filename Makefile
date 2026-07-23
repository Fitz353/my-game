CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lSDL3

all: mygame

mygame: main.c
	$(CC) $(CFLAGS) -o mygame main.c $(LDFLAGS)

clean:
	rm -f mygame
