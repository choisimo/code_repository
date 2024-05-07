import math
import heapq
from typing import List, Tuple


Coord = Tuple[int, int]


class Node:


    def __init__(self, pos: Coord, g: float, h: float):

        self.pos = pos
        self.g = g
        self.h = h
        self.f = g + h
        self.parent = None

    def __lt__(self, other):
        """
        Compare nodes based on the total cost (f = g + h).
        """
        return self.f < other.f


class solve_aStar:
    """
    A class implementing the A* algorithm.
    """

    def __init__(self):
        """
        Initialize the solver with movement directions.
        """
        # Directions for moving up, right, down, left, and diagonals
        self.directions = [
            (-1, 0), (0, 1), (1, 0), (0, -1),
            (-1, -1), (-1, 1), (1, -1), (1, 1)
        ]

    def a_star(self, matrix: List[List[bool]], start: Coord, goal: Coord, heuristic_type: str) -> Tuple[List[Coord], float]:
        """
        Perform the A* algorithm to find the optimal path from start to goal.

        :param matrix: The grid represented as a 2D list of booleans, where True is open and False is blocked.
        :param start: The starting coordinate as (row, col).
        :param goal: The goal coordinate as (row, col).
        :param heuristic_type: The type of heuristic to use ('Manhattan' or 'Euclidean').
        :return: A tuple containing the path as a list of coordinates and the length of the path.
        """
        # Initialize the open list (priority queue) and closed set
        open_list = []
        closed_set = set()

        # Create the starting node
        start_node = Node(start, 0, self.heuristic(start, goal, heuristic_type))
        heapq.heappush(open_list, start_node)

        while open_list:
            # Get the node with the lowest f value
            current = heapq.heappop(open_list)

            # Check if we've reached the goal
            if current.pos == goal:
                path = []
                while current:
                    path.append(current.pos)
                    current = current.parent
                return path[::-1], len(path) - 1

            closed_set.add(current.pos)

            # Explore neighbors
            for direction in self.directions:
                row, col = current.pos
                next_pos = (row + direction[0], col + direction[1])

                # Check if next position is within bounds and open
                if not (0 <= next_pos[0] < len(matrix) and 0 <= next_pos[1] < len(matrix[0])):
                    continue
                if not matrix[next_pos[0]][next_pos[1]]:
                    continue
                if next_pos in closed_set:
                    continue

                new_g = current.g + (1 if direction in self.directions[:4] else math.sqrt(2))
                new_h = self.heuristic(next_pos, goal, heuristic_type)
                new_node = Node(next_pos, new_g, new_h)
                new_node.parent = current
                heapq.heappush(open_list, new_node)

        # Return an empty path and -1 if no path is found
        return [], -1

    def heuristic(self, pos: Coord, goal: Coord, heuristic_type: str) -> float:
        """
        Calculate the heuristic cost from pos to goal.

        :param pos: The current position as (row, col).
        :param goal: The goal position as (row, col).
        :param heuristic_type: The type of heuristic ('Manhattan' or 'Euclidean').
        :return: The heuristic cost.
        """
        if heuristic_type == "Manhattan":
            return self.heuristic_manhattan(pos, goal)
        elif heuristic_type == "Euclidean":
            return self.heuristic_euclidean(pos, goal)
        return 0

    def heuristic_manhattan(self, pos: Coord, goal: Coord) -> int:
        """
        Calculate the Manhattan distance.

        :param pos: The current position as (row, col).
        :param goal: The goal position as (row, col).
        :return: The Manhattan distance.
        """
        x1, y1 = pos
        x2, y2 = goal
        return abs(x1 - x2) + abs(y1 - y2)

    def heuristic_euclidean(self, pos: Coord, goal: Coord) -> float:
        """
        Calculate the Euclidean distance.

        :param pos: The current position as (row, col).
        :param goal: The goal position as (row, col).
        :return: The Euclidean distance.
        """
        x1, y1 = pos
        x2, y2 = goal
        return math.sqrt((x1 - x2) ** 2 + (y1 - y2) ** 2)


