import math


class Node:
    def __init__(self, grid, x_pos, y_pos, wall_cells, current_heuristic):
        self.x_pos = x_pos
        self.y_pos = y_pos
        self.f = 0  # 총 비용 f = g + h
        self.g = 0  # 시작점부터 현재 셀까지의 비용
        self.h = 0  # 현재 셀부터 목표 지점까지의 추정

        self.in_wall = (x_pos, y_pos) in wall_cells
        print("current pos : [", self.x_pos, self.y_pos, "]", " is wall? => ", self.in_wall)
        self.neighbours = set()  # 인접 노드들
        self.previous = None  # 이전 노드
        self.open_list = []
        self.close_list = []

    def add_neighbour(self, grid, max_x, max_y):
        possible_direction = [(1, 0), (-1, 0), (0, 1), (0, -1)]
        for pdx, pdy in possible_direction:
            dx, dy = self.x_pos + pdx, self.y_pos + pdy
            if 0 <= dx <= max_x and 0 <= dy <= max_y:
                if (dx, dy) not in self.wall_cells:
                    self.neighbours.add(grid[dx][dy])

    def heuristic(self, pos, goal, heuristic_type):
        if heuristic_type == "Manhattan":
            return Node.heuristic_manhattan(pos, goal)
        elif heuristic_type == "Euclidean":
            return Node.heuristic_euclidean(pos, goal)

    def heuristic_euclidean(self, pos, goal):
        (x1, y1), (x2, y2) = pos, goal
        return math.sqrt((x1 - x2) ** 2 + (y1 - y2) ** 2)

    def heuristic_manhattan(self, pos, goal):
        (x1, y1), (x2, y2) = pos, goal
        return abs(x1 - x2) + abs(y1 - y2)



    def aStar(self, grid, start, goal, walls, heuristic_type):

        max_x = len(grid) - 1
        max_y = len(grid[0]) - 1

        for row in grid:
            for node in row:
                node.add_neighbour(grid, max_x, max_y)

        startNode = grid[start[0]][start[1]]
        goalNode = grid[goal[0]][goal[1]]

        startNode.g = 0
        startNode.h = self.heuristic(startNode, goalNode, heuristic_type)
        startNode.f = startNode.g + startNode.h

        self.open_list.add(startNode)

        while self.open_list:
            current = min(self.open_list, key=lambda x: x.f)
        # goal 도달하면 current 저장 후 경로 반환
            if current == goalNode:
                path = []
                while current.previous:
                    path.append(current)
                    current = current.previous
                return path[::-1]

            self.open_list.remove(current)
            self.close_list.append(current)

            for neighbor in current.neighbours:
                if neighbor in self.close_list:
                    continue

                temp_g = current.g + 1

                if neighbor not in self.open_list:
                    self.open_list.append(neighbor)
                elif temp_g >= neighbor.g:
                    continue

                neighbor.g = temp_g
                neighbor.h = self.heuristic((neighbor.x_pos, neighbor.y_pos), (goalNode.x_pos, goalNode.y_pos),
                                            heuristic_type)
                neighbor.f = neighbor.g + neighbor.h
                neighbor.previous = current

        return None

