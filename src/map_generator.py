import random
"""creates a random maze to act as levels"""


def create_base_maze(template):
    return [[cell for cell in row] for row in template]

def find_empty_cells(maze):
    return [(i, j) for i in range(len(maze)) for j in range(len(maze[0])) if maze[i][j] == '0']

def is_valid(maze, x, y):
    return 0 <= x < len(maze) and 0 <= y < len(maze[0]) and maze[x][y] == '0'

def generate_maze(template, start_x, start_y):
    maze = create_base_maze(template)
    stack = [(start_x, start_y)]
    visited = set()

    while stack:
        x, y = stack.pop()
        if (x, y) not in visited:
            visited.add((x, y))
            neighbors = [(x+dx, y+dy) for dx, dy in [(-1,0), (1,0), (0,-1), (0,1)] if is_valid(maze, x+dx, y+dy)]
            random.shuffle(neighbors)
            for nx, ny in neighbors:
                if (nx, ny) not in visited:
                    maze[(x+nx)//2][(y+ny)//2] = '0'  # Remove wall between cells
                    stack.append((nx, ny))

    # Place doors
    empty_cells = find_empty_cells(maze)
    for _ in range(2):  # Place 2 doors
        if empty_cells:
            door_x, door_y = random.choice(empty_cells)
            maze[door_x][door_y] = '1'
            empty_cells.remove((door_x, door_y))

    return maze

def print_maze(maze):
    for row in maze:
        print(''.join(row))

# Your provided template
template = [
        "222222222222222222222",
        "200000000000000000002",
        "202220000222000022202",
        "202020000202000020202",
        "202220000222000022202",
        "200000000000000000002",
        "200000222000333000002",
        "200002000200300300002",
        "200002222200300300002",
        "200002000200300300002",
        "200002000200333000002",
        "2000000000000000000P2",
        "200000000000000000002",
        "200000444000555000002",
        "200004000400500500002",
        "200004444400555550002",
        "200004000400000500002",
        "200004000400000500002",
        "200000000000000000002",
        "266666666666666666662",
        "222222222222222222222"
]

# Generate and print the maze
maze = generate_maze(template, 1, 1)
file = "map.txt"
#with open(file as f):
 #   file.append(maze)
print_maze(maze)
