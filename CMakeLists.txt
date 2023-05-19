cmake_minimum_required(VERSION 3.25)
project(myster-us-adventures C)

set(CMAKE_C_STANDARD 11)
# Common functions
function(add_common_compile_options target)
    if (MSVC)
        target_compile_options(${target} PRIVATE /D_CRT_SECURE_NO_WARNINGS /utf-8)


    endif ()
endfunction()
# Sub-dependencies
if (MSVC)
    add_subdirectory(external/pdcurses)
endif ()
add_subdirectory(external/rgr)

add_executable(myster-us-adventures
        main.c usual.c)
