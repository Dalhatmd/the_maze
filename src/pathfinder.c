#include "enemy.h"
/**
 * push - pushes a node to priority queue
 *
 * @node: node to be pushed
 */
void push(PriorityQueue *pq, Node node)
{
	int i;

	if (pq->size == pq->capacity)
		return;

	i = pq->size;
	pq->nodes[i] = node;
	pq->size++;

	while (i > 0 && pq->nodes[(i - 1) / 2].f > pq->nodes[i].f)
	{
		Node temp = pq->nodes[(i - 1) / 2];
		pq->nodes[(i - 1) / 2] = pq->nodes[i];
		pq->nodes[i] = temp;
		i = (i - 1) / 2;
	}
}

/** pop - removes(pops) a node from queue
 *
 * @pq: priorityQueue struct
 *
 * Return: popped Node
 */
Node pop(PriorityQueue *pq)
{
	int i, minIndex, left, right;

	Node root = pq->nodes[0];
	pq->nodes[0] = pq->nodes[pq->size - 1];
	pq->size--;

	while (true)
	{
		minIndex = i;
		left = 2 * i + 1;
		right = 2 * i + 2;

		if (left < pq->size && pq->nodes[left].f < pq->nodes[minIndex].f)
			minIndex = left;
		if (right < pq->size && pq->nodes[right].f < pq->nodes[minIndex].f)
			minIndex = right;
		if (minIndex == i)
			break;


		Node temp = pq->nodes[i];
		pq->nodes[i] = pq->nodes[minIndex];
		pq->nodes[minIndex] = temp;
		i = minIndex;
	}

	return root;
}

void enemyPathFinding(RaycasterState* state, Enemy* enemy, double playerX, double playerY) {
    PriorityQueue openSet;
    initPriorityQueue(&openSet, MAP_WIDTH * MAP_HEIGHT);

    bool closedSet[MAP_WIDTH][MAP_HEIGHT] = {false};
    Node nodes[MAP_WIDTH][MAP_HEIGHT] = {0};

    int startX = (int)enemy->posX;
    int startY = (int)enemy->posY;
    int endX = (int)playerX;
    int endY = (int)playerY;

    nodes[startX][startY].x = startX;
    nodes[startX][startY].y = startY;
    nodes[startX][startY].g = 0;
    nodes[startX][startY].h = heuristic(startX, startY, endX, endY);
    nodes[startX][startY].f = nodes[startX][startY].g + nodes[startX][startY].h;

    push(&openSet, nodes[startX][startY]);

    int dx[] = {-1, 0, 1, 0};
    int dy[] = {0, 1, 0, -1};

    while (!isEmpty(&openSet)) {
        Node current = pop(&openSet);

        if (current.x == endX && current.y == endY) {
            /* Path found, reconstruct and move enemy */
            int path[MAX_PATH_LENGTH][2];
            int pathLength = 0;
            reconstructPath(&nodes[current.x][current.y], path, &pathLength);

            if (pathLength > 1) {
                /* Move enemy towards the next point in the path */
                double moveX = path[pathLength - 2][0] - enemy->posX;
                double moveY = path[pathLength - 2][1] - enemy->posY;
                double length = sqrt(moveX * moveX + moveY * moveY);

                if (length > 0) {
                    double moveSpeed = 0.05; // Adjust this value to change enemy speed
                    enemy->posX += (moveX / length) * moveSpeed;
                    enemy->posY += (moveY / length) * moveSpeed;
		    printf("Enemy position: %f, %f\n", enemy->posX, enemy->posY);
                }
            }
            return;
        }
	 closedSet[current.x][current.y] = true;

        for (int i = 0; i < 4; i++) {
            int newX = current.x + dx[i];
            int newY = current.y + dy[i];

            if (!isValidCell(state, newX, newY) || closedSet[newX][newY]) {
                continue;
            }

            double tentativeG = nodes[current.x][current.y].g + 1.0;

            if (tentativeG < nodes[newX][newY].g || nodes[newX][newY].g == 0) {
                nodes[newX][newY].parent = (struct node*)&nodes[current.x][current.y];
                nodes[newX][newY].g = tentativeG;
                nodes[newX][newY].h = heuristic(newX, newY, endX, endY);
               nodes[newX][newY].f = nodes[newX][newY].g + nodes[newX][newY].h;
                nodes[newX][newY].x = newX;
                nodes[newX][newY].y = newY;

                push(&openSet, nodes[newX][newY]);
            }
        }
    }
}

void updateEnemy(RaycasterState *state, Enemy *enemy)
{
	enemyPathFinding(state, enemy, state->posX, state->posY);
}
