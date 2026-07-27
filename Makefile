CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lSDL3 -lSDL3_image

SRC = src/main.c src/game.c
OBJ = $(SRC:src/%.c=build/%.o)
EXEC = mygame

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

build/%.o: src/%.c src/game.h | build
	$(CC) $(CFLAGS) -c $< -o $@

build:
	mkdir -p build

clean:
	rm -f $(OBJ) $(EXEC)

.PHONY: all clean
