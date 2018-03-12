//
// Copyright (c) 2018 Melnikov Vladislav.
//

#ifndef GLFWLAB1_QUEUESTR_H
#define GLFWLAB1_QUEUESTR_H


#include <stdlib.h>


struct ListStr {
    char *value;
    struct ListStr *next;
};

struct QueueStr {
    struct ListStr *head;
    struct ListStr *tail;
};

struct QueueStr *init_queue_s();
void remove_queue_s(struct QueueStr *queue);
void push_s(struct QueueStr *queue, char *value);
char *pop_s(struct QueueStr *queue);


#endif //GLFWLAB1_QUEUESTR_H

