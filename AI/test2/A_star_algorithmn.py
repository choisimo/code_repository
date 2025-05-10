import math
import heapq
from typing import List, Tuple, Set


Coord = Tuple[int, int]


class Node:
    def __init__(self, pos: Coord, g: int, h: int):
        self.pos = pos
        self.g = g
        self.h = h
        self.f = g + h
        self.parent = None

    def __lt__(self, other):
        return self.f < other.f


class solve_aStar:
    def __init__(self):
        self.row_direction = [-1, 0, 1, 0]
        self.col_direction = [0, 1, 0, -1]


    def is_ok(self, matrix: List[List[bool]], x: int, y: int) -> bool:
        if x < 0 or x >= len(matrix) or y < 0 or y >= len(matrix[0]):
            return False
        return matrix[x][y]


    def a_star(self, matrix: List[List[bool]], start: Coord, goal: Coord, heuristic_type: str) -> Tuple[List[Coord], int]:
        open_list = []
        closed_list = set()

        start_node = Node(start, 0, self.heuristic(start, goal, heuristic_type))
        heapq.heappush(open_list, (start_node.f, start_node))

        while open_list:
            current = heapq.heappop(open_list)[1]

            if current.pos == goal:
                path = []
                while current:
                    path.append(current.pos)
                    current = current.parent
                return path[::-1], len(path) - 1

            closed_list.add(current.pos)

            for direction in range(4):
                row, col = current.pos
                next_pos = (row + self.row_direction[direction], col + self.col_direction[direction])

                if not self.is_ok(matrix, next_pos[0], next_pos[1]):
                    continue
                if next_pos in closed_list:
                    continue

                new_g = current.g + 1
                new_h = self.heuristic(next_pos, goal, heuristic_type)
                new_node = Node(next_pos, new_g, new_h)
                new_node.parent = current
                heapq.heappush(open_list, (new_node.f, new_node))

        return [], -1

    def heuristic(self, pos: Coord, goal: Coord, heuristic_type: str) -> int:
        if heuristic_type == "Manhattan":
            return self.heuristic_manhattan(pos, goal)
        elif heuristic_type == "Euclidean":
            return self.heuristic_euclidean(pos, goal)
        return 0

    def heuristic_euclidean(self, pos: Coord, goal: Coord):
        (x1, y1), (x2, y2) = pos, goal
        return math.sqrt((x1 - x2) ** 2 + (y1 - y2) ** 2)

    def heuristic_manhattan(self, pos: Coord, goal: Coord):
        (x1, y1), (x2, y2) = pos, goal
        return abs(x1 - x2) + abs(y1 - y2)


    def search(self, matrix: List[List[bool]], start_pos: Coord, goal_pos: Coord, heuristic_type: str) -> Tuple[Set[Coord], int]:
        result = self.a_star(matrix, start_pos, goal_pos, heuristic_type)
        path = set(result[0])
        path_length = result[1]
        return path, path_length
