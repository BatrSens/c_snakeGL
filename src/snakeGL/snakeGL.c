//
// Copyright (c) 2018 Melnikov Vladislav.
//

#include "snakeGL.h"
#define RED 3
#define GREEN 4
#define BLUE 5


int vertices_len;

void snake_vertices(struct SnakeGlfw *sngl) {
    vertices_len = sngl->snake->length + sngl->snake->wall_count + 1;

    sngl->vertices = realloc(sngl->vertices, sizeof(GLfloat) * vertices_len * 24);
    sngl->indices = realloc(sngl->indices, sizeof(GLuint) * vertices_len * 6);

    int *coords = malloc(sizeof(int) * vertices_len * 2);
    snake_coords(sngl->snake, coords);
    float *snake_buffer = malloc(sizeof(float) * vertices_len * 2);
    for (int i = 0; i < vertices_len; i++) {
        snake_buffer[i * 2] = (float)coords[i * 2] - (float)sngl->snake->width / 2;
        snake_buffer[i * 2 + 1] = (float)coords[i * 2 + 1] - (float)sngl->snake->height / 2;
    }
//    for (int i = 0; i < vertices_len; i++)
//        printf("%.1f %.1f\n", snake_buffer[i * 2], snake_buffer[i * 2 + 1]);
//    printf("\n");
    free(coords);

    GLfloat *vertices = sngl->vertices;
    for (int i = 0; i < vertices_len * 24; i += 6) {
        vertices[i + 2] = vertices[i + BLUE] = 0.0f;
        vertices[i + GREEN] = 1.0f;
    }
    vertices[4] = vertices[10] = vertices[16] = vertices[22] = 0.5f;
    vertices[3] = vertices[9] = vertices[15] = vertices[21] = 1.0f;

    GLfloat red_step = 1.0f / sngl->snake->length;
    for (int i = 1; i < sngl->snake->length + 1; i++) {
        vertices[i * 24 + RED] = vertices[i * 24 + 6 + RED] = vertices[i * 24 + 12 + RED] =
        vertices[i * 24 + 18 + RED] = red_step * (sngl->snake->length - i);
    }
    for (int i = sngl->snake->length + 1; i < vertices_len; i++) {
        vertices[i * 24 + RED] = vertices[i * 24 + 6 + RED] = vertices[i * 24 + 12 + RED] =
        vertices[i * 24 + 18 + RED] = 0.5f;
        vertices[i * 24 + GREEN] = vertices[i * 24 + 6 + GREEN] = vertices[i * 24 + 12 + GREEN] =
        vertices[i * 24 + 18 + GREEN] = 0.5f;
        vertices[i * 24 + BLUE] = vertices[i * 24 + 6 + BLUE] = vertices[i * 24 + 12 + BLUE] =
        vertices[i * 24 + 18 + BLUE] = 0.5f;
    }

    float width_step = 1.0f / sngl->snake->width;
    float height_step = 1.0f / sngl->snake->height;
    float width_ratio = width_step * 2;
    float height_ratio = height_step * 2;

    vertices[0] = (vertices[18] = snake_buffer[0] * width_ratio + width_step);
    vertices[6] = vertices[0] + width_step;
    vertices[12] = vertices[0] - width_step;
    vertices[7] = (vertices[13] = snake_buffer[1] * height_ratio + height_step);
    vertices[1] = vertices[7] + height_step;
    vertices[19] = vertices[7] - height_step;

    for (int i = 24; i < (sngl->snake->length + 1) * 24; i += 24) {
        vertices[i + 0] = (vertices[i + 6] =
                                   snake_buffer[i / 12] * width_ratio + width_ratio * 0.9f);
        vertices[i + 12] = (vertices[i + 18] = vertices[i + 0] - width_ratio * 0.8f);
        vertices[i + 1] = (vertices[i + 19] =
                                   snake_buffer[i / 12 + 1] * height_ratio + height_ratio * 0.9f);
        vertices[i + 7] = (vertices[i + 13] = vertices[i + 1] - height_ratio * 0.8f);
    }

    for (int i = (sngl->snake->length + 1) * 24; i < vertices_len * 24; i += 24) {
        vertices[i + 0] = (vertices[i + 6] =
                                   snake_buffer[i / 12] * width_ratio + width_ratio);
        vertices[i + 12] = (vertices[i + 18] = vertices[i + 0] - width_ratio);
        vertices[i + 1] = (vertices[i + 19] =
                                   snake_buffer[i / 12 + 1] * height_ratio + height_ratio);
        vertices[i + 7] = (vertices[i + 13] = vertices[i + 1] - height_ratio);
    }

    GLuint *indices = sngl->indices;

    indices[0] = 0;
    indices[1] = (indices[3] = 1);
    indices[2] = (indices[4] = 2);
    indices[5] = 3;

    for (int i = 1; i < vertices_len; i++) {
        GLuint j = i * 4, k = i * 6;
        indices[k + 0] = j + 0;
        indices[k + 1] = (indices[k + 3] = j + 1);
        indices[k + 2] = (indices[k + 5] = j + 3);
        indices[k + 4] = j + 2;
    }

    free(snake_buffer);
}


