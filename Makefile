CC = cc
CFLAGS = -Wall -Werror -Wextra -ggdb $(pkg-config --cflags ncurses)
LIBS = $(pkg-config -libs ncurses)
OUT = wirecroc
SRC = src/main.c

all: build

build: src/main.c
	$(CC) $(CFLAGS) -o $(OUT) $(SRC) $(LIBS)
