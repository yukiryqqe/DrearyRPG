cmake_minimum_required(VERSION 3.10)

project(DrearyRPG VERSION 0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/map
    ${PROJECT_SOURCE_DIR}/UI
    ${PROJECT_SOURCE_DIR}/player
)

set(SOURCES
    game.c
    map/map.c
    UI/ui.c
    player/player.c
)

set(HEADERS
    game.h
    map/map.h
    UI/ui.h
    player/player.h
)

add_executable(DrearyRPG ${SOURCES} ${HEADERS})

target_link_libraries(DrearyRPG PRIVATE /usr/lib/x86_64-linux-gnu/libncursesw.so)

target_compile_options(DrearyRPG PRIVATE -Wall -Wextra -pedantic)

