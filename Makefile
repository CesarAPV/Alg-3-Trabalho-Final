CC = gcc
CFLAGS = -Wall -Wextra -Iinc

SOURCE = $(wildcard src/*.c)

all: $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) -o main

clean:
	rm -rf main

ARGS ?=

run:
	./main $(ARGS)
