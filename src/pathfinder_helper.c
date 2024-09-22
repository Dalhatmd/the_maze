#include "enemy.h"
/**
 * isValidCell - Checks if a cell is valid
 *
 * @state: Raycaster state
 * @x: x coordinate of cell
 * @y: y coordinate of cell
 *
 * Return: True if cell is valid, false otherwise
 */
bool isValidCell(RaycasterState *state, int x, int y)
{
	return x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT
		&& state->map[x][y] == 0;
}

/**
 * reconstructPath - Reconstructs the path from the end node
 *
 * @endNode: End node
 * @path: Path array
 * @pathLength: Length of path
 */
void reconstructPath(Node *endNode, int path[MAX_PATH_LENGTH][2], int *pathLength)
{
	Node *current = endNode;
	*pathLength = 0;

	while (current != NULL && *pathLength < MAX_PATH_LENGTH)
	{
		path[*pathLength][0] = current->x;
		path[*pathLength][1] = current->y;
		(*pathLength)++;
		current = (Node*)current->parent;
	}
}

/**
 * heuristic - Calculates the heuristic value of a node
 *  @x1: x coordinate of node
 *  @y1: y coordinate of node
 *  @x2: x coordinate of target
 *  @y2: y coordinate of target
 *  @return: Heuristic value
 */
double heuristic(int x1, int y1, int x2, int y2)
{
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

/**
 * isEmpty - checks if priorityQueue is empty
 *
 * @pq: import priorityQueue
 * Return: true if empty, false otherwise
 */
bool isEmpty(PriorityQueue *pq)
{
	return pq->size == 0;
}
/**
 * initPriorityQueue - iinitializes priority queue 
 * @pq: priority queue struct
 * @capacity; capacity of pq
 */
void initPriorityQueue(PriorityQueue *pq, int capacity)
{
	pq->nodes = (Node*)malloc(capacity * sizeof(Node));
	if (pq->nodes == NULL)
	{
		fprintf(stderr, "Failed to malloc Priority Queue struct\n");
		exit(EXIT_FAILURE);
	}
	pq->size = 0;
	pq->capacity = capacity;
}
