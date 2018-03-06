//
// Copyright (c) 2018 Melnikov Vladislav.
//

#ifndef GLFWLAB1_QUEUE_H
#define GLFWLAB1_QUEUE_H

#include <stdlib.h>


struct List {
    int value;
    struct List *next;
};

struct Queue {
    struct List *head;
    struct List *tail;
};

struct Queue *init_queue();
void remove_queue(struct Queue *queue);
void push(struct Queue *queue, int value);
int pop(struct Queue *queue);


#endif //GLFWLAB1_QUEUE_H
