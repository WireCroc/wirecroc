CC = gcc
CFLAGS = -Wall -Werror -Wextra -ggdb $(shell pkg-config --cflags ncurses)
LIBS = -Llibwrc/bin -l:libwrc.so $(shell pkg-config --libs ncurses)
INC = -Ilibwrc/src/
OUT = wirecroc
SRC = src/main.c

all: build

build: src/main.c
	$(CC) $(INC) $(CFLAGS) -o $(OUT) $(SRC) $(LIBS)
