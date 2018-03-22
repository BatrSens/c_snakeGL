//
// Copyright (c) 2018 Melnikov Vladislav.
//

#ifndef CPP_SNAKEGL_QUEUE_HPP
#define CPP_SNAKEGL_QUEUE_HPP


template <class T>
class Queue {
public:
    Queue();
    virtual ~Queue();

    void push(T value);
    T pop();

    bool is_empty() { return tail == nullptr; }

private:
    struct List {
        T value;
        struct List *next;
    };

    List *head;
    List *tail;
};


template <class T>
Queue<T>::Queue() {
    head = nullptr;
    tail = nullptr;
}


template <class T>
Queue<T>::~Queue() {
    List *elem = tail;
    while (elem != nullptr) {
        List *next = elem->next;
        delete elem;
        elem = next;
    }
}


template <class T>
void Queue<T>::push(T value) {
    List *new_elem = new List;
    new_elem->value = value;
    new_elem->next = nullptr;
    if (head == nullptr)
        tail = new_elem;
    else
        head->next = new_elem;
    head = new_elem;
}


template <class T>
T Queue<T>::pop() {
    if (is_empty())
        return 0;
    struct List *old_tail = tail;
    T res = old_tail->value;
    tail = old_tail->next;
    delete old_tail;
    if (tail == nullptr)
        head = nullptr;
    return res;
}


#endif //CPP_SNAKEGL_QUEUE_HPP
