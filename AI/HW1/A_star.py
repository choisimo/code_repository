import argparse
import pygame

# new instance
parser = argparse.ArgumentParser(description='N x M matrix')

# input values
parser.add_argument('--height', type=int, default=30,
                    help='height(N) : Default is 30.')
parser.add_argument('--width', type=int, default=30,
                    help='width(M) : Default is 30.')

# args save values
args = parser.parse_args()

pygame.init()
window_size = (600, 400)
screen = pygame.display.set_mode(window_size)
# color
# wall
color_gray = (200, 200, 200)
# path
color_white = (255, 255, 255)

screen.fill(color_white)

cell_size = 20
grid_width , grid_height = args.width, args.height
grid_origin = (50, 50)

def draw_grid():
    for y in range(grid_height):
        for x in range(grid_width):
            rect = pygame.Rect(grid_origin[0] + x * cell_size, grid_origin[1] + y * cell_size)

