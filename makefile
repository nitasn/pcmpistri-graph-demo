CC = gcc
AS = as
CFLAGS = -Wall -O0 -g -m64
LDFLAGS = -no-pie -lm -g

all: main

main: main.o hamming.o
	$(CC) $(LDFLAGS) -o $@ $^

main.o: main.c
	$(CC) $(CFLAGS) -c $<

hamming.o: hamming.s
	$(AS) --64 -o $@ $<

clean:
	rm -f *.o main

.PHONY: all clean