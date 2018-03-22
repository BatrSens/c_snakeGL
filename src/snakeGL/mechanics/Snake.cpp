//
// Copyright (c) 2018 Melnikov Vladislav.
//

#include "Snake.hpp"


bool Snake::snake_is_here(int x, int y) {
    Segment *segment = tail->to_head;
    while (segment != nullptr) {
        if (segment->x == x && segment->y == y)
            return true;
        segment = segment->to_head;
    }
    return false;
}


bool Snake::wall_is_here(int x, int y) {
    Segment *segment = walls;
    while (segment != nullptr) {
        if (segment->x == x && segment->y == y)
            return true;
        segment = segment->to_head;
    }
    return false;
}


void Snake::replace_food() {
    food->x = rand() % params.width;
    food->y = rand() % params.height;
    while (snake_is_here(food->x, food->y) ||
           wall_is_here(food->x, food->y)) {
        food->x = rand() % params.width;
        food->y = rand() % params.height;
    }
}


int max(int a, int b) {
    return abs(a) > abs(b) ? abs(a) : abs(b);
}


void Snake::add_wall() {
    Segment *new_wall = new Segment;
    new_wall->x = rand() % params.width;
    new_wall->y = rand() % params.height;
    while (snake_is_here(new_wall->x, new_wall->y) ||
           wall_is_here(new_wall->x, new_wall->y) ||
           (food->x == new_wall->x && food->y == new_wall->y) ||
           max(new_wall->x - head->x, new_wall->y - head->y) < 3) {
        new_wall->x = rand() % params.width;
        new_wall->y = rand() % params.height;
    }
    new_wall->to_head = walls;
    walls = new_wall;
    wall_count++;
    //printf("wall added %d\n", snake->wall_count);
}


Snake::Snake(SnakeProperties mode) {
    srand(time(0));

    params = mode;

    length_ = params.width / 4 + 1;


    head = new Segment;
    head->x = length_;
    head->y = 1;
    head->to_head = nullptr;
    tail = head;
    for (int i = 1; i < (int)length_ + 1; i++) {
        struct Segment *segment = new Segment;
        segment->x = length_ - i;
        segment->y = 1;
        segment->to_head = tail;
        tail = segment;
    }

    walls = nullptr;
    food = new Food;
    replace_food();

    wall_count = 0;

    cur_direction = SNAKE_RIGHT;
    new_direction = SNAKE_RIGHT;
}


void Snake::remove_list(Segment *segment) {
    while (segment != nullptr) {
        struct Segment *next = segment->to_head;
        delete segment;
        segment = next;
    }
}


Snake::~Snake() {
    remove_list(tail);
    remove_list(walls);
    delete food;
}


void Snake::snake_eating() {
    length_++;
    Segment *new_segm = new Segment;
    new_segm->x = tail->x;
    new_segm->y = tail->y;
    new_segm->to_head = tail;
    tail = new_segm;
    replace_food();
    if (params.mode.walls)
        add_wall();
}


bool Snake::change_direction(unsigned int direction) {
    if (cur_direction % 2 != direction % 2) {
        new_direction = direction % 4;
        return true;
    }
    return false;
}


bool Snake::move() {
    struct Segment *segment = tail;
    while (segment != head) {
        segment->x = segment->to_head->x;
        segment->y = segment->to_head->y;
        segment = segment->to_head;
    }
    int x = head->x;
    int y = head->y;
    cur_direction = new_direction;
    switch (cur_direction) {
        case SNAKE_UP:
            y++;
            break;
        case SNAKE_RIGHT:
            x++;
            break;
        case SNAKE_DOWN:
            y--;
            break;
        case SNAKE_LEFT:
            x--;
            break;
    }
    if (!params.mode.borders) {
        if (x >= (int)params.width)
            x = 0;
        else if (x < 0)
            x = (int)params.width - 1;
        if (y >= (int)params.height)
            y = 0;
        else if (y < 0)
            y = (int)params.height - 1;
    }
    if (x >= (int)params.width || x < 0 ||
        y >= (int)params.height || y < 0 ||
        snake_is_here(x, y) ||
        wall_is_here(x, y))
        return true;
    head->x = x;
    head->y = y;
    if (head->x == food->x && head->y == food->y)
        snake_eating();
    return false;
}


int Snake::speed() {
    if (!params.mode.speed)
        return params.speed;
    float size = (params.width + params.height) / 2;
    float rate;
    if (size < 9)
        rate = 2;
    else if (size < 16)
        rate = 3;
    else
        rate = 4;
    float dif = rate * length_ * 0.4f;
    int result = params.speed - (int)dif;
    return result > 8 ? result : 8;
}


void Snake::coords(int *buffer) {
    buffer[0] = food->x;
    buffer[1] = food->y;
    int i = 2;
    struct Segment *segment = tail->to_head;
    while (segment != nullptr) {
        buffer[i++] = segment->x;
        buffer[i++] = segment->y;
        segment = segment->to_head;
    }
    segment = walls;
    while (segment != nullptr) {
        buffer[i++] = segment->x;
        buffer[i++] = segment->y;
        segment = segment->to_head;
    }
}