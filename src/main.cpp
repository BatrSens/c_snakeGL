//
// Copyright (c) 2018 Melnikov Vladislav.
//

#include <string>
#include "Queue/Queue.hpp"
#include "snakeGL/SnakeGL.hpp"


void proc_args(int argc, char **argv, Queue<std::string> &errors,
               SnakeProperties &params, std::string &mode);

int main(int argc, char **argv) {

    std::cout << "Flags:\n";
    std::cout << "-w [WIDTH]......from 5 to 40, default: 15\n";
    std::cout << "-h [HEIGHT].....from 5 to 20, default: 7\n";
    std::cout << "-s [SPEED]......from 15 to 75, default: 35\n";
    std::cout << "-m [MODE].......s, w, b, default: s b\n";
    std::cout << "-i .............rerun with this flag for more information\n\n\n";

    SnakeProperties params = {15, 7, 35, {1, 0, 1}};

    Queue<std::string> errors;
    std::string mode = "s b ";

    proc_args(argc, argv, errors, params, mode);
    if (mode == "")
        mode = "classic";

    //bool not_end = errors.pop();
    if (!errors.is_empty()) {
        while (!errors.is_empty())
            std::cout << errors.pop();
        return 0;
    }

    std::cout << "WIDTH:   " << params.width << '\n';
    std::cout << "HEIGHT:  " << params.height << '\n';
    std::cout << "SPEED:   " << params.speed << '\n';
    std::cout << "MODE:    " << mode << "\n\n";

    SnakeGL sn_glfw(params);
    sn_glfw.snake_start();

    return 0;
}





//////////////////////////////////


int is_number(std::string str) {
    for (int i = 0; i < (int)str.length(); i++) {
        if (str[i] < '0' || str[i] > '9')
            return 0;
    }
    return 1;
}


void proc_args(int argc, char **argv, Queue<std::string> &errors,
               SnakeProperties &params, std::string &mode) {
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-w") {
            if (i + 1 >= argc || !is_number(argv[i + 1]))
                errors.push("Error: -w [WIDTH]\n");
            else
                params.width = atoi(argv[++i]);
        }
        else if (arg == "-h") {
            if (i + 1 >= argc || !is_number(argv[i + 1]))
                errors.push("Error: -h [HEIGHT]\n");
            else
                params.height = atoi(argv[++i]);
        }
        else if (arg == "-s") {
            if (i + 1 >= argc || !is_number(argv[i + 1]))
                errors.push("Error: -s [SPEED]\n");
            else
                params.speed = atoi(argv[++i]);
        }
        else if (arg == "-m") {
            params.mode = {0, 0, 0};
            mode = "";
            if (i + 1 < argc) {
                std::string m = argv[i + 1];
                while (i + 1 < argc && m[0] != '-') {
                    if (m == "s") {
                        params.mode.speed = 1;
                        mode += "s ";
                    }
                    else if (m == "w") {
                        params.mode.walls = 1;
                        mode += "w ";
                    }
                    else if (m == "b") {
                        params.mode.borders = 1;
                        mode += "b ";
                    }
                    else {
                        std::string err = "Error: Invalid mode '";
                        err += m;
                        err += "'\n";
                        errors.push(err);
                    }
                    if ((++i) + 1 < argc)
                        m = argv[i + 1];
                }
            }
        }
        else if (arg == "-i") {
            errors.push("Flags:\n");
            errors.push("-i .............information\n\n");
            errors.push("-w [WIDTH]......Field width in cells,\n");
            errors.push("                from 5 to 40, default: 15\n\n");
            errors.push("-h [HEIGHT].....Field height in cells,\n");
            errors.push("                from 5 to 20, default: 7\n\n");
            errors.push("-s [SPEED]......Delay before moving the snake,\n");
            errors.push("                from 15 to 75, default: 35\n\n");
            errors.push("-m [MODE].......Modifications of rules,\n");
            errors.push("                s, w, b, default: s b\n\n\n");
            errors.push("Modifications:\n");
            errors.push("s ..............Speed mode:\n");
            errors.push("                The longer the snake,\n");
            errors.push("                the higher the speed.\n\n");
            errors.push("w ..............Walls mode:\n");
            errors.push("                When a snake eats food,\n");
            errors.push("                the wall appears in a random\n");
            errors.push("                place of the field.\n\n");
            errors.push("b ..............Borders mode:\n");
            errors.push("                When you try to go\n");
            errors.push("                beyond the borders of the\n");
            errors.push("                field the game ends.\n");
        }
        else {
            std::string err = "Error: Invalid flag '";
            err += arg;
            err += "'\n";
            errors.push(err);
        }
    }


    if (params.width < 5)
        params.width = 5;
    if (params.width > 40)
        params.width = 40;
    if (params.height < 5)
        params.height = 5;
    if (params.height > 20)
        params.height = 20;
    if (params.speed < 15)
        params.speed = 15;
    if (params.speed > 75)
        params.speed = 75;
    if (params.width < 7 || params.height < 7)
        params.mode.walls = 0;
}