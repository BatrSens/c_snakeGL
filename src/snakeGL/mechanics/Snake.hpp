//
// Copyright (c) 2018 Melnikov Vladislav.
//

#ifndef CPP_SNAKEGL_SNAKE_HPP
#define CPP_SNAKEGL_SNAKE_HPP

#define SNAKE_UP 0
#define SNAKE_RIGHT 1
#define SNAKE_DOWN 2
#define SNAKE_LEFT 3

#include <iostream>


struct SnakeMode {
    int speed;
    int walls;
    int borders;
};


struct SnakeProperties {
    unsigned int width, height;
    int speed;
    SnakeMode mode;
};


class Snake {
public:
    Snake(SnakeProperties mode);
    virtual ~Snake();

    bool change_direction(unsigned int direction);
    bool move();
    int speed();
    int length() { return length_; }
    int wall_c() { return wall_count; }
    int width() { return params.width; }
    int height() { return params.height; }

//The first coordinates - food, the rest - the snake
    void coords(int *buffer);

private:
    struct Food {
        int x, y;
    };

    struct Segment {
        int x, y;
        struct Segment *to_head;
    };

    SnakeProperties params;

    Segment *tail;
    Segment *head;
    Food *food;

    Segment *walls;
    unsigned int wall_count;

    unsigned int length_;

    unsigned int new_direction;
    unsigned int cur_direction;

    void replace_food();
    bool wall_is_here(int x, int y);
    bool snake_is_here(int x, int y);
    void add_wall();
    void snake_eating();

    void remove_list(Segment *segment);
};


#endif //CPP_SNAKEGL_SNAKE_HPP
