//
// Copyright (c) 2018 Melnikov Vladislav.
//

#include "snake.h"


int snake_is_here(struct Snake *snake, int x, int y) {
    struct Segment *segment = snake->tail->to_head;
    while (segment != NULL) {
        if (segment->x == x && segment->y == y)
            return 1;
        segment = segment->to_head;
    }
    return 0;
}


int wall_is_here(struct Snake *snake, int x, int y) {
    struct Segment *segment = snake->walls;
    while (segment != NULL) {
        if (segment->x == x && segment->y == y)
            return 1;
        segment = segment->to_head;
    }
    return 0;
}


void replace_food(struct Snake *snake) {
    snake->food->x = rand() % snake->width;
    snake->food->y = rand() % snake->height;
    while (snake_is_here(snake, snake->food->x, snake->food->y) ||
            wall_is_here(snake, snake->food->x, snake->food->y)) {
        snake->food->x = rand() % snake->width;
        snake->food->y = rand() % snake->height;
    }
}


int max(int a, int b) {
    return abs(a) > abs(b) ? abs(a) : abs(b);
}


void add_wall(struct Snake *snake) {
    struct Segment *new_wall = malloc(sizeof(struct Segment));
    new_wall->x = rand() % snake->width;
    new_wall->y = rand() % snake->height;
    while (snake_is_here(snake, new_wall->x, new_wall->y) ||
            wall_is_here(snake, new_wall->x, new_wall->y) ||
            (snake->food->x == new_wall->x && snake->food->y == new_wall->y) ||
            max(new_wall->x - snake->head->x, new_wall->y - snake->head->y) < 3) {
        new_wall->x = rand() % snake->width;
        new_wall->y = rand() % snake->height;
    }
    new_wall->to_head = snake->walls;
    snake->walls = new_wall;
    snake->wall_count++;
    //printf("wall added %d\n", snake->wall_count);
}


struct Snake *init_snake(unsigned int width, unsigned int height, int speed, int mode) {
    srand(time(NULL));
    //printf("snake_init_snake_begin\n");

    struct Snake *snake = malloc(sizeof(struct Snake));
    snake->length = width / 4 + 1;

    snake->width = width;
    snake->height = height;

    snake->head = malloc(sizeof(struct Segment));
    snake->head->x = snake->length;
    snake->head->y = 1;
    snake->head->to_head = NULL;
    snake->tail = snake->head;
    for (int i = 1; i < snake->length + 1; i++) {
        struct Segment *segment = malloc(sizeof(struct Segment));
        segment->x = snake->length - i;
        segment->y = 1;
        segment->to_head = snake->tail;
        snake->tail = segment;
    }

    snake->walls = NULL;
    snake->speed = speed;
    snake->food = malloc(sizeof(struct Food));
    replace_food(snake);

    snake->mode = mode;
    snake->wall_count = 0;

    snake->cur_direction = SNAKE_RIGHT;
    snake->new_direction = SNAKE_RIGHT;
    return snake;
}


void remove_list(struct Segment *segment) {
    while (segment != NULL) {
        struct Segment *next = segment->to_head;
        free(segment);
        segment = next;
    }
}


void remove_snake(struct Snake *snake) {
    remove_list(snake->tail);
    remove_list(snake->walls);
    free(snake->food);
    free(snake);
}


void snake_eating(struct Snake *snake) {
    snake->length++;
    struct Segment *new_segm = malloc(sizeof(struct Segment));
    new_segm->x = snake->tail->x;
    new_segm->y = snake->tail->y;
    new_segm->to_head = snake->tail;
    snake->tail = new_segm;
    replace_food(snake);
    if (snake->mode == MODE_WALLS)
        add_wall(snake);
}


int change_direction(struct Snake *snake, unsigned int direction) {
    if (snake->cur_direction % 2 != direction % 2) {
        snake->new_direction = direction % 4;
        return 1;
    }
    return 0;
}


int snake_move(struct Snake *snake) {
    struct Segment *segment = snake->tail;
    while (segment != snake->head) {
        segment->x = segment->to_head->x;
        segment->y = segment->to_head->y;
        segment = segment->to_head;
    }
    int x = snake->head->x;
    int y = snake->head->y;
    snake->cur_direction = snake->new_direction;
    switch (snake->cur_direction) {
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
    if (x >= snake->width || x < 0 ||
        y >= snake->height || y < 0 ||
        snake_is_here(snake, x, y) ||
        wall_is_here(snake, x, y))
        return 1;
    snake->head->x = x;
    snake->head->y = y;
    if (snake->head->x == snake->food->x && snake->head->y == snake->food->y)
        snake_eating(snake);
    return 0;
}


int snake_speed(struct Snake *snake) {
    float size = snake->width < snake->height ? snake->width : snake->height;
    float rate;
    if (size < 9)
        rate = 2;
    else if (size < 16)
        rate = 3;
    else
        rate = 4;
    float dif = rate * snake->length * 0.4f;
    int result = snake->speed - (int)dif;
    return result > 8 ? result : 8;
}


void snake_coords(struct Snake *snake, int *buffer) {
    buffer[0] = snake->food->x;
    buffer[1] = snake->food->y;
    int i = 2;
    struct Segment *segment = snake->tail->to_head;
    while (segment != NULL) {
        buffer[i++] = segment->x;
        buffer[i++] = segment->y;
        segment = segment->to_head;
    }
    segment = snake->walls;
    while (segment != NULL) {
        buffer[i++] = segment->x;
        buffer[i++] = segment->y;
        segment = segment->to_head;
    }
}