import argparse
import pygame
import pygame_gui
import random
import os

current_dir = os.getcwd()
print("현재 작업 디렉토리:", current_dir)

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

pygame.init()
manager = pygame_gui.UIManager((600, 600))

pygame.display.set_caption("N x M matrix star-algorithmn")
M, N = args.width, args.height
screen_size = 600
cell_size = screen_size // max(M, N)
screen = pygame.display.set_mode((screen_size, screen_size + 50))

g_wall_cells = {(random.randint(0, M-1), random.randint(0, N-1)) for _ in range(int(M * N * args.inc_obstacle_ratio))}

# Start, Goal position _ random
start_pos = (random.randint(0, M-1), random.randint(0, N-1))
print("start_pos : ", start_pos)
goal_pos = (random.randint(0, M-1), random.randint(0, N-1))
print("goal_pos : ", goal_pos)
# status of dragging
is_dragging = False
dragging_target = None


trace_set = set()
def draw_grid(wall_cells):
    screen.fill((255, 255, 255))
    for row in range(M):
        for col in range(N):
            rect = pygame.Rect(col*cell_size, row*cell_size, cell_size, cell_size)
            pygame.draw.rect(screen, (255, 255, 255), rect, 1)
            if (row, col) in wall_cells:
                pygame.draw.rect(screen, (128, 128, 128), rect)
            if (col,row) == start_pos:
                pygame.draw.rect(screen, (0, 255, 0),  rect)
            if (col,row) == goal_pos:
                pygame.draw.rect(screen, (255, 0, 0), rect)
            if (row, col) in trace_set:
                pygame.draw.rect(screen, (255,255,0), rect)
    pygame.display.flip()


def random_walls_btn():
    global g_wall_cells
    draw_grid(g_wall_cells)


def reset_btn():
    global g_wall_cells
    g_wall_cells = set()
    draw_grid(g_wall_cells)


btn_random_walls = pygame_gui.elements.UIButton(relative_rect=pygame.Rect((0, screen_size+50), (100, 50)),
                                                text='Random walls',
                                                manager=manager)
btn_reset = pygame_gui.elements.UIButton(relative_rect=pygame.Rect((105, screen_size+50), (100, 50)),
                                         text='Reset',
                                         manager=manager)

running = True
draw_grid(set())
clock = pygame.time.Clock()

while running:
    time_delta = clock.tick(60) / 1000.0
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.USEREVENT:
            if event.user_type == pygame_gui.UI_BUTTON_PRESSED:
                if event.ui_element == btn_random_walls:
                    print("Random walls pressed")
                    random_walls_btn()
                elif event.ui_element == btn_reset:
                    print("Reset pressed")
                    reset_btn()
        elif event.type == pygame.MOUSEBUTTONDOWN:
            pos = pygame.mouse.get_pos()
            print("mousebutton pressed (current position) : ", pos)
            col = pos[0] // cell_size
            row = pos[1] // cell_size

            if (col, row) == start_pos:
                print("start pos, set is_dragging : true ", col, row)
                is_dragging = True
                dragging_target = 'S'
            elif (col, row) == goal_pos:
                print("goal pos ", col, row)
                is_dragging = True
                dragging_target = 'G'
            else:
                if (row, col) in g_wall_cells:
                    g_wall_cells.remove((row, col))
                else:
                    g_wall_cells.add((row, col))
                draw_grid(g_wall_cells)

        elif event.type == pygame.MOUSEBUTTONUP:
            is_dragging = False
            dragging_target = ''
        elif is_dragging and event.type == pygame.MOUSEMOTION:
            pos = pygame.mouse.get_pos()
            col = pos[0] // cell_size
            row = pos[1] // cell_size
            trace_set.add((row, col))
            if dragging_target == 'S':
                start_pos = (col, row)
            elif dragging_target == 'G':
                goal_pos = (col, row)
            draw_grid(g_wall_cells)

        manager.process_events(event)
    manager.update(time_delta)
    manager.draw_ui(screen)
    pygame.display.flip()
pygame.quit()
