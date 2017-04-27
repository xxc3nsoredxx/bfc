OBJS = bfc.o
CC = gcc
CFLAGS = -Wall -Wextra -pedantic-errors -std=c99 -c

bfc: $(OBJS)
	$(CC) $(OBJS) -o bfc

bfc.o: bfc.c bfc.h
	$(CC) $(CFLAGS) bfc.c

all: bfc
