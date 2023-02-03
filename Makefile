CC = cc
CFLAGS = -Wall -Werror -Wextra -ggdb $(shell pkg-config --cflags ncurses)
LIBS = $(shell pkg-config -libs ncurses)
OUT = wirecroc
SRC = src/main.c

all: build

build: src/main.c
	$(CC) $(CFLAGS) $(LIBS) -o $(OUT) $(SRC)
