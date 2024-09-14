CC = gcc
AS = as
CFLAGS = -Wall -Wextra -m64 -msse4.2
LDFLAGS = 

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

.PHONY: clean run

a.out: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) a.out

run: a.out
	./a.out
