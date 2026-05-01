CC = gcc

CFLAGS = -g -Wall -Wextra -pedantic -O3
LDLIBS = -lcjson -lcurl

SRC = main.c src/*.c
TARGET = main

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDLIBS)

gdb:
	gdb -tui ./main

clean:
	rm -f $(TARGET)
