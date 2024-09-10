CC = gcc
AS = as
CFLAGS = -Wall -Wextra -O0 -g -m64 -msse4.2
LDFLAGS = -no-pie -lm -g

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

all: main

main: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) main
