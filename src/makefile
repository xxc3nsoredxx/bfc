OUT = ../bin
OBJS = $(OUT)/bfc.o
CC = gcc
CFLAGS = -Wall -Wextra -pedantic-errors -std=c99 -c

$(OUT)/bfc: $(OBJS)
	$(CC) $(OBJS) -o $(OUT)/bfc

$(OUT)/bfc.o: bfc.c bfc.h
	$(CC) $(CFLAGS) bfc.c -o $(OUT)/bfc.o

all: $(OUT)/bfc

clean:
	rm -rf ../bin/
	sh init.sh
