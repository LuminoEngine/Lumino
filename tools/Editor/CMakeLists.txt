#
# For basic cmake file structure see: http://doc.qt.io/qt-5/cmake-manual.html
#
cmake_minimum_required(VERSION 2.8.11)

set(LN_QT_ROOT "D:/Qt/5.12.3/msvc2017")
set(CMAKE_PREFIX_PATH ${LN_QT_ROOT})

project(LuminoStudio)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the Qt libraries for Qt Quick/QML
find_package(Qt5 REQUIRED Qml Quick Gui)

# add the qml.qrc file
qt5_add_resources(qml_QRC src/qml.qrc)

# add source files
set(SOURCE 
	${SOURCE}
	src/main.cpp
	src/Squircle.cpp
	src/Squircle.h
)

add_executable(LuminoStudio ${SOURCE} ${qml_QRC})

# Use the Qml/Quick modules from Qt 5.
target_link_libraries(LuminoStudio LuminoEngine Qt5::Qml Qt5::Quick)


# post-build steps
# copy all the Qt support files to the debug/release directories
# You really only need the proper qt redistibution dll's/files, but this copies everything for easier development. Trim files out as needed.
message("Copying required Qt libraries and binaries to output directory....")
add_custom_command(TARGET LuminoStudio POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_PREFIX_PATH}\\bin $<TARGET_FILE_DIR:LuminoStudio>)

add_custom_command(TARGET LuminoStudio POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_PREFIX_PATH}\\plugins $<TARGET_FILE_DIR:LuminoStudio>)

add_custom_command(TARGET LuminoStudio POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_PREFIX_PATH}\\qml $<TARGET_FILE_DIR:LuminoStudio>)
