import argparse
import pygame
import pygame_gui
import random
import math
import heapq
import tkinter as tk
from Node import Node
from AstarAlgorithmn2 import solve_aStar
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

        self.mybuttons()
        self.reset_game()

    Coord = Tuple[int, int]

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
                if (col, row) in self.path:
                    pygame.draw.rect(self.screen, (255, 255, 0), rect)

        pygame.display.flip()

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
                elif event.type == pygame.USEREVENT:
                    if event.user_type == pygame_gui.UI_BUTTON_PRESSED:
                        if event.ui_element == self.btn_random_walls:
                            print("Random walls btn pressed")
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
                            wall_cells = self.g_wall_cells
                            current_heuristic = self.Heuristic
                            cell_size = self.cell_size

                            matrix = self.create_maze_matrix(self.width, self.height, wall_cells)
                            solver = solve_aStar(self, start_pos, goal_pos, current_heuristic, wall_cells, matrix)
                            solver.add_open_list(start_pos)

                self.handle_mouse_events(event)
                self.manager.process_events(event)

            self.manager.update(time_delta)
            self.manager.draw_ui(self.screen)
            pygame.display.flip()

        pygame.quit()

    def create_maze_matrix(self, width: int, height: int, g_wall_cells: set[Coord]) -> List[List[bool]]:

        matrix = [[True for _ in range(width)] for _ in range(height)]

        for row, col in g_wall_cells:
            if 0 <= row < height and 0 <= col < width:
                matrix[col][row] = False
        print(matrix)
        return matrix


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
                    print("new wall add : [", col, ", ", row, "]")
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
    # new instance
    parser = argparse.ArgumentParser(description='N x M matrix')

    # input values
    parser.add_argument('--height', type=int, default=30,
                        help='height(N) : Default is 30.')
    parser.add_argument('--width', type=int, default=30,
                        help='width(M) : Default is 30.')

    parser.add_argument('--inc_obstacle_ratio', type=float, default=0.2,
                        help='inc_obstacle_ratio : default: 0.2')

    # args save values
    args = parser.parse_args()

    game = matrix(args.width, args.height, args.inc_obstacle_ratio)
    game.run()

