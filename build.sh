#!/usr/bin/env bash

if [[ ! -d './bin' ]]; then
    mkdir bin
fi
g++ src/main.cpp src/snakeGL/SnakeGL.cpp src/snakeGL/mechanics/Snake.cpp -std=c++14 -Wall -o bin/c_snakeGL -lglfw -lGL -lGLEW