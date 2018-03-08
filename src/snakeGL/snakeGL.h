//
// Copyright (c) 2018 Melnikov Vladislav.
//

#ifndef GLFWLAB1_SNAKEGL_H
#define GLFWLAB1_SNAKEGL_H

#include <stddef.h>
#define _BSD_SOURCE
#include <stdio.h>
#include "mechanics/snake.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <unistd.h>

#include "../Queue/Queue.h"



struct SnakeGlfw {
    GLfloat *vertices;
    GLuint *indices;
    struct Snake *snake;
    GLuint VBO, VAO, EBO, shaderProgram;
    struct Queue *commands;
    GLFWwindow* window;
};

//void snake_vertices(struct Snake *snake, GLfloat *vertices, GLuint *indices);
int snake_init_glfw(struct SnakeGlfw *sngl, GLuint WIDTH, GLuint HEIGHT, int speed);
void snake_start(struct SnakeGlfw *sngl);
void snake_end(struct SnakeGlfw *sngl);


#endif //GLFWLAB1_SNAKEGL_H
