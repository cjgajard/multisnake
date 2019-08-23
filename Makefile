CC = gcc
CFLAGS = -Wall
CLIBS = -lSDL2 -lSDL2_ttf
DEBUG ?= 0
OBJ = main.o movement.o snake.o game.o renderer.o

ifeq ($(DEBUG), 1)
	CFLAGS += -DDEBUG
else
	CFLAGS += -DNDEBUG
endif

.PHONY: clean build
all: clean build

%.o: %.c
	$(CC) -o $@ $(CFLAGS) -c $<

build: $(OBJ)
	$(CC) -o multisnake $(CFLAGS) $^ $(CLIBS)

clean:
	rm -f *.o
	rm -f multisnake
