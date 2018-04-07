CC = gcc
CFLAGS = -Wall
LIBS  = -lSDL2

ASSETS_DIR := static
OUT_DIR := bin
OUT_NAME = ${OUT_DIR}/main
OBJ := main.o movement.o snake.o game.o
SRC = $(OBJ:.o=.c)

TEST := minunit

.PHONY: auto directories build

auto: clean directories build

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

directories:
	mkdir -p $(OUT_DIR)
	mkdir -p $(ASSETS_DIR)

build: $(OBJ)
	$(CC) -o $(OUT_NAME) $^ $(LIBS) $(CFLAGS)

test: test_$(TEST).c $(DEPS)
	gcc $^ -o $@ && ./$@ || true
	-@rm $@

clean:
	-@rm -f *.o
	-@rm -f $(OUT_NAME)
