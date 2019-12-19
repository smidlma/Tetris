cmake_minimum_required(VERSION 3.4)
project(Tetris)

# use address sanitizer to find memory problems
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")

find_package(SDL2)
if (SDL2_FOUND)
    message("SDL2 FOUND")
    add_executable(tetris main.c tetris.c game.c)
    target_include_directories(tetris PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2Mixer_INCLUDE_DIR})
    target_link_libraries(tetris ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_MIXER_LIBRARY} SDL2_ttf SDL2_mixer SDL2_image)
endif()