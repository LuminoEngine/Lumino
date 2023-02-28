
cmake_minimum_required(VERSION 3.0)
project(LuminoEngine)
include(cmake/LuminoConfig-LuminoEngine.cmake)

# デフォルトでは不足している include path を追加しておく
set_target_properties(Bullet3Common PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${LN_VCPKG_INSTALLED_DIR}/include/bullet")

set(LN_ENGINE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)

#-------------------------------------------------------------------------------
# Externals
#-------------------------------------------------------------------------------



#-------------------------------------------------------------------------------
# Files
#-------------------------------------------------------------------------------

file(GLOB LN_SOURCES_Base ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Base/*.hpp src/Base/*.cpp src/Base/*.hpp)

file(GLOB LN_SOURCES_Engine ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Engine/*.hpp src/Engine/*.cpp src/Engine/*.hpp)
set(LN_SOURCES_Engine
	${LN_SOURCES_Engine}
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/EngineContext.hpp
    src/EngineContext.cpp
    src/RegisterTypes.cpp
)


set(LN_SOURCES_Input
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Input/Common.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Input/Input.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Input/InputBinding.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Input/InputController.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Input/Mouse.hpp
    src/Input/GLFWInputDriver.cpp
    src/Input/GLFWInputDriver.hpp
    src/Input/Input.cpp
    src/Input/InputBinding.cpp
    src/Input/InputDriver.cpp
    src/Input/InputDriver.hpp
    src/Input/InputManager.cpp
    src/Input/InputManager.hpp
    src/Input/Internal.hpp
    src/Input/InputController.cpp
    src/Input/Mouse.cpp
    src/Input/Win32InputDriver.cpp
    src/Input/Win32InputDriver.hpp
    src/Input/Win32JoystickDriver.cpp
    src/Input/Win32JoystickDriver.hpp
)

file(GLOB LN_SOURCES_Audio ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Audio/*.hpp src/Audio/*.cpp src/Audio/*.hpp src/Audio/*.md)
file(GLOB LN_SOURCES_Audio_Decoder src/Audio/Decoder/*.cpp src/Audio/Decoder/*.hpp)
file(GLOB LN_SOURCES_Audio_ARIs src/Audio/ARIs/*.cpp src/Audio/ARIs/*.hpp src/Audio/ARIs/*.h)
file(GLOB LN_SOURCES_Audio_Backend src/Audio/Backend/*.cpp src/Audio/Backend/*.hpp)

file(GLOB LN_SOURCES_Shader ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Shader/*.hpp src/Shader/*.cpp src/Shader/*.hpp src/Shader/*.md)
file(GLOB LN_SOURCES_Shader_Lexer ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Shader/Lexer/*.hpp src/Shader/Lexer/*.cpp src/Shader/Lexer/*.hpp src/Shader/Lexer/*.md)



file(GLOB LN_SOURCES_PostEffect ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/PostEffect/*.hpp src/PostEffect/*.cpp src/PostEffect/*.hpp)

file(GLOB LN_SOURCES_Effect ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Effect/*.hpp src/Effect/*.cpp src/Effect/*.hpp src/Effect/*.md)

file(GLOB LN_SOURCES_Physics ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Physics/*.hpp src/Physics/*.cpp src/Physics/*.hpp  src/Physics/*.md)

file(GLOB LN_SOURCES_Asset ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Asset/*.hpp src/Asset/*.cpp src/Asset/*.hpp src/Asset/*.md)

set(LN_SOURCES_Visual
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Visual/Common.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Visual/CameraComponent.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Visual/LightComponent.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Visual/SpriteComponent.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Visual/VisualComponent.hpp
    src/Visual/Internal.hpp
    src/Visual/CameraComponent.cpp
    src/Visual/LightComponent.cpp
    src/Visual/SpriteComponent.cpp
    src/Visual/VisualComponent.cpp
    src/Visual/VisualManager.cpp
    src/Visual/VisualManager.hpp
)

file(GLOB LN_SOURCES_Scene ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Scene/*.hpp src/Scene/*.cpp src/Scene/*.hpp)
file(GLOB LN_SOURCES_Scene_Text ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Scene/Text/*.hpp src/Scene/Text/*.cpp src/Scene/Text/*.hpp)
file(GLOB LN_SOURCES_Scene_Shapes ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Scene/Shapes/*.hpp src/Scene/Shapes/*.cpp src/Scene/Shapes/*.hpp)
file(GLOB LN_SOURCES_Scene_Mesh ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Scene/Mesh/*.hpp src/Scene/Mesh/*.cpp src/Scene/Mesh/*.hpp)
file(GLOB LN_SOURCES_Scene_MeshTilemap ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Scene/MeshTilemap/*.hpp src/Scene/MeshTilemap/*.cpp src/Scene/MeshTilemap/*.hpp)
file(GLOB LN_SOURCES_Scene_MeshVoxelmap ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Scene/MeshVoxelmap/*.hpp src/Scene/MeshVoxelmap/*.cpp src/Scene/MeshVoxelmap/*.hpp)
file(GLOB LN_SOURCES_Scene_Reflection ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Scene/Reflection/*.hpp src/Scene/Reflection/*.cpp src/Scene/Reflection/*.hpp)
file(GLOB LN_SOURCES_Scene_Physics ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Scene/Physics/*.hpp src/Scene/Physics/*.cpp src/Scene/Physics/*.hpp)
file(GLOB LN_SOURCES_Scene_Effect ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Scene/Effect/*.hpp src/Scene/Effect/*.cpp src/Scene/Effect/*.hpp)
file(GLOB LN_SOURCES_Scene_Sprite ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Scene/Sprite/*.hpp src/Scene/Sprite/*.cpp src/Scene/Sprite/*.hpp)

file(GLOB LN_SOURCES_UI ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/UI/*.hpp src/UI/*.cpp src/UI/*.hpp)
file(GLOB LN_SOURCES_UI_Controls ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/UI/Controls/*.hpp src/UI/Controls/*.cpp src/UI/Controls/*.hpp)
file(GLOB LN_SOURCES_UI_Layout ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/UI/Layout/*.hpp src/UI/Layout/*.cpp src/UI/Layout/*.hpp)

# set(LN_SOURCES_UI_imgui
#     ${LUMINO_REPO_ROOT_DIR}/build/BuildCache/imgui/imconfig.h
#     ${LUMINO_REPO_ROOT_DIR}/build/BuildCache/imgui/imgui.cpp
#     ${LUMINO_REPO_ROOT_DIR}/build/BuildCache/imgui/imgui.h
#     ${LUMINO_REPO_ROOT_DIR}/build/BuildCache/imgui/imgui_draw.cpp
#     ${LUMINO_REPO_ROOT_DIR}/build/BuildCache/imgui/imgui_internal.h
#     ${LUMINO_REPO_ROOT_DIR}/build/BuildCache/imgui/imgui_tables.cpp
#     ${LUMINO_REPO_ROOT_DIR}/build/BuildCache/imgui/imgui_widgets.cpp
#     ${LUMINO_REPO_ROOT_DIR}/build/BuildCache/imgui/imstb_rectpack.h
#     ${LUMINO_REPO_ROOT_DIR}/build/BuildCache/imgui/imstb_textedit.h
#     ${LUMINO_REPO_ROOT_DIR}/build/BuildCache/imgui/imstb_truetype.h
# )
set(LN_IMGUI_INCLUDE_DIRECTORY ${LUMINO_REPO_ROOT_DIR}/build/BuildCache/imgui)
file(GLOB LN_SOURCES_Tilemap ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Tilemap/*.hpp src/Tilemap/*.cpp src/Tilemap/*.hpp)

file(GLOB LN_SOURCES_Scripting ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Scripting/*.hpp src/Scripting/*.cpp src/Scripting/*.hpp src/Scripting/*.md)

file(GLOB LN_SOURCES_Runtime
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Runtime/*.h
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Runtime/*.hpp
    src/Runtime/*.cpp
    src/Runtime/*.hpp
    src/Runtime/*.h
)

file(GLOB LN_SOURCES_Editor ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Editor/*.hpp src/Editor/*.cpp src/Editor/*.hpp src/Editor/*.md)

if (LN_USE_VULKAN)
    if (WIN32)
        list(APPEND LN_PLATFORM_DEFINITIONS LN_USE_VULKAN=1)
    endif()
endif()



# Grouping.
source_group("Base" FILES ${LN_SOURCES_Base})
source_group("Engine" FILES ${LN_SOURCES_Engine})
source_group("Input" FILES ${LN_SOURCES_Input})
source_group("Grammar" FILES ${LN_SOURCES_Grammar})
source_group("Audio" FILES ${LN_SOURCES_Audio})
source_group("Audio/Decoder" FILES ${LN_SOURCES_Audio_Decoder})
source_group("Audio/Backend" FILES ${LN_SOURCES_Audio_Backend})
source_group("Audio/ARIs" FILES ${LN_SOURCES_Audio_ARIs})
source_group("Shader" FILES ${LN_SOURCES_Shader})
source_group("Shader/Lexer" FILES ${LN_SOURCES_Shader_Lexer})
source_group("PostEffect" FILES ${LN_SOURCES_PostEffect})
source_group("Effect" FILES ${LN_SOURCES_Effect})
source_group("Physics" FILES ${LN_SOURCES_Physics})
source_group("Asset" FILES ${LN_SOURCES_Asset})
source_group("Visual" FILES ${LN_SOURCES_Visual})
source_group("Scene" FILES ${LN_SOURCES_Scene})
source_group("Scene/Text" FILES ${LN_SOURCES_Scene_Text})
source_group("Scene/Shapes" FILES ${LN_SOURCES_Scene_Shapes})
source_group("Scene/Mesh" FILES ${LN_SOURCES_Scene_Mesh})
source_group("Scene/MeshTilemap" FILES ${LN_SOURCES_Scene_MeshTilemap})
source_group("Scene/MeshVoxelmap" FILES ${LN_SOURCES_Scene_MeshVoxelmap})
source_group("Scene/Reflection" FILES ${LN_SOURCES_Scene_Reflection})
source_group("Scene/Physics" FILES ${LN_SOURCES_Scene_Physics})
source_group("Scene/Effect" FILES ${LN_SOURCES_Scene_Effect})
source_group("Scene/Sprite" FILES ${LN_SOURCES_Scene_Sprite})
source_group("UI" FILES ${LN_SOURCES_UI})
source_group("UI/Controls" FILES ${LN_SOURCES_UI_Controls})
source_group("UI/Layout" FILES ${LN_SOURCES_UI_Layout})
# source_group("UI/imgui" FILES ${LN_SOURCES_UI_imgui})
source_group("Tilemap" FILES ${LN_SOURCES_Tilemap})
source_group("Scripting" FILES ${LN_SOURCES_Scripting})
source_group("Runtime" FILES ${LN_SOURCES_Runtime})
source_group("Editor" FILES ${LN_SOURCES_Editor})

set(LN_SOURCES
    src/LuminoEngine.PCH.h
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine.hpp
    ${LN_ENGINE_INCLUDE_DIR}/Lumino.hpp
    ${LN_SOURCES_Base}
    ${LN_SOURCES_Engine}
    ${LN_SOURCES_Grammar}
    ${LN_SOURCES_Input}
    ${LN_SOURCES_Audio}
    ${LN_SOURCES_Audio_Decoder}
    ${LN_SOURCES_Audio_Backend}
    ${LN_SOURCES_Audio_ARIs}
    ${LN_SOURCES_Shader}
    ${LN_SOURCES_Shader_Lexer}
    ${LN_SOURCES_TextRendering}
    ${LN_SOURCES_PostEffect}
    ${LN_SOURCES_Effect}
    ${LN_SOURCES_Physics}
    ${LN_SOURCES_Asset}
    ${LN_SOURCES_Visual}
    ${LN_SOURCES_Scene}
    ${LN_SOURCES_Scene_Text}
    ${LN_SOURCES_Scene_Shapes}
    ${LN_SOURCES_Scene_Mesh}
    ${LN_SOURCES_Scene_MeshTilemap}
    ${LN_SOURCES_Scene_MeshVoxelmap}
    ${LN_SOURCES_Scene_Reflection}
    ${LN_SOURCES_Scene_Physics}
    ${LN_SOURCES_Scene_Effect}
    ${LN_SOURCES_Scene_Sprite}
    ${LN_SOURCES_UI}
    ${LN_SOURCES_UI_Controls}
    ${LN_SOURCES_UI_Layout}
    # ${LN_SOURCES_UI_imgui}
    ${LN_SOURCES_Tilemap}
    ${LN_SOURCES_Scripting}
    ${LN_SOURCES_Runtime}
    ${LN_SOURCES_Editor}
)

set(LN_SOURCES_SharedLib
    src/SharedLibraryMain.cpp
)

#-------------------------------------------------------------------------------
# Configuration
#-------------------------------------------------------------------------------

if (LN_USE_FBX_IMPORTER)
    list(APPEND LN_PLATFORM_DEFINITIONS LN_USE_FBX_IMPORTER=1)
endif()

if (LN_EMSCRIPTEN)
    list(APPEND LN_PLATFORM_DEFINITIONS LN_USE_OPENAL=1)

elseif (LN_IOS)

elseif (WIN32)
    #list(APPEND LN_PLATFORM_LIBS glfw GLAD)
    #list(APPEND LN_PLATFORM_LIBS user32 gdi32 winmm imm32 ole32 oleaut32 version uuid)
    list(APPEND LN_PLATFORM_LIBS winmm imm32 version)
    #list(APPEND LN_PLATFORM_LIBS OpenAL)
    if (LN_USE_FBX_IMPORTER)
        list(APPEND LN_PLATFORM_LIBS LN::FBX)
    endif()
    

    #if (LN_USE_VULKAN)
    #    list(APPEND LN_PLATFORM_LIBS VulkanImported)
    #endif()

elseif (APPLE)
    #list(APPEND LN_PLATFORM_LIBS glfw GLAD)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(CoreVideo_LIBRARY CoreVideo)
    find_library(IOKit_LIBRARY IOKit)
    #find_library(OpenAL_LIBRARY OpenAL)
    
    #find_package(OpenAL)
    list(APPEND LN_PLATFORM_LIBS ${COCOA_LIBRARY})
    list(APPEND LN_PLATFORM_LIBS ${CoreVideo_LIBRARY})
    list(APPEND LN_PLATFORM_LIBS ${IOKit_LIBRARY})
    #list(APPEND LN_PLATFORM_LIBS ${OpenAL_LIBRARY})
    #list(APPEND LN_PLATFORM_LIBS OpenAL)
    
    list(APPEND LN_PLATFORM_LIBS "-framework OpenAL")
    list(APPEND LN_PLATFORM_LIBS "-framework Accelerate")
    
    #list(APPEND LN_PLATFORM_LIBS SDL2)
    
    
elseif (UNIX)
    if (LN_ANDROID)
    else()
        #list(APPEND LN_PLATFORM_LIBS glfw GLAD OpenAL)
    endif()
endif()

if (LN_USE_SDL)
    #list(APPEND LN_PLATFORM_LIBS SDL2)
    list(APPEND LN_PLATFORM_DEFINITIONS LN_USE_SDL=1)
endif()
if (LN_USE_OPENAL)
    list(APPEND LN_PLATFORM_DEFINITIONS LN_USE_OPENAL=1)
endif()


#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------

set(
    _LN_ENGINE_LINK_LIBS
    LuminoCore
    LuminoRuntime
    LuminoPlatform
    LuminoGraphics
    LuminoGraphicsRHI
    #pcre
    pcre2_32
    Vorbis::vorbis
    Vorbis::vorbisenc
    Vorbis::vorbisfile
    box2d::box2d
    LinearMath
    Bullet3Common
    BulletCollision
    BulletDynamics
    BulletSoftBody
    #tmxlite
    toml11::toml11
    imgui::imgui
    ${LN_PLATFORM_LIBS}
)

if (LN_USE_Effekseer)
    find_package(Vulkan REQUIRED)
    list(APPEND _LN_ENGINE_LINK_LIBS ${Vulkan_LIBRARIES} Effekseer EffekseerRendererVulkan LLGI)   # for LLGI
endif()

add_library(LuminoEngine STATIC ${LN_SOURCES})
add_library(lumino::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(LuminoEngine PUBLIC  
    $<BUILD_INTERFACE:${LN_ENGINE_INCLUDE_DIR}>
)
target_include_directories(LuminoEngine PRIVATE ${LN_ENGINE_INCLUDE_DIR} "${LUMINO_INCLUDE_DIRECTORIES}" "src" ${LN_DEPENDENCIES_INCLUDE_DIRECTORIES} ${glslang_INCLUDE_DIRS} ${spirv-cross_INCLUDE_DIRS} ${LN_IMGUI_INCLUDE_DIRECTORY})
#target_compile_definitions(LuminoEngine PUBLIC LUMINO_BUILD_DLL=1)
target_compile_definitions(LuminoEngine PRIVATE ${LN_PLATFORM_DEFINITIONS})
target_link_libraries(LuminoEngine INTERFACE LuminoCore ${glslang_LIBRARIES} ${spirv-cross_LIBRARIES})
target_link_libraries(LuminoEngine PRIVATE ${_LN_ENGINE_LINK_LIBS})
target_link_libraries(LuminoEngine INTERFACE LuminoPlatform LuminoRuntime LuminoGraphics)

if (LN_OS_DESKTOP)
    target_include_directories(LuminoEngine PRIVATE ${VULKAN_HEADERS_INCLUDE_DIRS})
    target_include_directories(LuminoEngine PRIVATE ${STB_INCLUDE_DIRS})
    target_include_directories(LuminoEngine PRIVATE ${TINYGLTF_INCLUDE_DIRS})
    target_link_libraries(LuminoEngine PRIVATE glfw glad::glad)
    target_link_libraries(LuminoEngine PRIVATE OpenAL::OpenAL)
endif()

if (LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER)
    target_compile_definitions(LuminoEngine PRIVATE LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER=1)
endif()

#target_link_libraries(LuminoEngine PRIVATE glslang)
set_target_properties(LuminoEngine PROPERTIES FOLDER "Lumino")
target_precompile_headers(LuminoEngine PRIVATE "src/LuminoEngine.PCH.h")

if (LN_EMSCRIPTEN)
    set_target_properties(LuminoEngine PROPERTIES COMPILE_FLAGS "-pthread")
    set_target_properties(LuminoEngine PROPERTIES LINK_FLAGS "-s USE_SDL=2")
endif()

if (LN_BUILD_XCODE_FRAMEWORK)
    # Build as framework.
    set_target_properties(LuminoEngine PROPERTIES
        FRAMEWORK TRUE
        #VERSION 1.0.0
        #SOVERSION 1.0.0
        MACOSX_FRAMEWORK_IDENTIFIER lib.Lumino
        BUILD_WITH_INSTALL_RPATH 1
        XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMIRY "1,2"
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer")
    # Do not use PUBLIC_HEADERS.
    # Since PUBLIC_HEADERS does not maintain the folder tree,
    # install the header file with the install command.
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/Lumino DESTINATION "LuminoEngine.framework/Headers")
    install(FILES ${PROJECT_SOURCE_DIR}/include/LuminoEngine.hpp DESTINATION "LuminoEngine.framework/Headers")
endif()

# Unit test.
if (LN_BUILD_TESTS)
    add_subdirectory(test)
    add_subdirectory(sandbox)
endif()

# Shared library
if (LN_BUILD_SHARED_LIBRARY)
    add_library(LuminoEngine SHARED ${LN_SOURCES_SharedLib})
    target_include_directories(LuminoEngine PRIVATE ${LN_ENGINE_INCLUDE_DIR} "${LUMINO_INCLUDE_DIRECTORIES}" "src" ${LN_DEPENDENCIES_INCLUDE_DIRECTORIES} ${glslang_INCLUDE_DIRS} ${spirv-cross_INCLUDE_DIRS} ${LN_IMGUI_INCLUDE_DIRECTORY})
    target_compile_definitions(LuminoEngine PRIVATE LUMINO_BUILD_DLL=1)
    target_link_libraries(LuminoEngine PRIVATE LuminoEngine)
    set_target_properties(LuminoEngine PROPERTIES FOLDER "Lumino")

    # Export symbols defined in another static library.
    if (MSVC)
        set_target_properties(LuminoEngine PROPERTIES LINK_FLAGS_DEBUG "/WHOLEARCHIVE:LuminoEngined")
        set_target_properties(LuminoEngine PROPERTIES LINK_FLAGS_RELEASE "/WHOLEARCHIVE:LuminoEngine")
    else()
        set_target_properties(LuminoEngine PROPERTIES LINK_FLAGS_DEBUG "--no-whole-archive")
        set_target_properties(LuminoEngine PROPERTIES LINK_FLAGS_RELEASE "--no-whole-archive")
    endif()
    
    if (LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER)
        target_compile_definitions(LuminoEngine PRIVATE LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER=1)
    endif()
endif()


#-------------------------------------------------------------------------------
# Installation
#-------------------------------------------------------------------------------
if (LUMINO_INSTALL)
    install(
        TARGETS     LuminoEngine
        EXPORT      LuminoExport
    )
    install(DIRECTORY include DESTINATION .)
    install(FILES cmake/LuminoConfig-${PROJECT_NAME}.cmake DESTINATION ${LN_EXPORT_DESTINATION_DIR})
endif()

#-------------------------------------------------------------------------------
# Tests
#-------------------------------------------------------------------------------

if (LUMINO_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
