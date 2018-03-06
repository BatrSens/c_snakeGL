//
// Copyright (c) 2018 Melnikov Vladislav.
//

#include "Queue.h"


struct Queue *init_queue() {
    struct Queue *queue = malloc(sizeof(struct Queue));
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

void remove_queue(struct Queue *queue) {
    struct List *elem = queue->tail;
    while (elem != NULL) {
        struct List *next = elem->next;
        free(elem);
        elem = next;
    }
    free(queue);
}

void push(struct Queue *queue, int value) {
    struct List *new_elem = malloc(sizeof(struct List));
    new_elem->value = value;
    new_elem->next = NULL;
    if (queue->head == NULL)
        queue->tail = new_elem;
    else
        queue->head->next = new_elem;
    queue->head = new_elem;
}

int pop(struct Queue *queue) {
    if (queue->tail == NULL)
        return -1;
    struct List *tail = queue->tail;
    int value = tail->value;
    queue->tail = tail->next;
    free(tail);
    if (queue->tail == NULL)
        queue->head = NULL;
    return value;
}