struct Queue *commands;
int pse = 0;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GL_TRUE);
                printf("Esc key");
                break;
            case GLFW_KEY_SPACE:
                pse = !pse;
                break;
            case GLFW_KEY_LEFT:
            case GLFW_KEY_A:
                if (!pse)
                    push(commands, SNAKE_LEFT);
                break;
            case GLFW_KEY_RIGHT:
            case GLFW_KEY_D:
                if (!pse)
                    push(commands, SNAKE_RIGHT);
                break;
            case GLFW_KEY_DOWN:
            case GLFW_KEY_S:
                if (!pse)
                    push(commands, SNAKE_DOWN);
                break;
            case GLFW_KEY_UP:
            case GLFW_KEY_W:
                if (!pse)
                    push(commands, SNAKE_UP);
                break;
        }
    }
}


void update_picture(struct SnakeGlfw *sngl) {
    snake_vertices(sngl);

    ptrdiff_t size_ver = (sizeof(GLfloat) * vertices_len * 24);
    glBufferData(GL_ARRAY_BUFFER, size_ver, sngl->vertices, GL_DYNAMIC_DRAW);

    ptrdiff_t size_ind = (sizeof(GLuint) * vertices_len * 6);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_ind, sngl->indices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, vertices_len * 6, GL_UNSIGNED_INT, 0);
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
    update_picture(sngl);
    while (!glfwWindowShouldClose(sngl->window)) {
        glfwPollEvents();
        usleep(17500);
        if (!pse)
            timer++;
        if (timer == snake_speed(sngl->snake)) {
            timer = 0;
            if (ping_snake(sngl))
                glfwSetWindowShouldClose(sngl->window, GL_TRUE);
            update_picture(sngl);
        }
    }
}


int snake_init_glfw(struct SnakeGlfw *sngl, GLuint size_x, GLuint size_y, int speed, int mode) {

    //printf("snake_init_begin\n");
    GLuint WIDTH = size_x * 40;
    GLuint HEIGHT = size_y * 40;

    /////////////

    const GLchar* vertexShaderSource = "#version 330 core\n"
            "layout (location = 0) in vec3 position;\n"
            "layout (location = 1) in vec3 color;\n"
            "out vec3 ourColor;\n"
            "void main() {\n"
            "gl_Position = vec4(position, 1.0);\n"
            "ourColor = color;\n"
            "}\0";
    const GLchar* fragmentShaderSource = "#version 330 core\n"
            "in vec3 ourColor;\n"
            "out vec4 color;\n"
            "void main() {\n"
            "color = vec4(ourColor, 1.0f);\n"
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

    //printf("snake_init_shaders\n");
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
    //printf("snake_init_end_sh\n");
    //////////////

    glGenVertexArrays(1, &sngl->VAO);
    glGenBuffers(1, &sngl->VBO);
    glGenBuffers(1, &sngl->EBO);
    glUseProgram(shaderProgram);
    glBindVertexArray(sngl->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, sngl->VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sngl->EBO);

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

    sngl->snake = init_snake(size_x, size_y, speed, mode);
    sngl->vertices = malloc(sizeof(GLfloat) * (sngl->snake->length + 1) * 24);
    sngl->indices = malloc(sizeof(GLuint) * (sngl->snake->length + 1) * 6);
    sngl->commands = init_queue();
    sngl->shaderProgram = shaderProgram;
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
    glfwDestroyWindow(sngl->window);
    glfwTerminate();
}