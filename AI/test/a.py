import argparse
import pygame
import pygame_gui
import random
import math
import heapq
from typing import List, Tuple

Coord = Tuple[int, int]

class Node:
    def __init__(self, x_pos, y_pos, goal_pos, wall_cells, heuristic_type):
        self.x_pos = x_pos
        self.y_pos = y_pos
        self.f = 0
        self.g = 0
        self.h = self.calculate_heuristic(goal_pos, heuristic_type)
        self.in_wall = (x_pos, y_pos) in wall_cells
        self.neighbours = set()
        self.previous = None

    def calculate_heuristic(self, goal_pos, heuristic_type):
        if heuristic_type == 'Manhattan':
            return abs(self.x_pos - goal_pos[0]) + abs(self.y_pos - goal_pos[1])
        elif heuristic_type == 'Euclidean':
            return math.sqrt((self.x_pos - goal_pos[0]) ** 2 + (self.y_pos - goal_pos[1]) ** 2)
        return 0

    def __lt__(self, other):
        return self.f < other.f

    @staticmethod
    def a_star(grid: List[List['Node']], start_node: 'Node', goal_node: 'Node', wall_cells: set, heuristic_type: str) -> List[Coord]:
        open_list = []
        closed_list = set()
        heapq.heappush(open_list, (start_node.f, start_node))

        while open_list:
            _, current_node = heapq.heappop(open_list)

            if current_node == goal_node:
                path = []
                while current_node:
                    path.append((current_node.y_pos, current_node.x_pos))
                    current_node = current_node.previous
                return path[::-1]

            closed_list.add(current_node)

            for neighbour in current_node.neighbours:
                if neighbour in closed_list or neighbour.in_wall:
                    continue

                tentative_g_score = current_node.g + 1
                if tentative_g_score < neighbour.g or neighbour not in [i[1] for i in open_list]:
                    neighbour.previous = current_node
                    neighbour.g = tentative_g_score
                    neighbour.f = neighbour.g + neighbour.h
                    heapq.heappush(open_list, (neighbour.f, neighbour))

        return None


