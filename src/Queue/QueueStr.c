//
// Copyright (c) 2018 Melnikov Vladislav.
//

#include "QueueStr.h"

struct QueueStr *init_queue_s() {
    struct QueueStr *queue = malloc(sizeof(struct QueueStr));
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

void remove_queue_s(struct QueueStr *queue) {
    struct ListStr *elem = queue->tail;
    while (elem != NULL) {
        struct ListStr *next = elem->next;
        free(elem);
        elem = next;
    }
    free(queue);
}

void push_s(struct QueueStr *queue, char *value) {
    struct ListStr *new_elem = malloc(sizeof(struct ListStr));
    new_elem->value = value;
    new_elem->next = NULL;
    if (queue->head == NULL)
        queue->tail = new_elem;
    else
        queue->head->next = new_elem;
    queue->head = new_elem;
}

char *pop_s(struct QueueStr *queue) {
    if (queue->tail == NULL)
        return "";
    struct ListStr *tail = queue->tail;
    char *value = tail->value;
    queue->tail = tail->next;
    free(tail);
    if (queue->tail == NULL)
        queue->head = NULL;
    return value;
}