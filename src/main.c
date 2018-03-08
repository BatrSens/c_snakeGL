//
// Copyright (c) 2018 Melnikov Vladislav.
//

#include <string.h>
#include "snakeGL/snakeGL.h"


int main(int argc, char **argv) {

    printf("Flags:\n");
    printf("-w [WIDTH]      from 5 to 40, default: 15\n");
    printf("-h [HEIGHT]     from 5 to 20, default: 7\n");
    printf("-s [SPEED]      from 15 to 75, default: 35\n\n");

    int size_x = 15;
    int size_y = 7;
    int speed = 35;

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-w")) {
            if (++i >= argc) {
                printf("Error: -w [WIDTH]\n");
                return 0;
            }
            size_x = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-h")) {
            if (++i >= argc) {
                printf("Error: -h [HEIGHT]\n");
                return 0;
            }
            size_y = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-s")) {
            if (++i >= argc) {
                printf("Error: -s [SPEED]\n");
                return 0;
            }
            speed = atoi(argv[i]);
        }
        else {
            printf("Invalid flags\n");
            return 0;
        }
    }

    if (size_x < 5)
        size_x = 5;
    if (size_x > 40)
        size_x = 40;
    if (size_y < 5)
        size_y = 5;
    if (size_y > 20)
        size_y = 20;
    if (speed < 15)
        speed = 15;
    if (speed > 75)
        speed = 75;

    printf("WIDTH:   %d\n", size_x);
    printf("HEIGHT:  %d\n", size_y);
    printf("SPEED:   %d\n\n", speed);

    struct SnakeGlfw sn_glfw;
    if (snake_init_glfw(&sn_glfw, size_x, size_y, speed) == -1)
        return -1;
    snake_start(&sn_glfw);
    printf(", result: %d\n", sn_glfw.snake->length);
    snake_end(&sn_glfw);

    return 0;
}