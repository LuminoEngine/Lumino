project(luminosc)

find_package(CLI11 CONFIG REQUIRED)

add_executable(${PROJECT_NAME}
    Main.cpp
)
target_link_libraries(${PROJECT_NAME} PRIVATE LuminoShader CLI11::CLI11)
