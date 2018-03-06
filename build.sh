#!/usr/bin/env bash

if [[ ! -d './bin' ]]; then
    mkdir bin
fi
gcc src/main.c src/snakeGL/snakeGL.c src/snakeGL/mechanics/snake.c src/Queue/Queue.c -std=c11 -Wall -o bin/c_snakeGL -lglfw -lGL -lGLEW