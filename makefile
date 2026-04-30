CFLAGS= -g -Wall -Werror -pedantic -lcjson -lcurl
FILE=main.c
CC=gcc
TARGET=main

all: $(FILE)
	$(CC) $(FILE) -o $(TARGET) $(CFLAGS) 

gdb: $(FILE)
	gdb -tui ./$(TARGET)