class Matrix:
    def __init__(self, width, height, inc_obstacle_ratio):
        self.width, self.height = width, height
        self.inc_obstacle_ratio = inc_obstacle_ratio
        self.g_wall_cells = set()
        self.start_pos = (random.randint(0, width - 1), random.randint(0, height - 1))
        self.goal_pos = (random.randint(0, width - 1), random.randint(0, height - 1))
        self.first_start_pos = self.start_pos
        self.first_goal_pos = self.goal_pos
        self.prev_dragging_pos = (self.start_pos[0], self.start_pos[1])
        self.is_dragging = False
        self.dragging_target = None
        self.trace_set = set()
        self.screen_size = 600
        self.cell_size = self.screen_size // max(self.width, self.height)
        self.Heuristic = 'Manhattan'
        self.path = set()
        pygame.init()
        self.manager = pygame_gui.UIManager((800, 650))
        self.screen = pygame.display.set_mode((self.screen_size + 100, self.screen_size + 50))
        pygame.display.set_caption("M x N matrix A star-algorithmn")

        self.mybuttons()
        self.reset_game()

    def reset_game(self):
        self.g_wall_cells = set()
        self.trace_set = set()
        self.path = set()

    def mybuttons(self):
        self.btn_random_walls = pygame_gui.elements.UIButton(
            relative_rect=pygame.Rect((250, self.screen_size), (100, 50)),
            text='Random walls',
            manager=self.manager)
        self.btn_reset = pygame_gui.elements.UIButton(relative_rect=pygame.Rect((400, self.screen_size), (100, 50)),
                                                      text="Reset",
                                                      manager=self.manager)
        self.btn_start_search = pygame_gui.elements.UIButton(
            relative_rect=pygame.Rect((50, self.screen_size), (150, 50)),
            text="Start A* Search",
            manager=self.manager)

        self.btn_manhattan = pygame_gui.elements.UIButton(relative_rect=pygame.Rect((600, 175), (100, 50)),
                                                          text='Manhattan',
                                                          manager=self.manager)
        self.btn_euclidean = pygame_gui.elements.UIButton(relative_rect=pygame.Rect((600, 245), (100, 50)),
                                                          text='Euclidean',
                                                          manager=self.manager)

    def draw_grid(self):
        self.screen.fill((255, 255, 255))
        for row in range(self.height):
            for col in range(self.width):

                rect = pygame.Rect(col * self.cell_size, row * self.cell_size, self.cell_size, self.cell_size)
                pygame.draw.rect(self.screen, (150, 150, 150), rect, 1)

                if (row, col) in self.g_wall_cells:
                    pygame.draw.rect(self.screen, (128, 128, 128), rect)
                if (col, row) == self.start_pos:
                    pygame.draw.rect(self.screen, (0, 255, 0), rect)
                if (col, row) == self.goal_pos:
                    pygame.draw.rect(self.screen, (255, 0, 0), rect)
                if (row, col) in self.trace_set:
                    pygame.draw.rect(self.screen, (150, 255, 0), rect)
                if (row, col) in self.path:
                    pygame.draw.rect(self.screen, (255, 255, 0), rect)

        pygame.display.flip()

    def create_matrix(self):
        matrix = [[True for _ in range(self.width)] for _ in range(self.height)]
        for row, col in self.g_wall_cells:
            matrix[row][col] = False
        return matrix

    def run(self):
        running = True
        clock = pygame.time.Clock()

        while running:
            if self.start_pos == self.goal_pos:
                self.screen.fill((255, 255, 255))
                pygame.display.flip()

            time_delta = clock.tick(60) / 1000.0

            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False
                elif event.type == pygame_gui.UI_BUTTON_PRESSED:
                    if event.ui_element == self.btn_random_walls:
                        self.reset_game()
                        while len(self.g_wall_cells) < int(self.height * self.width * self.inc_obstacle_ratio):
                            rand_pos = (random.randint(0, self.width - 1), random.randint(0, self.height - 1))
                            if rand_pos != self.start_pos and rand_pos != self.goal_pos:
                                self.g_wall_cells.add(rand_pos)
                        self.draw_grid()
                    elif event.ui_element == self.btn_reset:
                        self.g_wall_cells = set()
                        self.reset_game()
                        self.draw_grid()
                    elif event.ui_element == self.btn_manhattan:
                        self.Heuristic = 'Manhattan'
                    elif event.ui_element == self.btn_euclidean:
                        self.Heuristic = 'Euclidean'
                    elif event.ui_element == self.btn_start_search:
                        start_pos = self.start_pos
                        goal_pos = self.goal_pos

                        grid = [[Node(x, y, goal_pos, self.g_wall_cells, self.Heuristic)
                                 for x in range(self.width)] for y in range(self.height)]

                        for y in range(self.height):
                            for x in range(self.width):
                                node = grid[y][x]
                                node.neighbours = {grid[ny][nx] for nx, ny in [(x - 1, y), (x + 1, y), (x, y - 1), (x, y + 1)]
                                                   if 0 <= nx < self.width and 0 <= ny < self.height and (nx, ny) not in self.g_wall_cells}

                        start_node = grid[start_pos[1]][start_pos[0]]
                        goal_node = grid[goal_pos[1]][goal_pos[0]]

                        path = Node.a_star(grid, start_node, goal_node, self.g_wall_cells, self.Heuristic)

                        if path is not None:
                            self.path = {(node[0], node[1]) for node in path}
                            self.draw_grid()
                            all_path = self.create_matrix()
                            for node in path:
                                all_path[node[0]][node[1]] = True
                            for row in all_path:
                                print(row)
                        else:
                            print("No valid path found.")

                self.handle_mouse_events(event)
                self.manager.process_events(event)

            self.manager.update(time_delta)
            self.manager.draw_ui(self.screen)
            pygame.display.flip()

        pygame.quit()

    def handle_mouse_events(self, event):
        if event.type == pygame.MOUSEBUTTONDOWN:
            pos = pygame.mouse.get_pos()
            col = pos[0] // self.cell_size
            row = pos[1] // self.cell_size

            if (col, row) == self.start_pos:
                self.is_dragging = True
                self.dragging_target = 'S'
                self.prev_dragging_pos = self.start_pos
            elif (col, row) == self.goal_pos:
                self.is_dragging = True
                self.dragging_target = 'G'
                self.prev_dragging_pos = self.goal_pos
            else:
                if (row, col) in self.g_wall_cells:
                    self.g_wall_cells.remove((row, col))
                else:
                    self.g_wall_cells.add((row, col))
                    self.prev_dragging_pos = (row, col)
                self.draw_grid()
        elif event.type == pygame.MOUSEBUTTONUP:
            self.is_dragging = False
            self.dragging_target = ''
        elif self.is_dragging and event.type == pygame.MOUSEMOTION:
            pos = pygame.mouse.get_pos()
            col = pos[0] // self.cell_size
            row = pos[1] // self.cell_size

            if not (row, col) in self.g_wall_cells and self.prev_dragging_pos and (
                    abs(self.prev_dragging_pos[0] - col) + abs(self.prev_dragging_pos[1] - row) == 1):
                if self.dragging_target == 'S':
                    self.start_pos = (col, row)
                elif self.dragging_target == 'G':
                    self.goal_pos = (col, row)
                self.prev_dragging_pos = (col, row)
            self.draw_grid()


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='N x M matrix')

    parser.add_argument('--height', type=int, default=30,
                        help='height(N) : Default is 30.')
    parser.add_argument('--width', type=int, default=30,
                        help='width(M) : Default is 30.')

    parser.add_argument('--inc_obstacle_ratio', type=float, default=0.2,
                        help='inc_obstacle_ratio : default: 0.2')

    args = parser.parse_args()

    game = Matrix(args.width, args.height, args.inc_obstacle_ratio)
    game.run()
