CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lSDL3 -lSDL3_image

SRC = main.c game.c
OBJ = $(SRC:.c=.o)
EXEC = mygame

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.c game.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)

.PHONY: all clean
