class Node:
    def __init__(self, block, x_location, y_location, cell_size):
        self.blocks = []
        self.block = block
        self.x_location = x_location
        self.y_location = y_location
        self.cell_size = cell_size
        self.type = True

class solve_aStar:
    def __init__(self, block, start_pos, goal_pos, heuristic_type, wall, matrix):
        self.open_list = []
        self.closed_list = set(start_pos)
        self.current = start_pos
        self.parent = set()
        self.g_stack = []
        self.start_pos = start_pos
        self.goal_pos = goal_pos
        self.matrix = matrix
        self.block = block
        self.wall = wall
        print("wall List : ", wall)
        self.heuristic_type = heuristic_type

        self.diagonal_cost = 14
        self.Horizontal_cost = 10

    def cal_direction(self, x_add, y_add):
        if y_add == -1:  # 위쪽으로 이동 (대각선 X)
            # (1, -1) 오른쪽 위 대각선 -> 2
            # (0, -1) 위쪽 -> 1
            # (-1, -1) 왼쪽 위 대각선 -> 0
            return x_add + 1
        elif y_add == 0:  # 수평 이동
            if x_add == -1:  # (-1, 0) 왼쪽 -> 3
                return 3
            else:  # (1, 0) 오른쪽 -> 4
                return 4
        elif y_add == 1:  # 아래쪽으로 이동 (대각선 X)
            # (1, 1) 오른쪽 아래 대각선 -> 7
            # (0, 1) 아래쪽 -> 6
            # (-1, 1) 왼쪽 아래 대각선 -> 5
            return x_add + 6

    def add_open_list(self, block_pos):
        print("start_pos = ", self.start_pos, "goal_pos = ", self.goal_pos)
        x_pos = block_pos[0]
        y_pos = block_pos[1]
        for x_plus in [-1, 0, 1]:
            for y_plus in [-1, 0, 1]:
                if not (x_plus == 0 and y_plus == 0):
                    if x_plus != 0 and y_plus != 0:
                        continue

                    x_forward = x_pos + x_plus
                    y_forward = y_pos + y_plus

                    print("current forward path = ", (x_forward,y_forward))
                    current_path = (x_pos, y_pos)
                    forward_path = (x_forward, y_forward)

                    # 햐당 위치가 valid 한지 체크
                    if self.checkValidPath(x_forward, y_forward, x_plus, y_plus):
                        # open_list 에 있는 경로인지 체크
                        if forward_path in self.open_list:
                            print("already in open_list : ", x_forward, ",", y_forward)
                            self.parent.add(current_path)
                            if self.cell_g(x_plus, y_plus) < self.g_stack[0]:
                                forward = self.cal_direction(x_plus, y_plus)
                                self.parent.add(current_path)

                        else:
                            self.open_list.append(forward_path)



    # 부모 g 값을 기반으로 g 값 계산하기 (수평 +10, 대각선 +14)
    def cell_g(self, x_plus, y_plus):
        if  x_plus == 0 and y_plus == 0:
            return self.Horizontal_cost + self.g_stack[-1]
        else:
            return self.diagonal_cost + self.g_stack[-1]


    def checkValidPath(self, x_forward, y_forward, x_plus, y_plus):
        position = (x_forward, y_forward)
        if position in self.wall:
            return False
        if x_plus == 1 and y_plus == 1:
            if (x_forward - 1, y_forward) in self.wall or (x_forward, y_forward - 1) in self.wall:
                print("wall adjacent: ", x_forward, ",", y_forward)
                return False
        elif x_plus == 1 and y_plus == -1:
            if (x_forward - 1, y_forward) in self.wall or (x_forward, y_forward + 1) in self.wall:
                print("wall adjacent: ", x_forward, ",", y_forward)
                return False
        elif x_plus == -1 and y_plus == 1:
            if (x_forward + 1, y_forward) in self.wall or (x_forward, y_forward - 1) in self.wall:
                print("wall adjacent: ", x_forward, ",", y_forward)
                return False
        elif x_plus == -1 and y_plus == -1:
            if (x_forward + 1, y_forward) in self.wall or (x_forward, y_forward + 1) in self.wall:
                print("wall adjacent: ", x_forward, ",", y_forward)
                return False

        print("valid: ", x_forward, ",", y_forward)
        return True
