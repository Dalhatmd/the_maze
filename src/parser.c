#include "ray.h"
#include <string.h>

#define MAX_LINE_LENGTH 1024

RaycasterState* parseMapFile(const char *filename)
{
	FILE *file = fopen(filename, "r");
	if (!file)
	{
		printf("Error: Could not open file %s\n", filename);
		return NULL;
	}
	RaycasterState *state = malloc(sizeof(RaycasterState));
	if (!state)
	{
		printf("Error: Could not allocate memory for RaycasterState\n");
		fclose(file);
		return NULL;
	}
	char line[MAX_LINE_LENGTH];
	int row = 0;
	int max_width = 0;

	while (fgets(line, sizeof(line), file) && row < MAP_HEIGHT)
	{
		int col = 0;
		for (char *c = line; *c != '\n' && *c != '\0' && col < MAP_WIDTH; c++)
		{
			switch (*c)
			{
		case '0': state->map[col][row] = 0; break;
		case '1': state->map[col][row] = 1; break;

		case '2': state->map[col][row] = 2; break;
		case '3': state->map[col][row] = 3; break; // Special wall type 2
                case '4': state->map[col][row] = 4; break; // Special wall type 3
                case '5': state->map[col][row] = 5; break; // Special wall type 4
                case '6': state->map[col][row] = 6; break; // Special wall type 5
                case '7': state->map[col][row] = 7; break; // Special wall type 6
                case '8': state->map[col][row] = 8; break; // Special wall type 7
                case 'P': // Player start position
                    state->posX = col + 0.5;
                    state->posY = row + 0.5;
                    state->map[col][row] = 0; // Assume player starts on empty space
                    break;
                default:
                    fprintf(stderr, "Unknown character in map: %c\n", *c);
                    state->map[col][row] = 0; // Treat unknown as empty space
            }
            col++;
        }
        if (col > max_width) max_width = col;
        row++;
    }

    fclose(file);

     state->dirX = -1;
    state->dirY = 0;
    state->planeX = 0;
    state->planeY = 0.66;
    state->moveForward = false;
    state->moveBackward = false;
    state->rotateLeft = false;
    state->rotateRight = false;

    // Validate map dimensions
    if (row != MAP_HEIGHT || max_width != MAP_WIDTH) {
        fprintf(stderr, "Invalid map dimensions. Expected %dx%d, got %dx%d\n", 
                MAP_WIDTH, MAP_HEIGHT, max_width, row);
        free(state);
        return NULL;
    }

    return state;
}
