project(luminosc)

add_executable(${PROJECT_NAME}
    Main.cpp
)
target_link_libraries(${PROJECT_NAME} PRIVATE lumino_shader)
