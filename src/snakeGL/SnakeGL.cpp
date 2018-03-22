//
// Copyright (c) 2018 Melnikov Vladislav.
//

#include "SnakeGL.hpp"

#define RED 3
#define GREEN 4
#define BLUE 5


int vertices_len;

void SnakeGL::snake_vertices() {
    vertices_len = snake_->length() + snake_->wall_c() + 1;

    delete vertices;
    delete indices;

    vertices = new GLfloat[vertices_len * 24];
    indices = new GLuint[vertices_len * 6];

    int *coords = new int[vertices_len * 2];
    snake_->coords(coords);
    float *snake_buffer = new float[vertices_len * 2];
    for (int i = 0; i < vertices_len; i++) {
        snake_buffer[i * 2] = (float)coords[i * 2] - (float)snake_->width() / 2;
        snake_buffer[i * 2 + 1] = (float)coords[i * 2 + 1] - (float)snake_->height() / 2;
    }

    delete coords;

    for (int i = 0; i < vertices_len * 24; i += 6) {
        vertices[i + 2] = vertices[i + BLUE] = 0.0f;
        vertices[i + GREEN] = 1.0f;
    }
    vertices[4] = vertices[10] = vertices[16] = vertices[22] = 0.5f;
    vertices[3] = vertices[9] = vertices[15] = vertices[21] = 1.0f;

    GLfloat red_step = 1.0f / (snake_->length() - 1);
    for (int i = 1; i < snake_->length() + 1; i++) {
        vertices[i * 24 + RED] = vertices[i * 24 + 6 + RED] = vertices[i * 24 + 12 + RED] =
        vertices[i * 24 + 18 + RED] = red_step * (snake_->length() - i);
        //std::cout << vertices[i * 24 + RED] << '\n';
    }
    for (int i = snake_->length() + 1; i < vertices_len; i++) {
        vertices[i * 24 + RED] = vertices[i * 24 + 6 + RED] = vertices[i * 24 + 12 + RED] =
        vertices[i * 24 + 18 + RED] = 0.5f;
        vertices[i * 24 + GREEN] = vertices[i * 24 + 6 + GREEN] = vertices[i * 24 + 12 + GREEN] =
        vertices[i * 24 + 18 + GREEN] = 0.5f;
        vertices[i * 24 + BLUE] = vertices[i * 24 + 6 + BLUE] = vertices[i * 24 + 12 + BLUE] =
        vertices[i * 24 + 18 + BLUE] = 0.5f;
    }

    float width_step = 1.0f / snake_->width();
    float height_step = 1.0f / snake_->height();
    float width_ratio = width_step * 2;
    float height_ratio = height_step * 2;

    vertices[0] = (vertices[18] = snake_buffer[0] * width_ratio + width_step);
    vertices[6] = vertices[0] + width_step;
    vertices[12] = vertices[0] - width_step;
    vertices[7] = (vertices[13] = snake_buffer[1] * height_ratio + height_step);
    vertices[1] = vertices[7] + height_step;
    vertices[19] = vertices[7] - height_step;

    for (int i = 24; i < (snake_->length() + 1) * 24; i += 24) {
        vertices[i + 0] = (vertices[i + 6] =
                                   snake_buffer[i / 12] * width_ratio + width_ratio * 0.9f);
        vertices[i + 12] = (vertices[i + 18] = vertices[i + 0] - width_ratio * 0.8f);
        vertices[i + 1] = (vertices[i + 19] =
                                   snake_buffer[i / 12 + 1] * height_ratio + height_ratio * 0.9f);
        vertices[i + 7] = (vertices[i + 13] = vertices[i + 1] - height_ratio * 0.8f);
    }

    for (int i = (snake_->length() + 1) * 24; i < vertices_len * 24; i += 24) {
        vertices[i + 0] = (vertices[i + 6] =
                                   snake_buffer[i / 12] * width_ratio + width_ratio);
        vertices[i + 12] = (vertices[i + 18] = vertices[i + 0] - width_ratio);
        vertices[i + 1] = (vertices[i + 19] =
                                   snake_buffer[i / 12 + 1] * height_ratio + height_ratio);
        vertices[i + 7] = (vertices[i + 13] = vertices[i + 1] - height_ratio);
    }

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

    delete snake_buffer;
}


Queue<int> *commands;
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
                    commands->push(SNAKE_LEFT);
                break;
            case GLFW_KEY_RIGHT:
            case GLFW_KEY_D:
                if (!pse)
                    commands->push(SNAKE_RIGHT);
                break;
            case GLFW_KEY_DOWN:
            case GLFW_KEY_S:
                if (!pse)
                    commands->push(SNAKE_DOWN);
                break;
            case GLFW_KEY_UP:
            case GLFW_KEY_W:
                if (!pse)
                    commands->push(SNAKE_UP);
                break;
        }
    }
}


void SnakeGL::update_picture() {
    snake_vertices();

    ptrdiff_t size_ver = (sizeof(GLfloat) * vertices_len * 24);
    glBufferData(GL_ARRAY_BUFFER, size_ver, vertices, GL_DYNAMIC_DRAW);

    ptrdiff_t size_ind = (sizeof(GLuint) * vertices_len * 6);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_ind, indices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, vertices_len * 6, GL_UNSIGNED_INT, nullptr);
    glfwSwapBuffers(window);
}


bool SnakeGL::ping_snake() {
    while (!commands->is_empty() && !snake_->change_direction(commands->pop()));
    if (snake_->move()) {
        std::cout << "Bad move";
        return true;
    }
    return false;
}


void SnakeGL::snake_start() {
    int timer = 0;
    update_picture();
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        usleep(17500);
        if (!pse)
            timer++;
        if (timer == snake_->speed()) {
            timer = 0;
            if (ping_snake())
                glfwSetWindowShouldClose(window, GL_TRUE);
            update_picture();
        }
    }
}


SnakeGL::SnakeGL(SnakeProperties params) {
    GLuint WIDTH = params.width * 40;
    GLuint HEIGHT = params.height * 40;

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

    window = glfwCreateWindow(WIDTH, HEIGHT, "Snake on GLFW", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW\n");
        exit(-1);
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

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

    snake_ = new Snake(params);
    vertices = new GLfloat[(snake_->length() + 1) * 24];
    indices = new GLuint[(snake_->length() + 1) * 6];
    commands = new Queue<int>;
    //commands = this->commands;
}


SnakeGL::~SnakeGL() {
    std::cout << ", result: " << snake_->length() << '\n';

    delete commands;
    delete snake_;
    delete vertices;
    delete indices;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwDestroyWindow(window);
    glfwTerminate();
}