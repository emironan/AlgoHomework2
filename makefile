CC = gcc

CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -g

TARGET = app

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

.PHONY: all clean run

INPUT ?= files/test-input-1-tsptw.txt
OUTPUT ?= files/test-output-1-tsptw.txt

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) -lm

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET) $(INPUT) $(OUTPUT)

clean:
	rm -f $(OBJ) $(TARGET)