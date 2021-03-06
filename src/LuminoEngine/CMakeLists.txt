
cmake_minimum_required(VERSION 3.0)
project(LuminoEngine)

include(../LuminoCore/LuminoCoreSources.cmake)

set(LN_ENGINE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/../../include)

#------------------------------------------------------------------------------
# Files

file(GLOB LN_SOURCES_Base ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Base/*.hpp src/Base/*.cpp src/Base/*.hpp)

file(GLOB LN_SOURCES_Engine ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Engine/*.hpp src/Engine/*.cpp src/Engine/*.hpp)
set(LN_SOURCES_Engine
	${LN_SOURCES_Engine}
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/EngineContext.hpp
    src/EngineContext.cpp
)

file(GLOB LN_SOURCES_Platform ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Platform/*.hpp src/Platform/*.cpp src/Platform/*.hpp)

set(LN_SOURCES_Grammar
    src/Grammar/Internal.hpp
    src/Grammar/AbstractLexer.cpp
    src/Grammar/AbstractLexer.hpp
    src/Grammar/CppLexer.cpp
    src/Grammar/CppLexer.hpp
    src/Grammar/Token.hpp
)

file(GLOB LN_SOURCES_Animation ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Animation/*.hpp src/Animation/*.cpp src/Animation/*.hpp)

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

file(GLOB LN_SOURCES_Graphics ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Graphics/*.hpp src/Graphics/*.cpp src/Graphics/*.hpp)
file(GLOB LN_SOURCES_Graphics_RHIs ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Graphics/RHIs/*.hpp src/Graphics/RHIs/*.cpp src/Graphics/RHIs/*.hpp)

set(LN_SOURCES_Font
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Font/Common.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Font/Font.hpp
    src/Font/Internal.hpp
    src/Font/Font.cpp
    src/Font/FontCore.cpp
    src/Font/FontCore.hpp
    src/Font/FontGlyphCache.cpp
    src/Font/FontGlyphCache.hpp
    src/Font/FontManager.cpp
    src/Font/FontManager.hpp
    src/Font/FreeTypeFont.cpp
    src/Font/FreeTypeFont.hpp
    src/Font/TextLayoutEngine.cpp
    src/Font/TextLayoutEngine.hpp
)

file(GLOB LN_SOURCES_Mesh ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Mesh/*.hpp src/Mesh/*.cpp src/Mesh/*.hpp)

file(GLOB LN_SOURCES_Rendering ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Rendering/*.hpp src/Rendering/*.cpp src/Rendering/*.hpp src/Rendering/*.md)
file(GLOB LN_SOURCES_Rendering_RLIs src/Rendering/RLIs/*.cpp src/Rendering/RLIs/*.hpp)
file(GLOB LN_SOURCES_Rendering_RenderFeature ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Rendering/RenderFeature/*.hpp src/Rendering/RenderFeature/*.cpp src/Rendering/RenderFeature/*.hpp)

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


file(GLOB LN_SOURCES_UI ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/UI/*.hpp src/UI/*.cpp src/UI/*.hpp)
file(GLOB LN_SOURCES_UI_Controls ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/UI/Controls/*.hpp src/UI/Controls/*.cpp src/UI/Controls/*.hpp)
file(GLOB LN_SOURCES_UI_Layout ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/UI/Layout/*.hpp src/UI/Layout/*.cpp src/UI/Layout/*.hpp)

set(LN_SOURCES_UI_imgui
    ${LUMINO_ROOT_PATH}/build/BuildCache/imgui/imconfig.h
    ${LUMINO_ROOT_PATH}/build/BuildCache/imgui/imgui.cpp
    ${LUMINO_ROOT_PATH}/build/BuildCache/imgui/imgui.h
    ${LUMINO_ROOT_PATH}/build/BuildCache/imgui/imgui_draw.cpp
    ${LUMINO_ROOT_PATH}/build/BuildCache/imgui/imgui_internal.h
    ${LUMINO_ROOT_PATH}/build/BuildCache/imgui/imgui_tables.cpp
    ${LUMINO_ROOT_PATH}/build/BuildCache/imgui/imgui_widgets.cpp
    ${LUMINO_ROOT_PATH}/build/BuildCache/imgui/imstb_rectpack.h
    ${LUMINO_ROOT_PATH}/build/BuildCache/imgui/imstb_textedit.h
    ${LUMINO_ROOT_PATH}/build/BuildCache/imgui/imstb_truetype.h
)
set(LN_IMGUI_INCLUDE_DIRECTORY ${LUMINO_ROOT_PATH}/build/BuildCache/imgui)
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
        file(GLOB LN_SOURCES_Graphics_RHIs_Vulkan src/Graphics/RHIs/Vulkan/*.cpp src/Graphics/RHIs/Vulkan/*.hpp)
        list(APPEND LN_PLATFORM_DEFINITIONS LN_USE_VULKAN=1)
    endif()
endif()


if (LN_EMSCRIPTEN)
elseif (WIN32)
    file(GLOB LN_SOURCES_Graphics_RHIs_DirectX12 src/Graphics/RHIs/DirectX12/*.cpp src/Graphics/RHIs/DirectX12/*.hpp)
    set(LN_SOURCES_Platform
        ${LN_SOURCES_Platform}
        ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Platform/Win32PlatformInterface.hpp
        src/Platform/Windows/Win32PlatformInterface.cpp
        src/Platform/Windows/Win32PlatformWindowManager.hpp
        src/Platform/Windows/Win32PlatformWindowManager.cpp
    )
elseif (APPLE)
    set(LN_SOURCES_Platform
        ${LN_SOURCES_Platform}
        src/Platform/iOS/iOSPlatformInterface.mm
        src/Platform/macOS/CocoaPlatformInterface.mm
        src/Platform/macOS/PlatformSupport.mm
    )
endif()

# Grouping.
source_group("Base" FILES ${LN_SOURCES_Base})
source_group("Engine" FILES ${LN_SOURCES_Engine})
source_group("Platform" FILES ${LN_SOURCES_Platform})
source_group("Input" FILES ${LN_SOURCES_Input})
source_group("Grammar" FILES ${LN_SOURCES_Grammar})
source_group("Animation" FILES ${LN_SOURCES_Animation})
source_group("Audio" FILES ${LN_SOURCES_Audio})
source_group("Audio\\Decoder" FILES ${LN_SOURCES_Audio_Decoder})
source_group("Audio\\Backend" FILES ${LN_SOURCES_Audio_Backend})
source_group("Audio\\ARIs" FILES ${LN_SOURCES_Audio_ARIs})
source_group("Shader" FILES ${LN_SOURCES_Shader})
source_group("Graphics" FILES ${LN_SOURCES_Graphics})
source_group("Graphics\\RHIs" FILES ${LN_SOURCES_Graphics_RHIs})
source_group("Graphics\\RHIs\\Vulkan" FILES ${LN_SOURCES_Graphics_RHIs_Vulkan})
source_group("Graphics\\RHIs\\DirectX12" FILES ${LN_SOURCES_Graphics_RHIs_DirectX12})
source_group("Font" FILES ${LN_SOURCES_Font})
source_group("Mesh" FILES ${LN_SOURCES_Mesh})
source_group("Rendering" FILES ${LN_SOURCES_Rendering})
source_group("Rendering\\RLIs" FILES ${LN_SOURCES_Rendering_RLIs})
source_group("Rendering\\RenderFeature" FILES ${LN_SOURCES_Rendering_RenderFeature})
source_group("PostEffect" FILES ${LN_SOURCES_PostEffect})
source_group("Effect" FILES ${LN_SOURCES_Effect})
source_group("Physics" FILES ${LN_SOURCES_Physics})
source_group("Asset" FILES ${LN_SOURCES_Asset})
source_group("Visual" FILES ${LN_SOURCES_Visual})
source_group("Scene" FILES ${LN_SOURCES_Scene})
source_group("Scene\\Text" FILES ${LN_SOURCES_Scene_Text})
source_group("Scene\\Shapes" FILES ${LN_SOURCES_Scene_Shapes})
source_group("Scene\\Mesh" FILES ${LN_SOURCES_Scene_Mesh})
source_group("Scene\\MeshTilemap" FILES ${LN_SOURCES_Scene_MeshTilemap})
source_group("Scene\\MeshVoxelmap" FILES ${LN_SOURCES_Scene_MeshVoxelmap})
source_group("Scene\\Reflection" FILES ${LN_SOURCES_Scene_Reflection})
source_group("Scene\\Physics" FILES ${LN_SOURCES_Scene_Physics})
source_group("Scene\\Effect" FILES ${LN_SOURCES_Scene_Effect})
source_group("UI" FILES ${LN_SOURCES_UI})
source_group("UI\\Controls" FILES ${LN_SOURCES_UI_Controls})
source_group("UI\\Layout" FILES ${LN_SOURCES_UI_Layout})
source_group("UI\\imgui" FILES ${LN_SOURCES_UI_imgui})
source_group("Tilemap" FILES ${LN_SOURCES_Tilemap})
source_group("Scripting" FILES ${LN_SOURCES_Scripting})
source_group("Runtime" FILES ${LN_SOURCES_Runtime})
source_group("Editor" FILES ${LN_SOURCES_Editor})

set(LN_SOURCES
    src/LuminoEngine.PCH.cpp
    src/LuminoEngine.PCH.h
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine.hpp
    ${LN_ENGINE_INCLUDE_DIR}/Lumino.hpp
    ${LN_SOURCES_Base}
    ${LN_SOURCES_Engine}
    ${LN_SOURCES_Platform}
    ${LN_SOURCES_Grammar}
    ${LN_SOURCES_Animation}
    ${LN_SOURCES_Input}
    ${LN_SOURCES_Audio}
    ${LN_SOURCES_Audio_Decoder}
    ${LN_SOURCES_Audio_Backend}
    ${LN_SOURCES_Audio_ARIs}
    ${LN_SOURCES_Shader}
    ${LN_SOURCES_Graphics}
    ${LN_SOURCES_Graphics_RHIs}
    ${LN_SOURCES_Graphics_RHIs_Vulkan}
    ${LN_SOURCES_Graphics_RHIs_DirectX12}
    ${LN_SOURCES_Font}
    ${LN_SOURCES_Mesh}
    ${LN_SOURCES_Rendering}
    ${LN_SOURCES_Rendering_RLIs}
    ${LN_SOURCES_Rendering_RenderFeature}
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
    ${LN_SOURCES_UI}
    ${LN_SOURCES_UI_Controls}
    ${LN_SOURCES_UI_Layout}
    ${LN_SOURCES_UI_imgui}
    ${LN_SOURCES_Tilemap}
    ${LN_SOURCES_Scripting}
    ${LN_SOURCES_Runtime}
    ${LN_SOURCES_Editor}
    ${LN_COMMON_META_FILES}
)

set(LN_SOURCES_SharedLib
    src/SharedLibraryMain.cpp
)


if (LN_USE_FBX_IMPORTER)
    list(APPEND LN_PLATFORM_DEFINITIONS LN_USE_FBX_IMPORTER=1)
endif()

if (LN_EMSCRIPTEN)
    list(APPEND LN_PLATFORM_DEFINITIONS LN_USE_OPENAL=1)

elseif (LN_IOS)

elseif (WIN32)
    list(APPEND LN_PLATFORM_LIBS glfw GLAD)
    #list(APPEND LN_PLATFORM_LIBS user32 gdi32 winmm imm32 ole32 oleaut32 version uuid)
    list(APPEND LN_PLATFORM_LIBS winmm imm32 version)
    list(APPEND LN_PLATFORM_LIBS OpenAL)
    if (LN_USE_FBX_IMPORTER)
        list(APPEND LN_PLATFORM_LIBS LN::FBX)
    endif()
    
    
    #if (LN_USE_VULKAN)
    #    list(APPEND LN_PLATFORM_LIBS VulkanImported)
    #endif()

elseif (APPLE)
    list(APPEND LN_PLATFORM_LIBS glfw GLAD)
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
        list(APPEND LN_PLATFORM_LIBS glfw GLAD OpenAL)
    endif()
endif()

if (LN_USE_SDL)
    list(APPEND LN_PLATFORM_LIBS SDL2)
    list(APPEND LN_PLATFORM_DEFINITIONS LN_USE_SDL=1)
endif()
if (LN_USE_OPENAL)
    list(APPEND LN_PLATFORM_DEFINITIONS LN_USE_OPENAL=1)
endif()


#------------------------------------------------------------------------------
# Targets

set(
    _LN_ENGINE_LINK_LIBS
    LuminoCore
    pcre
    PNG
    ZLIB
    FreeType
    vorbisfile
    vorbis
    ogg
    Box2D
    BulletSoftBody
    BulletDynamics
    BulletCollision
    LinearMath
    tmxlite
    yamlcpp
    VulkanHeaders
    nanovg
    ${LN_PLATFORM_LIBS}
)

if (LN_USE_Effekseer)
    find_package(Vulkan REQUIRED)
    list(APPEND _LN_ENGINE_LINK_LIBS ${Vulkan_LIBRARIES} Effekseer EffekseerRendererVulkan LLGI)   # for LLGI
endif()

add_library(LuminoEngine-static STATIC ${LN_SOURCES})
target_include_directories(LuminoEngine-static PUBLIC  
    $<BUILD_INTERFACE:${LN_ENGINE_INCLUDE_DIR}>
)
target_include_directories(LuminoEngine-static PRIVATE ${LN_ENGINE_INCLUDE_DIR} "${LUMINO_INCLUDE_DIRECTORIES}" "src" ${LN_DEPENDENCIES_INCLUDE_DIRECTORIES} ${glslang_INCLUDE_DIRS} ${spirv-cross_INCLUDE_DIRS} ${LN_IMGUI_INCLUDE_DIRECTORY})
#target_compile_definitions(LuminoEngine-static PUBLIC LUMINO_BUILD_DLL=1)
target_compile_definitions(LuminoEngine-static PRIVATE ${LN_PLATFORM_DEFINITIONS})
target_link_libraries(LuminoEngine-static INTERFACE LuminoCore ${glslang_LIBRARIES} ${spirv-cross_LIBRARIES})
target_link_libraries(LuminoEngine-static PRIVATE ${_LN_ENGINE_LINK_LIBS})

#target_link_libraries(LuminoEngine-static PRIVATE glslang)
set_target_properties(LuminoEngine-static PROPERTIES FOLDER "Lumino")
ln_add_pch(LuminoEngine-static "src/LuminoEngine.PCH.h" "src/LuminoEngine.PCH.cpp")

if (LN_EMSCRIPTEN)
    set_target_properties(LuminoEngine-static PROPERTIES COMPILE_FLAGS "-pthread")
    set_target_properties(LuminoEngine-static PROPERTIES LINK_FLAGS "-s USE_SDL=2")
endif()

if (LN_BUILD_XCODE_FRAMEWORK)
    # Build as framework.
    set_target_properties(LuminoEngine-static PROPERTIES
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

if (LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER)
    target_compile_definitions(LuminoEngine-static PRIVATE LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER=1)
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
    target_link_libraries(LuminoEngine PRIVATE LuminoEngine-static)
    set_target_properties(LuminoEngine PROPERTIES FOLDER "Lumino")

    # Export symbols defined in another static library.
    if (MSVC)
        set_target_properties(LuminoEngine PROPERTIES LINK_FLAGS_DEBUG "/WHOLEARCHIVE:LuminoEngine-staticd")
        set_target_properties(LuminoEngine PROPERTIES LINK_FLAGS_RELEASE "/WHOLEARCHIVE:LuminoEngine-static")
    else()
        set_target_properties(LuminoEngine PROPERTIES LINK_FLAGS_DEBUG "--no-whole-archive")
        set_target_properties(LuminoEngine PROPERTIES LINK_FLAGS_RELEASE "--no-whole-archive")
    endif()
    
    if (LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER)
        target_compile_definitions(LuminoEngine PRIVATE LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER=1)
    endif()
endif()


#------------------------------------------------------------------------------
# Install

install(TARGETS LuminoEngine-static
    EXPORT LuminoEngineTargets
    ARCHIVE     DESTINATION lib         # Static-library or MSVC-Link-library installation location (Copied at "INSTALL" build time)
    INCLUDES    DESTINATION ./include   # Include directory (File is not copied)
    RUNTIME     DESTINATION bin
    FRAMEWORK   DESTINATION .)
    
install(DIRECTORY ../../include/LuminoEngine DESTINATION ./include)
install(FILES ../../include/Lumino.hpp DESTINATION include)
install(FILES ../../include/LuminoEngine.hpp DESTINATION include)

install(
    EXPORT LuminoEngineTargets
    FILE LuminoEngineTargets.cmake
    DESTINATION lib/cmake/LuminoEngine
    EXPORT_LINK_INTERFACE_LIBRARIES)

install(
    FILES LuminoEngineConfig.cmake
    DESTINATION lib/cmake/LuminoEngine)

#add_custom_command(TARGET LuminoEngine-static POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:LuminoEngine-static> ../../../../lib/${LN_TARGET_ARCH})

#------------------------------------------------------------------------------
# Install

# Shared library
if (LN_BUILD_SHARED_LIBRARY)
    install(TARGETS LuminoEngine
        EXPORT ${PROJECT_NAME}Targets
        RUNTIME     DESTINATION bin
        ARCHIVE     DESTINATION lib
        LIBRARY     DESTINATION lib)
endif()
