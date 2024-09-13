FILES=src/init.c src/main.c src/move.c src/raycaster.c src/parser.c src/textures.c src/minimap.c
LINK=-lm -lSDL2 -lSDL2_image
COMP=gcc
FLAGS=-Wall -Werror -Wextra -pedantic
OUTPUT=game

all:
	$(COMP) $(FLAGS) $(FILES) $(LINK) -o $(OUTPUT)

