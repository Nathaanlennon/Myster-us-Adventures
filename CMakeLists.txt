cmake_minimum_required(VERSION 3.8)
project(myster-us-adventures C)

set(CMAKE_C_STANDARD 11)
# Common functions
function(add_common_compile_options target)
    if (MSVC)
        target_compile_options(${target} PRIVATE /D_CRT_SECURE_NO_WARNINGS /utf-8)


    endif ()
endfunction()
# Sub-dependencies

add_executable(myster-us-adventures
        main.c include/struct.h init.c gameplay.c scoremanager.c game.c menu.c include/usual.h include/init.h include/gameplay.h include/menu.h include/scoremanager.h include/game.h)
