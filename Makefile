CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lSDL3 -lSDL3_image

SRC = src/main.c src/game.c src/player.c
OBJ = $(SRC:src/%.c=build/%.o)
EXEC = mygame

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

build/%.o: src/%.c src/game.h | build
	$(CC) $(CFLAGS) -c $< -o $@

build:
	mkdir -p build
	@rm -rf -- -p 2>/dev/null || true

clean:
	rm -rf build $(EXEC)

.PHONY: all clean