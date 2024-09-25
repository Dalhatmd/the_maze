# Variables
CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lm
SRC = $(wildcard *.c)  # All .c files in the directory
OBJ = $(SRC:.c=.o)     # Corresponding .o files for each .c file
TARGET = game          # The name of output

# Default target to build the game
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

# Rule to compile .c files into .o object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up the build files
clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: clean
