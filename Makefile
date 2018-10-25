CC = gcc
CFLAGS = -Wall -Werror
OUTPUT = mycopy.o
DEBUG = -g
SOURCES = mycopy.c

all:
	$(CC) $(SOURCES) $(CFLAGS) -o $(OUTPUT)

debug:
	$(CC) $(SOURCES) $(CFLAGS) $(DEBUG) -o $(OUTPUT)

clean:
	rm -f $(OUTPUT)

fresh:
	make clean
	make all

