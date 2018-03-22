//
// Copyright (c) 2018 Melnikov Vladislav.
//

#ifndef CPP_SNAKEGL_SNAKEGL_HPP
#define CPP_SNAKEGL_SNAKEGL_HPP

#include <stddef.h>
#define _BSD_SOURCE
#include <stdio.h>
#include "mechanics/Snake.hpp"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <unistd.h>

#include "../Queue/Queue.hpp"



class SnakeGL {
public:
    SnakeGL(SnakeProperties params);
    virtual ~SnakeGL();
    void snake_start();

private:
    GLfloat *vertices;
    GLuint *indices;
    Snake *snake_;
    GLuint VBO, VAO, EBO, shaderProgram;
    //Queue<int> *commands;
    GLFWwindow* window;

    void snake_vertices();
    void update_picture();
    bool ping_snake();
};

//void snake_vertices(struct Snake *snake, GLfloat *vertices, GLuint *indices);


#endif //CPP_SNAKEGL_SNAKEGL_HPP
