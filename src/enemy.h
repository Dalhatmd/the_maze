#ifndef ENEMY_H
#define ENEMY_H

#include "ray.h"
#include <math.h>
#include <stdbool.h>

#define MAX_PATH_LENGTH 1000

typedef struct 
{
	int x, y;
	double f, g, h;
	struct node *parent;
} Node;


typedef struct
{
	Node *nodes;
	int size;
	int capacity;
} PriorityQueue;

void initPriorityQueue(PriorityQueue *pq, int capacity);
void push(PriorityQueue *pq, Node node);
Node pop(PriorityQueue *pq);
bool isEmpty(PriorityQueue *pq);
double heuristic(int x1, int y1, int x2, int y2);
bool isValidCell(RaycasterState *state, int x, int y);
void reconstructPath(Node *endNode, int path[MAX_PATH_LENGTH][2], int *pathLength);
void enemyPathFinding(RaycasterState *state, Enemy* enemy, double playerX, double playerY);
void updateEnemy(RaycasterState *state, Enemy *enemy);
#endif /*"ENEMY_H" */
