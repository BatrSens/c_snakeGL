//
// Copyright (c) 2018 Melnikov Vladislav.
//

#include "snakeGL.h"


void snake_vertices(struct Snake *snake, GLfloat *vertices, GLuint *indices) {
    int *coords = malloc(sizeof(int) * (snake->length + 1) * 2);
    snake_coords(snake, coords);
    float *snake_buffer = malloc(sizeof(float) * (snake->length + 1) * 2);
    for (int i = 0; i < snake->length + 1; i++) {
        snake_buffer[i * 2] = (float)coords[i * 2] - (float)snake->width / 2;
        snake_buffer[i * 2 + 1] = (float)coords[i * 2 + 1] - (float)snake->height / 2;
        //printf("%d %d |", snake_buffer[i * 2], snake_buffer[i * 2 + 1]);
    }
    free(coords);
    //printf("\n");

    int vertices_length = (snake->length + 1) * 12;
    for (int i = 2; i < vertices_length; i += 3)
        vertices[i] = 0.0f;

    float width_step = 1.0f / snake->width;
    float height_step = 1.0f / snake->height;
    float width_ratio = width_step * 2;
    float height_ratio = height_step * 2;
    //printf("%f %f %f %f", width_step, width_ratio, height_step, height_ratio);

    vertices[0] = (vertices[9] = snake_buffer[0] * width_ratio + width_step);
    vertices[3] = vertices[0] + width_step;
    vertices[6] = vertices[0] - width_step;
    vertices[4] = (vertices[7] = snake_buffer[1] * height_ratio + height_step);
    vertices[1] = vertices[4] + height_step;
    vertices[10] = vertices[4] - height_step;

    for (int i = 12; i < vertices_length; i += 12) {
        vertices[i + 0] = (vertices[i + 3] =
                                   (float)snake_buffer[i / 6] * width_ratio + width_ratio * 0.9f);
        vertices[i + 6] = (vertices[i + 9] = vertices[i + 0] - width_ratio * 0.8f);
        vertices[i + 1] = (vertices[i + 10] =
                                   (float)snake_buffer[i / 6 + 1] * height_ratio + height_ratio * 0.9f);
        vertices[i + 4] = (vertices[i + 7] = vertices[i + 1] - height_ratio * 0.8f);
    }

    indices[0] = 0;
    indices[1] = (indices[3] = 1);
    indices[2] = (indices[4] = 2);
    indices[5] = 3;

    for (int i = 1; i < snake->length + 1; i++) {
        GLuint j = i * 4, k = i * 6;
        indices[k + 0] = j + 0;
        indices[k + 1] = (indices[k + 3] = j + 1);
        indices[k + 2] = (indices[k + 5] = j + 3);
        indices[k + 4] = j + 2;
    }

    free(snake_buffer);
}


struct Queue *commands;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GL_TRUE);
                printf("Esc key");
                break;
            case GLFW_KEY_LEFT:
            case GLFW_KEY_A:
                push(commands, SNAKE_LEFT);
                break;
            case GLFW_KEY_RIGHT:
            case GLFW_KEY_D:
                push(commands, SNAKE_RIGHT);
                break;
            case GLFW_KEY_DOWN:
            case GLFW_KEY_S:
                push(commands, SNAKE_DOWN);
                break;
            case GLFW_KEY_UP:
            case GLFW_KEY_W:
                push(commands, SNAKE_UP);
                break;
        }
    }
}


void update_picture(struct SnakeGlfw *sngl) {
    sngl->vertices = realloc(sngl->vertices, sizeof(GLfloat) * (sngl->snake->length + 1) * 12);
    sngl->indices = realloc(sngl->indices, sizeof(GLuint) * (sngl->snake->length + 1) * 6);
    snake_vertices(sngl->snake, sngl->vertices, sngl->indices);

    ptrdiff_t size_ver = (sizeof(GLfloat) * (sngl->snake->length + 1) * 12);
    glBufferData(GL_ARRAY_BUFFER, size_ver, sngl->vertices, GL_DYNAMIC_DRAW);

    ptrdiff_t size_ind = (sizeof(GLfloat) * (sngl->snake->length + 1) * 6);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_ind, sngl->indices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glClear(GL_COLOR_BUFFER_BIT);

    glDrawElements(GL_TRIANGLES, (sngl->snake->length + 1) * 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(sngl->window);
}


int ping_snake(struct SnakeGlfw *sngl) {
    int command = pop(sngl->commands);
    while (command != -1 && !change_direction(sngl->snake, command))
        command = pop(sngl->commands);
    if (snake_move(sngl->snake)) {
        printf("Bad move");
        return 1;
    }
    return 0;
}


void snake_start(struct SnakeGlfw *sngl) {
    int timer = 0;
    while (!glfwWindowShouldClose(sngl->window)) {
        update_picture(sngl);
        glfwPollEvents();
        usleep(10000);
        timer++;
        if (timer == snake_speed(sngl->snake)) {
            timer = 0;
            if (ping_snake(sngl))
                glfwSetWindowShouldClose(sngl->window, GL_TRUE);
        }
    }
}


int snake_init_glfw(struct SnakeGlfw *sngl, GLuint size_x, GLuint size_y, int speed) {

    GLuint WIDTH = size_x * 40;
    GLuint HEIGHT = size_y * 40;

    /////////////

    const GLchar* vertexShaderSource = "#version 330 core\n"
            "layout (location = 0) in vec3 position;\n"
            "void main() {\n"
            "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
            "}\0";
    const GLchar* fragmentShaderSource = "#version 330 core\n"
            "out vec4 color;\n"
            "void main() {\n"
            "color = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
            "}\n\0";

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    sngl->window = glfwCreateWindow(WIDTH, HEIGHT, "Snake on GLFW", NULL, NULL);
    if (sngl->window == NULL) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(sngl->window);

    glfwSetKeyCallback(sngl->window, key_callback);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW\n");
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);
    /////////////


    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("Error: vertex shader - compilation failed\n %s\n", infoLog);
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("Error: fragment shader - compilation failed\n %s\n", infoLog);
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("Error: shader program - linking failed\n %s\n", infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    //////////////

    glGenVertexArrays(1, &sngl->VAO);
    glGenBuffers(1, &sngl->VBO);
    glGenBuffers(1, &sngl->EBO);
    glUseProgram(shaderProgram);
    glBindVertexArray(sngl->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, sngl->VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sngl->EBO);

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

    sngl->snake = init_snake(size_x, size_y, speed);
    sngl->vertices = malloc(sizeof(GLfloat) * (sngl->snake->length + 1) * 12);
    sngl->indices = malloc(sizeof(GLuint) * (sngl->snake->length + 1) * 6);
    snake_vertices(sngl->snake, sngl->vertices, sngl->indices);
    sngl->commands = init_queue();
    commands = sngl->commands;
    return 0;
}


void snake_end(struct SnakeGlfw *sngl) {
    remove_queue(sngl->commands);
    remove_snake(sngl->snake);
    free(sngl->vertices);
    free(sngl->indices);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &sngl->VAO);
    glDeleteBuffers(1, &sngl->VBO);
    glfwTerminate();
}