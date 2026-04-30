CC = gcc

CFLAGS = -g -Wall -Wextra -pedantic
LDLIBS = -lcjson -lcurl

SRC = main.c src/*.c
TARGET = main

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDLIBS)

clean:
	rm -f $(TARGET)
