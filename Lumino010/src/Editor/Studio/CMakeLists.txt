cmake_minimum_required(VERSION 3.1.0)
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

file(GLOB LOCAL_SOURCES_Navigators "Navigators/*.h" "Navigators/*.cpp")
source_group("Navigators" FILES ${LOCAL_SOURCES_Navigators})

file(GLOB LOCAL_SOURCES_AssetBrowser AssetBrowser/*.h AssetBrowser/*.cpp)
source_group("AssetBrowser" FILES ${LOCAL_SOURCES_AssetBrowser})

file(GLOB LOCAL_SOURCES_SpriteFrameset "SpriteFrameset/*.h" "SpriteFrameset/*.cpp")
source_group("SpriteFrameset" FILES ${LOCAL_SOURCES_SpriteFrameset})

file(GLOB LOCAL_SOURCES_Widgets "Widgets/*.h" "Widgets/*.cpp")
source_group("Widgets" FILES ${LOCAL_SOURCES_Widgets})

file(GLOB LN_SOURCES_External_QtAwesome "External/QtAwesome/*.h" "External/QtAwesome/*.cpp")
source_group("External/QtAwesome" FILES ${LN_SOURCES_External_QtAwesome})

set(
	LOCAL_SOURCES
	${LuminoStudio_SOURCES}
	${LOCAL_SOURCES_Navigators}
	${LOCAL_SOURCES_AssetBrowser}
	${LOCAL_SOURCES_SpriteFrameset}
	${LOCAL_SOURCES_Widgets}
	${LN_SOURCES_External_QtAwesome}
)

qt5_add_resources(
	LN_QT_RESOURCES
	External/QtAwesome/QtAwesome.qrc
)

# Tell CMake to create the ${PROJECT_NAME} executable
add_executable(${PROJECT_NAME} ${LOCAL_SOURCES} ${LN_QT_RESOURCES} )
target_include_directories(${PROJECT_NAME} PRIVATE ${LUMINO_INCLUDE_DIRECTORIES})
target_link_libraries(${PROJECT_NAME} LuminoEngine-static EditorCore EditorCore Qt5::Widgets Qt5::OpenGL)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Editor")
ln_add_pch(${PROJECT_NAME} "PCH.h" "PCH.cpp")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${LN_QT_ROOT}/bin/Qt5Cored.dll" "Qt5Cored.dll"
    COMMAND ${CMAKE_COMMAND} -E copy "${LN_QT_ROOT}/bin/Qt5Widgetsd.dll" "Qt5Widgetsd.dll"
    COMMAND ${CMAKE_COMMAND} -E copy "${LN_QT_ROOT}/bin/Qt5Guid.dll" "Qt5Guid.dll"
    COMMAND ${CMAKE_COMMAND} -E copy "${LN_QT_ROOT}/bin/Qt5OpenGLd.dll" "Qt5OpenGLd.dll"
)
