cmake_minimum_required(VERSION 3.8)
project(myster-us-adventures C)

set(CMAKE_C_STANDARD 11)

add_executable(myster-us-adventures
        main.c include/struct.h init.c gameplay.c scoremanager.c game.c menu.c include/usual.h include/init.h include/gameplay.h include/menu.h include/scoremanager.h include/game.h)
