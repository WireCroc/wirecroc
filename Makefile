CC = gcc
CFLAGS = -Wall -Werror -Wextra -ggdb $(shell pkg-config --cflags ncurses)
LIBS = -l:libwrc.so $(shell pkg-config --libs ncurses)
OUT = wirecroc
SRC = src/main.c

all: build

build: src/main.c
	$(CC) $(CFLAGS) -o $(OUT) $(SRC) $(LIBS)
