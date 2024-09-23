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
	if (pq == NULL || pq->size == 0) {
        Node emptyNode = {0};
        return emptyNode;
    }
	int i = 0, minIndex, left, right;

	Node root = pq->nodes[0];
	pq->nodes[0] = pq->nodes[pq->size - 1];
	pq->size--;

	while (i < pq->size)
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
}void enemyPathFinding(RaycasterState* state, __attribute__((unused))Enemy enemy, double playerX, double playerY)
{
    static int updateCounter = 0;
    static double targetX = -1, targetY = -1;
    double moveSpeed = 0.03; // Adjusted for smoother movement

    // Update path less frequently
    if (++updateCounter % 30 == 0 || (targetX == -1 && targetY == -1)) {
        // Full path calculation
        PriorityQueue openSet;
        initPriorityQueue(&openSet, MAP_WIDTH * MAP_HEIGHT);

        bool closedSet[MAP_WIDTH][MAP_HEIGHT] = {false};
        Node nodes[MAP_WIDTH][MAP_HEIGHT] = {0};

        int startX = (int)state->boss.posX;
        int startY = (int)state->boss.posY;
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
                // Path found, reconstruct and set next target
                int path[MAX_PATH_LENGTH][2];
                int pathLength = 0;
                reconstructPath(&nodes[current.x][current.y], path, &pathLength);

                if (pathLength > 1) {
                    targetX = path[pathLength - 2][0];
                    targetY = path[pathLength - 2][1];
                }
                break;
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

    // Move towards the current target point
    if (targetX != -1 && targetY != -1) {
        double dx = targetX - state->boss.posX;
        double dy = targetY - state->boss.posY;
        double distance = sqrt(dx * dx + dy * dy);

        double newPosX = state->boss.posX;
        double newPosY = state->boss.posY;

        if (distance > moveSpeed) {
            newPosX += (dx / distance) * moveSpeed;
            newPosY += (dy / distance) * moveSpeed;
        } else {
            // Reached the target point, clear it
            newPosX = targetX;
            newPosY = targetY;
            targetX = -1;
            targetY = -1;
        }

        // Simple collision detection
        if (isValidCell(state, (int)newPosX, (int)newPosY)) {
            state->boss.posX = newPosX;
            state->boss.posY = newPosY;
        }
        // If collision occurs, we simply don't update the position
    }
}

void updateEnemy(RaycasterState *state)
{
	enemyPathFinding(state, state->boss, state->posX, state->posY);
}
