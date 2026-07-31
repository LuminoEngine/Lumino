project(luminosc)

find_package(CLI11 CONFIG REQUIRED)

add_executable(${PROJECT_NAME}
    Main.cpp
)
target_link_libraries(${PROJECT_NAME} PRIVATE LuminoShader CLI11::CLI11)

# WGSL 検証は Dawn を使うため、実行時に webgpu_dawn.dll が必要になる。
if(LUMINO_SHADER_VALIDATE_WGSL)
    target_copy_webgpu_binaries(${PROJECT_NAME})
endif()