# Example use case
if __name__ == "__main__":
    matrix = [[False, True, True, True, True, True, True, True, True, True, False, True, True, True, True, True, False, True, True, False, False, True, True, True, True, True, False, True, False, True], [True, True, True, True, True, True, True, True, True, True, False, True, True, True, True, True, True, True, True, True, True, True, True, True, False, True, False, False, False, True], [True, True, True, False, False, True, True, True, True, True, False, True, True, True, False, True, False, False, True, True, True, True, True, True, True, True, True, True, True, True], [True, True, True, True, True, True, True, True, True, True, True, True, True, True, True, True, False, True, False, True, True, True, True, True, True, True, True, False, True, True], [True, True, True, True, True, True, True, False, True, True, True, True, True, True, True, True, True, True, True, True, True, True, True, True, True, True, True, True, True, True], [True, True, True, True, False, True, False, True, True, True, True, False, False, True, True, True, True, True, True, True, True, False, True, False, True, True, True, True, True, True], [True, True, True, True, True, True, False, True, True, True, True, False, True, True, False, False, True, True, True, True, True, True, False, True, True, False, True, True, True, True], [True, False, False, False, True, True, True, True, True, False, False, False, False, False, True, True, True, True, False, True, True, True, False, True, True, True, True, True, True, True], [True, True, True, True, False, True, True, True, True, True, True, True, True, True, True, True, False, True, True, False, True, False, True, True, True, True, True, True, True, True], [True, True, True, False, True, True, True, False, True, False, True, True, True, False, False, True, False, True, True, True, False, True, True, True, True, True, True, False, True, True], [True, True, False, False, True, True, True, True, True, True, True, True, True, True, True, True, True, True, True, True, True, False, True, True, True, False, True, True, False, True], [False, True, False, True, True, True, True, True, True, True, False, False, True, True, False, True, True, False, True, True, True, True, True, True, True, False, True, True, True, True], [True, True, False, True, True, True, True, True, True, True, True, True, True, True, True, False, True, True, False, True, False, True, True, True, True, False, True, True, False, True], [True, False, True, True, True, True, False, True, True, True, True, True, False, True, True, True, True, True, True, True, True, False, True, True, True, True, True, True, True, True], [True, True, True, True, False, True, True, True, True, True, False, False, True, True, False, True, True, False, False, True, False, True, True, False, True, True, True, False, True, True], [True, False, True, True, False, False, True, False, True, True, True, True, False, True, True, True, True, True, True, True, False, True, False, True, False, True, True, True, True, True], [True, True, True, True, True, True, False, True, True, True, False, True, False, True, False, True, False, True, True, True, True, True, True, True, True, True, True, True, True, True], [True, False, True, False, True, True, True, True, True, True, True, True, True, False, True, True, True, True, True, True, True, True, True, True, True, True, True, True, True, True], [True, True, True, True, False, True, True, True, True, True, False, True, True, True, False, True, True, False, True, False, False, True, False, True, False, True, True, True, True, False], [True, True, True, True, True, True, True, False, False, True, True, True, True, True, True, True, True, True, True, True, True, True, True, True, True, False, False, True, False, True], [False, True, True, False, True, True, True, True, True, True, True, True, True, True, True, True, True, True, False, False, True, False, True, False, True, False, False, True, False, True], [False, True, False, True, True, True, False, False, True, True, False, True, True, True, False, False, False, True, True, True, True, True, False, True, True, True, True, True, True, False], [True, True, True, True, True, True, False, True, True, False, True, True, True, True, True, True, True, True, True, True, True, True, False, True, True, True, True, True, True, True], [True, False, False, True, True, True, True, True, True, True, True, True, False, True, False, True, True, False, False, True, False, True, True, False, True, True, True, True, True, True], [True, True, True, True, True, True, True, True, False, True, True, True, True, True, True, True, True, True, True, True, False, True, True, True, True, False, True, True, True, True], [False, True, True, True, True, True, True, True, True, True, True, True, True, True, True, False, True, False, False, True, True, True, False, True, True, True, True, True, False, True], [True, True, True, True, True, False, False, True, True, True, True, True, True, True, True, True, False, True, False, True, False, True, True, True, True, True, False, True, True, True], [True, True, False, True, True, True, True, True, False, True, True, True, True, True, True, False, True, True, False, True, True, True, True, False, True, True, True, True, True, True], [True, False, True, True, True, True, True, True, True, True, True, True, True, True, True, True, True, True, True, False, True, True, True, True, True, False, True, True, False, False], [True, True, True, True, True, True, True, True, False, True, True, False, False, True, False, True, True, True, False, True, True, True, True, True, False, True, False, False, True, True]]


    start_pos = (0, 0)
    goal_pos = (6, 6)
    solver = solve_aStar()

    # Test with Manhattan heuristic
    manhattan_result = solver.a_star(matrix, start_pos, goal_pos, 'Manhattan')
    print("Manhattan Path:", manhattan_result[0])
    print("Manhattan Path length:", manhattan_result[1])

    # Test with Euclidean heuristic
    euclidean_result = solver.a_star(matrix, start_pos, goal_pos, 'Euclidean')
    print("Euclidean Path:", euclidean_result[0])
    print("Euclidean Path length:", euclidean_result[1])
