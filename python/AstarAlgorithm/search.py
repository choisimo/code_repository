cost_1 = 10
cost_2 = 14

class A_star:

    def __init__(self, screen, width):
        self.open_list = []
        self.close_list = []
        self.distance = True  # 0 is manhattan 1 is euclid
        self.width = width
        self.screen = screen
        self.over = False

        start_block = screen.start_block_index
        self.cur_block = start_block

        self.screen.blocks[start_block].set_g(0)
        self.screen.blocks[start_block].set_close()
        self.close_list.append(start_block)
        self.add_open_list(start_block)

    # 주어진 x, y 변화량에 따라 방향을 계산한다
    def cal_direction(self, x_plus, y_plus):
        if y_plus == -1:
            return x_plus + 1
        elif y_plus == 0:
            return 3 if x_plus == -1 else 4
        elif y_plus == 1:
            return x_plus + 6

    # 블록 주변의 유효한 블록들을 열린 목록에 추가하고,
    # 각 블록의 g, h 값을 계산하여 설정한다.
    def add_open_list(self, block_index):
        # block_index == start position
        block = self.screen.blocks[block_index]
        x = block.x
        y = block.y
        for x_plus in [-1, 0, 1]:
            for y_plus in [-1, 0, 1]:
                if not (x_plus == 0 and y_plus == 0):
                    if x_plus!= 0 and y_plus!=0:
                        continue

                    x_temp = x + x_plus
                    y_temp = y + y_plus

                    temp_index = y_temp * self.width + x_temp

                    if self.is_ok(x_temp, y_temp, x_plus, y_plus):
                        if temp_index in self.open_list:
                            old_father = self.screen.blocks[block_index].father
                            '''
                                def set_father(self, father, direction):
                                self.father = father
                                if direction != -1:
                                    self.father_direction = direction
                            '''
                            self.screen.blocks[temp_index].set_father(block_index, -1)
                            new_g = self.cal_g(temp_index, x_plus, y_plus)
                            if new_g < self.screen.blocks[temp_index].g:
                                self.screen.blocks[temp_index].set_g(new_g)
                                direction = self.cal_direction(x_plus, y_plus)
                                self.screen.blocks[temp_index].set_father(block_index, direction)
                            else:
                                self.screen.blocks[temp_index].set_father(old_father, -1)
                        else:
                            self.open_list.append(temp_index)
                            self.screen.blocks[temp_index].set_open() # 색상 변경

                            direction = self.cal_direction(x_plus, y_plus)
                            self.screen.blocks[temp_index].set_father(block_index, direction)

                            g = self.cal_g(temp_index, x_plus, y_plus)
                            h = self.cal_h(x_temp, y_temp)
                            self.screen.blocks[temp_index].set_h(h)
                            self.screen.blocks[temp_index].set_g(g)

                        if temp_index == self.screen.end_block_index:
                            self.over = True
                            print('Search Complete')
                            print('All explored nodes : ' + str(len(self.close_list)+len(self.open_list)))

    def cal_g(self, block_index, x_plus, y_plus):
        father_index = self.screen.blocks[block_index].father
        father_g = self.screen.blocks[father_index].g
        if x_plus == 0 or y_plus == 0:
            return father_g + cost_1
        else:
            return father_g + cost_2

    def man_distance(self, x1, y1, x2, y2):
        return (abs(x2 - x1) + abs(y2 - y1)) * cost_1
    def euc_distance(self, x1, y1, x2, y2):
        return ((x2 - x1)**2 + (y2 - y1)**2)**0.5 * cost_1

    def set_distance(self, dist):
        if dist == 0:  # man
            self.distance =True
        else:  # euc
            self.distance = False
    def cal_h(self, x, y):
        end_block_index = self.screen.end_block_index
        end_block = self.screen.blocks[end_block_index]

        end_x = end_block.x
        end_y = end_block.y

        if self.distance ==True:
            return self.man_distance(x, y, end_x, end_y)
        elif self.distance==False:
            return self.euc_distance(x, y, end_x, end_y)


    # 다음 블록 이동 가능한지  체크 (주어진 위치 x, y 와 이동 방향 x_plus, y_plus 를 기반으로)
    def is_ok(self, x, y, x_plus, y_plus):
        index = y * self.width + x

        if x_plus == 1 and y_plus == 1:
            x_temp = x
            y_temp = y - 1
            temp_index = y_temp * self.width + x_temp
            if temp_index in self.screen.obstacle_blocks_index:
                return False

            x_temp = x - 1
            y_temp = y
            temp_index = y_temp * self.width + x_temp
            if temp_index in self.screen.obstacle_blocks_index:
                return False
        elif x_plus == -1 and y_plus == 1:
            x_temp = x
            y_temp = y - 1
            temp_index = y_temp * self.width + x_temp
            if temp_index in self.screen.obstacle_blocks_index:
                return False

            x_temp = x + 1
            y_temp = y
            temp_index = y_temp * self.width + x_temp
            if temp_index in self.screen.obstacle_blocks_index:
                return False
        elif x_plus == 1 and y_plus == -1:
            x_temp = x
            y_temp = y + 1
            temp_index = y_temp * self.width + x_temp
            if temp_index in self.screen.obstacle_blocks_index:
                return False

            x_temp = x - 1
            y_temp = y
            temp_index = y_temp * self.width + x_temp
            if temp_index in self.screen.obstacle_blocks_index:
                return False
        elif x_plus == -1 and y_plus == -1:
            x_temp = x
            y_temp = y + 1
            temp_index = y_temp * self.width + x_temp
            if temp_index in self.screen.obstacle_blocks_index:
                return False

            x_temp = x + 1
            y_temp = y
            temp_index = y_temp * self.width + x_temp
            if temp_index in self.screen.obstacle_blocks_index:
                return False

        if x < 0 or x >= self.width or y >= self.width or y < 0:
            return False
        elif index in self.close_list or index in self.screen.obstacle_blocks_index:
            return False
        else:
            return True

    def step(self):
        if len(self.open_list) == 0:
            self.screen.over = True
            print('fail')
            return True

        min_f = 99999
        min_index = -1
        for index in self.open_list:
            f = self.screen.blocks[index].f
            if f <= min_f:
                min_f = f
                min_index = index
        self.cur_block = min_index
        self.screen.blocks[min_index].set_close()
        self.close_list.append(min_index)
        self.open_list.remove(min_index)
        self.add_open_list(min_index)