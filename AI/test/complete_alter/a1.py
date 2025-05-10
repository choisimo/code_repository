import argparse
import pygame
import pygame_gui
import random
import math
import heapq
import tkinter as tk
from a2 import Astar
from typing import List, Tuple, Callable

class matrix:
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

        self.astar = None
        self.mybuttons()
        self.reset_game()

    Coord = Tuple[int, int]

    def reset_game(self):
        self.g_wall_cells = set()
        self.trace_set = set()
        self.path = set()

    def mybuttons(self):

        self.btn_random_walls = pygame_gui.elements.UIButton(relative_rect=pygame.Rect((250, self.screen_size), (100, 50)),
                                                            text='Random walls',
                                                            manager=self.manager)

        self.btn_reset = pygame_gui.elements.UIButton(relative_rect=pygame.Rect((450, self.screen_size), (100, 50)),
                                                      text="Reset",
                                                      manager=self.manager)

        self.btn_start_search = pygame_gui.elements.UIButton(relative_rect=pygame.Rect((50, self.screen_size), (100, 50)),
                                                     text="Solve A*", manager=self.manager)

        self.btn_manhattan = pygame_gui.elements.UIButton(relative_rect=pygame.Rect((600, 175), (100, 50)),
                                                          text='Manhattan',
                                                          manager=self.manager)

        self.btn_euclidean = pygame_gui.elements.UIButton(relative_rect=pygame.Rect((600, 245), (100, 50)),
                                                          text='Euclidean',
                                                          manager=self.manager)



    def draw_grid(self):
        self.screen.fill((255, 255, 255))
        for x in range(0, self.screen_size, self.cell_size):
            pygame.draw.line(self.screen, (200, 200, 200), (x, 0), (x, self.screen_size))
        for y in range(0, self.screen_size, self.cell_size):
            pygame.draw.line(self.screen, (200, 200, 200), (0, y), (self.screen_size, y))

    def draw_cells(self):
        for cell in self.g_wall_cells:
            pygame.draw.rect(self.screen, (169, 169, 169), pygame.Rect(cell[0] * self.cell_size, cell[1] * self.cell_size, self.cell_size, self.cell_size))
        pygame.draw.rect(self.screen, (0, 255, 0), pygame.Rect(self.start_pos[0] * self.cell_size, self.start_pos[1] * self.cell_size, self.cell_size, self.cell_size))
        pygame.draw.rect(self.screen, (255, 0, 0), pygame.Rect(self.goal_pos[0] * self.cell_size, self.goal_pos[1] * self.cell_size, self.cell_size, self.cell_size))
        for cell in self.path:
            pygame.draw.rect(self.screen, (255, 255, 0), pygame.Rect(cell[0] * self.cell_size, cell[1] * self.cell_size, self.cell_size, self.cell_size))
    def random_walls(self):
        self.g_wall_cells = {(random.randint(0, self.width - 1), random.randint(0, self.height - 1)) for _ in range(int(self.width * self.height * self.inc_obstacle_ratio))}
        if self.start_pos in self.g_wall_cells:
            self.g_wall_cells.remove(self.start_pos)
        if self.goal_pos in self.g_wall_cells:
            self.g_wall_cells.remove(self.goal_pos)

    def solve_astar(self):
        self.astar = Astar(self.width, self.height, self.g_wall_cells, self.start_pos, self.goal_pos, self.Heuristic)
        self.path = self.astar.get_aStar_path()
        explored_nodes = self.astar.get_node_count()
        print("explored_nodes : ", explored_nodes)
        if not self.path:
            print("No path found")
            self.path = self.astar.get_lowest_f_Node()

    def handle_mouse_event(self, event):
        x, y = event.pos
        cell_x = x // self.cell_size
        cell_y = y // self.cell_size
        cell = (cell_x, cell_y)
        if self.dragging_target is None:
            if cell == self.start_pos or cell == self.goal_pos:
                self.dragging_target = 'S' if cell == self.start_pos else 'G'
                self.is_dragging = True
                self.prev_dragging_pos = cell
            elif cell in self.g_wall_cells:
                self.g_wall_cells.remove(cell)
            else:
                self.g_wall_cells.add(cell)
        else:
            if self.dragging_target == 'S':
                self.start_pos = cell
            elif self.dragging_target == 'G':
                self.goal_pos = cell
    def main(self):
        is_running = True
        clock = pygame.time.Clock()

        while is_running:
            time_delta = clock.tick(60) / 1000.0
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    is_running = False

                if event.type == pygame.MOUSEBUTTONDOWN:
                    self.handle_mouse_event(event)

                if event.type == pygame.MOUSEBUTTONUP:
                    self.is_dragging = False
                    self.dragging_target = None

                if event.type == pygame.MOUSEMOTION and self.is_dragging:
                    self.handle_mouse_event(event)

                if event.type == pygame_gui.UI_BUTTON_PRESSED:
                    if event.ui_element == self.btn_random_walls:
                        self.random_walls()
                    elif event.ui_element == self.btn_reset:
                        self.reset_game()
                    elif event.ui_element == self.btn_start_search:
                        self.solve_astar()
                    elif event.ui_element == self.btn_manhattan:
                        self.Heuristic = 'Manhattan'
                    elif event.ui_element == self.btn_euclidean:
                        self.Heuristic = 'Euclidean'

                self.manager.process_events(event)

            self.draw_grid()
            self.draw_cells()
            self.manager.update(time_delta)
            self.manager.draw_ui(self.screen)

            pygame.display.update()

if __name__ == "__main__":
    print("python 파일이름.py width height obstacle_ratio 입력하여 실행")
    print("예시 : python test.py 30 30 0.3")
    parser = argparse.ArgumentParser(description="A* Pathfinding")
    parser.add_argument("width", type=int, help="Width (N)")
    parser.add_argument("height", type=int, help="Height (M)")
    parser.add_argument("obstacle_ratio", type=float, help="Ratio of obstacles")
    args = parser.parse_args()

    game = matrix(args.width, args.height, args.obstacle_ratio)
    game.main()
