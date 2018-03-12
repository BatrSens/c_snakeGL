//
// Copyright (c) 2018 Melnikov Vladislav.
//

#include <string.h>
#include "Queue/QueueStr.h"
#include "snakeGL/snakeGL.h"


int is_number(char *str) {
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] < '0' || str[i] > '9')
            return 0;
    }
    return 1;
}


int main(int argc, char **argv) {

    printf("Flags:\n");
    printf("-w [WIDTH]      from 5 to 40, default: 15\n");
    printf("-h [HEIGHT]     from 5 to 20, default: 7\n");
    printf("-s [SPEED]      from 15 to 75, default: 35\n");
    printf("-d [MODE]       c or w, default: c\n\n");

    int size_x = 15;
    int size_y = 7;
    int speed = 35;
    int mode = MODE_CLASSIC;
    char modes[10];
    strcpy(modes, "CLASSIC");

    struct QueueStr *errors = init_queue_s();

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-w")) {
            if (i + 1 >= argc || !is_number(argv[i + 1]))
                push_s(errors, "Error: -w [WIDTH]\n");
            else
                size_x = atoi(argv[++i]);
        }
        else if (!strcmp(argv[i], "-h")) {
            if (i + 1 >= argc || !is_number(argv[i + 1]))
                push_s(errors, "Error: -h [HEIGHT]\n");
            else
                size_y = atoi(argv[++i]);
        }
        else if (!strcmp(argv[i], "-s")) {
            if (i + 1 >= argc || !is_number(argv[i + 1]))
                push_s(errors, "Error: -s [SPEED]\n");
            else
                speed = atoi(argv[++i]);
        }
        else if (!strcmp(argv[i], "-m")) {
            if (i + 1 >= argc)
                push_s(errors, "Error: -d [MODE]\n");
            else if (!strcmp(argv[i + 1], "c")) {
                mode = MODE_CLASSIC;
                strcpy(modes, "CLASSIC");
                i++;
            }
            else if (!strcmp(argv[i + 1], "w")) {
                mode = MODE_WALLS;
                strcpy(modes, "WALLS");
                i++;
            }
            else
                push_s(errors, "Invalid mode 's'\n");
        }
        else
            push_s(errors, "Invalid flag #n\n");
    }

    char *error = pop_s(errors);
    if (strcmp(error, "")) {
        while (strcmp(error, "")) {
            printf("%s", error);
            error = pop_s(errors);
        }
        remove_queue_s(errors);
        return 0;
    }

    remove_queue_s(errors);

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
    if (size_x < 7 || size_y < 7) {
        mode = MODE_CLASSIC;
        strcpy(modes, "CLASSIC");
    }

    printf("WIDTH:   %d\n", size_x);
    printf("HEIGHT:  %d\n", size_y);
    printf("SPEED:   %d\n", speed);
    printf("MODE:    %s\n\n", modes);

    struct SnakeGlfw sn_glfw;
    if (snake_init_glfw(&sn_glfw, size_x, size_y, speed, mode) == -1) {
        printf("Error: snake_init");
        return -1;
    }
    snake_start(&sn_glfw);
    printf(", result: %d\n", sn_glfw.snake->length);
    snake_end(&sn_glfw);

    return 0;
}