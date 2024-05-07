import math
import heapq
from typing import List, Tuple, Set, Any


class Astar:
    def __init__(self, width: int, height: int, walls: Set[Tuple[int, int]], start: Tuple[int, int], goal: Tuple[int, int], heuristic: str):
        self.width = width
        self.height = height
        self.walls = walls
        self.start = start
        self.goal = goal
        self.heuristic_type = heuristic
        self.g_scores = {start: 0}
        self.f_scores = {start: self.cell_h(start)}
        self.came_from = {}
        self.open_list = [(self.f_scores[start], start)]
        self.closed_list = set()
        self.explored_nodes = 0

    def cell_h(self, cell: Tuple[int, int]) -> float:
        x = cell[0]
        y = cell[1]
        if self.heuristic_type == "Manhattan":
            return abs(x - self.goal[0]) + abs(y - self.goal[1])
        elif self.heuristic_type == "Euclidean":
            return math.sqrt((x - self.goal[0]) ** 2 + (y - self.goal[1]) ** 2)

    def get_aStar_path(self) -> Set[Tuple[int, int]]:
        while self.open_list:
            _, current = heapq.heappop(self.open_list)
            self.explored_nodes += 1
            if current == self.goal:
                return self.reversePath(current)

            self.closed_list.add(current)

            for neighbor in self.get_neighbors(current):
                if neighbor in self.closed_list:
                    continue

                tentative_g_score = self.g_scores[current] + 1

                if neighbor not in self.g_scores or tentative_g_score < self.g_scores[neighbor]:
                    self.came_from[neighbor] = current
                    self.g_scores[neighbor] = tentative_g_score
                    self.f_scores[neighbor] = tentative_g_score + self.cell_h(neighbor)
                    heapq.heappush(self.open_list, (self.f_scores[neighbor], neighbor))

        return set()


    def get_node_count(self):
        return self.explored_nodes

    def get_neighbors(self, cell: Tuple[int, int]) -> List[Tuple[int, int]]:
        x, y = cell
        directions = [(-1, 0), (0, -1), (1, 0), (0, 1)]

        neighbors = [(x + dx, y + dy) for dx, dy in directions]
        valid_neighbors = [(nx, ny) for nx, ny in neighbors if 0 <= nx < self.width and 0 <= ny < self.height and (nx, ny) not in self.walls]
        return valid_neighbors

    def reversePath(self, current: Tuple[int, int]) -> Set[Tuple[int, int]]:
        total_path = {current}
        while current in self.came_from:
            current = self.came_from[current]
            total_path.add(current)
        return total_path

    def get_lowest_f_Node(self) -> Set[Tuple[int, int]]:
        lowest_f_node = None
        lowest_f_score = float('inf')

        for node in self.closed_list:
            f_val = self.f_scores.get(node, float('inf'))
            if f_val < lowest_f_score:
                lowest_f_score = f_val
                lowest_f_node = node

        if lowest_f_node is not None:
            return self.reversePath(lowest_f_node)
        print(lowest_f_score, ", ", lowest_f_node)
        return set()