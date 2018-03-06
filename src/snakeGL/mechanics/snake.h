//
// Copyright (c) 2018 Melnikov Vladislav.
//

#ifndef GLFWLAB1_SNAKE_H
#define GLFWLAB1_SNAKE_H

#define SNAKE_UP 0
#define SNAKE_RIGHT 1
#define SNAKE_DOWN 2
#define SNAKE_LEFT 3

#include <stdlib.h>
#include <stdio.h>
#include <time.h>


struct Food {
    int x, y;
};

struct Segment {
    int x, y;
    struct Segment *to_head;
};

struct Snake {
    unsigned int width;
    unsigned int height;

    struct Segment *tail;
    struct Segment *head;
    struct Food *food;

    unsigned int length;

    unsigned int new_direction;
    unsigned int cur_direction;

    int speed;
};

/////////////////////////////////////////////

struct Snake *init_snake(unsigned int width, unsigned int height, int speed);
void remove_snake(struct Snake *snake);

//void replace_food(struct Snake *snake);
//int snake_is_here(struct Snake *snake, int x, int y);
//void snake_eating(struct Snake *snake);

int change_direction(struct Snake *snake, unsigned int direction);
int snake_move(struct Snake *snake);
int snake_speed(struct Snake *snake);

//The first coordinates - food, the rest - the snake
void snake_coords(struct Snake *snake, int *buffer);


#endif //GLFWLAB1_SNAKE_H
