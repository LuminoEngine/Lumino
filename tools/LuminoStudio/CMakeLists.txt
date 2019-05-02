cmake_minimum_required(VERSION 3.1.0)

set(LN_QT_ROOT "D:/Qt/5.12.3/msvc2017")
set(CMAKE_PREFIX_PATH ${LN_QT_ROOT})

project(LuminoStudio)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5OpenGL CONFIG REQUIRED)

# Populate a CMake variable with the sources
file(GLOB LuminoStudio_SOURCES "*.h" "*.cpp" "*.ui")
source_group("Sources" FILES ${LuminoStudio_SOURCES})

qt5_add_resources(LN_QT_RESOURCES
    Resources/breeze.qrc
)

# Tell CMake to create the ${PROJECT_NAME} executable
#add_executable(${PROJECT_NAME} WIN32 ${LuminoStudio_SOURCES} ${LN_QT_RESOURCES})
add_executable(${PROJECT_NAME} ${LuminoStudio_SOURCES} ${LN_QT_RESOURCES})
# Use the Widgets module from Qt 5
target_link_libraries(${PROJECT_NAME} LuminoEngine Qt5::Widgets Qt5::OpenGL)
ln_add_pch(${PROJECT_NAME} "PCH.h" "PCH.cpp")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${LN_QT_ROOT}/bin/Qt5Cored.dll" "Qt5Cored.dll"
    COMMAND ${CMAKE_COMMAND} -E copy "${LN_QT_ROOT}/bin/Qt5Widgetsd.dll" "Qt5Widgetsd.dll"
    COMMAND ${CMAKE_COMMAND} -E copy "${LN_QT_ROOT}/bin/Qt5Guid.dll" "Qt5Guid.dll"
    COMMAND ${CMAKE_COMMAND} -E copy "${LN_QT_ROOT}/bin/Qt5OpenGLd.dll" "Qt5OpenGLd.dll"
)