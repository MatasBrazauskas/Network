CC = gcc

CFLAGS = -g -Wall -Wextra -pedantic -O3 #-fsanitize=address
LDLIBS = -lcjson -lcurl

SRC = src/*.c
TARGET = main

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDLIBS)

gdb: $(TARGET)
	gdb -tui ./$(TARGET)

clean: $(TARGET)
	rm -f $(TARGET)

memLeak: $(TARGET)
	valgrind --leak-check=full --verbose --track-origins=yes --show-leak-kinds=all ./$(TARGET)
