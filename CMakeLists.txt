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
if (MSVC)
    add_subdirectory(external/pdcurses)
endif ()
add_subdirectory(external/rgr)

add_executable(myster-us-adventures
        usual.c test.c)
target_link_libraries(myster-us-adventures GameRGR2)
target_include_directories(myster-us-adventures PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)