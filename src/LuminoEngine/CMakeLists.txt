
cmake_minimum_required(VERSION 3.0)
project(LuminoEngine)

include(../LuminoCore/LuminoCoreSources.cmake)

set(LN_ENGINE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/../../include)

#------------------------------------------------------------------------------
# Files

file(GLOB LN_SOURCES_Base ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Base/*.hpp src/Base/*.cpp src/Base/*.hpp)

set(LN_SOURCES_Engine
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Engine/Common.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Engine/Application.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Engine/Diagnostics.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Engine/Engine.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Engine/EngineSettings.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Engine/Object.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Engine/Property.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Engine/RenderingCommandList.hpp
    src/Engine/Internal.hpp
    src/Engine/Application.cpp
    src/Engine/Diagnostics.cpp
    src/Engine/Engine.cpp
    src/Engine/EngineDomain.hpp
    src/Engine/EngineManager.cpp
    src/Engine/EngineManager.hpp
    src/Engine/FpsController.cpp
    src/Engine/FpsController.hpp
    src/Engine/LinearAllocator.hpp
    src/Engine/RenderingCommandList.cpp
    src/Engine/Object.cpp
    src/Engine/Property.cpp
)

set(LN_SOURCES_Platform
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Platform/AndroidPlatformInterface.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Platform/CocoaPlatformInterface.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Platform/PlatformDialogs.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Platform/PlatformEvent.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Platform/PlatformSupport.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Platform/PlatformWindow.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Platform/Win32PlatformInterface.hpp
    src/Platform/Internal.hpp
    src/Platform/AndroidPlatformInterface.cpp
    src/Platform/EmptyPlatformWindowManager.cpp
    src/Platform/EmptyPlatformWindowManager.hpp
    src/Platform/EmscriptenPlatformInterface.cpp
    src/Platform/GLFWPlatformWindowManager.cpp
    src/Platform/GLFWPlatformWindowManager.hpp
    src/Platform/PlatformDialogs.cpp
    src/Platform/PlatformEvent.cpp
    src/Platform/PlatformManager.cpp
    src/Platform/PlatformManager.hpp
    src/Platform/PlatformSupport.cpp
    src/Platform/PlatformWindow.cpp
    src/Platform/PlatformWindowManager.cpp
    src/Platform/PlatformWindowManager.hpp
    src/Platform/TextInputMethodSystem.cpp
    src/Platform/TextInputMethodSystem.hpp
	src/Platform/Win32PlatformDialogs.hpp
    src/Platform/Win32PlatformInterface.cpp
    src/Platform/Win32PlatformWindowManager.cpp
    src/Platform/Win32PlatformWindowManager.hpp
)

set(LN_SOURCES_Grammar
    src/Grammar/Internal.hpp
    src/Grammar/AbstractLexer.cpp
    src/Grammar/AbstractLexer.hpp
    src/Grammar/CppLexer.cpp
    src/Grammar/CppLexer.hpp
    src/Grammar/Token.hpp
)

set(LN_SOURCES_Animation
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Animation/Common.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Animation/Animation.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Animation/AnimationClip.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Animation/AnimationClock.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Animation/AnimationContext.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Animation/AnimationController.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Animation/AnimationCurve.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Animation/AnimationTrack.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Animation/Animator.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Animation/EasingFunctions.hpp
    src/Animation/Internal.hpp
    src/Animation/Animation.cpp
    src/Animation/AnimationClip.cpp
    src/Animation/AnimationClock.cpp
    src/Animation/AnimationController.cpp
    src/Animation/AnimationCurve.cpp
    src/Animation/AnimationContext.cpp
    src/Animation/AnimationManager.cpp
    src/Animation/AnimationManager.hpp
    src/Animation/AnimationTrack.cpp
    src/Animation/Animator.cpp
    src/Animation/VmdLoader.cpp
    src/Animation/VmdLoader.hpp
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

set(LN_SOURCES_Audio
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Audio/Common.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Audio/AudioBus.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Audio/AudioContext.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Audio/AudioGainNode.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Audio/AudioListener.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Audio/AudioNode.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Audio/AudioProcessorNode.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Audio/GameAudio.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Audio/InternalSharedMutex.inc
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Audio/Sound.hpp
    src/Audio/Internal.hpp
    src/Audio/AudioBus.cpp
    src/Audio/AudioContext.cpp
    src/Audio/AudioDevice.cpp
    src/Audio/AudioDevice.hpp
    src/Audio/AudioGainNode.cpp
    src/Audio/AudioListener.cpp
    src/Audio/AudioManager.cpp
    src/Audio/AudioManager.hpp
    src/Audio/AudioNode.cpp
    src/Audio/AudioProcessorNode.cpp
    src/Audio/CAGainNode.cpp
    src/Audio/CAGainNode.hpp
    src/Audio/CAProcessorNode.cpp
    src/Audio/CAProcessorNode.hpp
    src/Audio/ChromiumWebCore.cpp
    src/Audio/ChromiumWebCore.hpp
    src/Audio/CoreAudioNode.cpp
    src/Audio/CoreAudioNode.hpp
    src/Audio/GameAudio.cpp
    src/Audio/GameAudioImpl.cpp
    src/Audio/GameAudioImpl.hpp
    src/Audio/Sound.cpp
    src/Audio/blink/VectorMath.cpp
    src/Audio/blink/VectorMath.h
)

set(LN_SOURCES_Audio_Decoder
    src/Audio/AudioDecoder.cpp
    src/Audio/AudioDecoder.hpp
    src/Audio/Mp3AudioDecoder.cpp
    src/Audio/Mp3AudioDecoder.hpp
    src/Audio/OggAudioDecoder.cpp
    src/Audio/OggAudioDecoder.hpp
)

set(LN_SOURCES_Audio_Device
    src/Audio/ALAudioDevice.cpp
    src/Audio/ALAudioDevice.hpp
    src/Audio/DSoundAudioDevice.cpp
    src/Audio/DSoundAudioDevice.hpp
    src/Audio/SDLAudioDevice.cpp
    src/Audio/SDLAudioDevice.hpp
)

file(GLOB LN_SOURCES_Shader ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Shader/*.hpp src/Shader/*.cpp src/Shader/*.hpp)

file(GLOB LN_SOURCES_Graphics ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Graphics/*.hpp src/Graphics/*.cpp src/Graphics/*.hpp)

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

file(GLOB LN_SOURCES_Rendering ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Rendering/*.hpp src/Rendering/*.cpp src/Rendering/*.hpp)

set(LN_SOURCES_ImageEffect
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/ImageEffect/ImageEffect.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/ImageEffect/ScreenBlurImageEffect.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/ImageEffect/ToneImageEffect.hpp
    src/ImageEffect/ImageEffect.cpp
    src/ImageEffect/ImageEffectRenderer.cpp
    src/ImageEffect/ImageEffectRenderer.hpp
    src/ImageEffect/ScreenBlurImageEffect.cpp
    src/ImageEffect/ToneImageEffect.cpp
)

file(GLOB LN_SOURCES_Effect ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Effect/*.hpp src/Effect/*.cpp src/Effect/*.hpp)

set(LN_SOURCES_Physics
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Physics/Common.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Physics/CollisionShape.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Physics/PhysicsWorld.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Physics/PhysicsWorld2D.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Physics/PhysicsObject.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Physics/RigidBody.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Physics/SoftBody.hpp
    src/Physics/Internal.hpp
    src/Physics/BulletUtils.hpp
    src/Physics/CollisionShape.cpp
    src/Physics/PhysicsManager.cpp
    src/Physics/PhysicsManager.hpp
    src/Physics/PhysicsObject.cpp
    src/Physics/PhysicsWorld.cpp
    src/Physics/PhysicsWorld2D.cpp
    src/Physics/RigidBody.cpp
    src/Physics/SoftBody.cpp
)

file(GLOB LN_SOURCES_Asset ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Asset/*.hpp src/Asset/*.cpp src/Asset/*.hpp)

set(LN_SOURCES_Visual
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Visual/Common.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Visual/CameraComponent.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Visual/LightComponent.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Visual/MeshPrimitiveComponent.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Visual/ParticleEmitterComponent.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Visual/SkinnedMeshComponent.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Visual/SpriteComponent.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Visual/StaticMeshComponent.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Visual/VisualComponent.hpp
    src/Visual/Internal.hpp
    src/Visual/CameraComponent.cpp
    src/Visual/LightComponent.cpp
    src/Visual/MeshPrimitiveComponent.cpp
    src/Visual/ParticleEmitterComponent.cpp
    src/Visual/SkinnedMeshComponent.cpp
    src/Visual/SpriteComponent.cpp
    src/Visual/StaticMeshComponent.cpp
    src/Visual/VisualComponent.cpp
    src/Visual/VisualManager.cpp
    src/Visual/VisualManager.hpp
)

file(GLOB LN_SOURCES_Scene ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Scene/*.hpp src/Scene/*.cpp src/Scene/*.hpp)

file(GLOB LN_SOURCES_UI ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/UI/*.hpp src/UI/*.cpp src/UI/*.hpp)

set(LN_SOURCES_UI
    ${LN_SOURCES_UI}
    ${LUMINO_ROOT_PATH}/build/ExternalSource/imgui/imconfig.h
    ${LUMINO_ROOT_PATH}/build/ExternalSource/imgui/imgui.cpp
    ${LUMINO_ROOT_PATH}/build/ExternalSource/imgui/imgui.h
    ${LUMINO_ROOT_PATH}/build/ExternalSource/imgui/imgui_draw.cpp
    ${LUMINO_ROOT_PATH}/build/ExternalSource/imgui/imgui_internal.h
    ${LUMINO_ROOT_PATH}/build/ExternalSource/imgui/imgui_widgets.cpp
    ${LUMINO_ROOT_PATH}/build/ExternalSource/imgui/imstb_rectpack.h
    ${LUMINO_ROOT_PATH}/build/ExternalSource/imgui/imstb_textedit.h
    ${LUMINO_ROOT_PATH}/build/ExternalSource/imgui/imstb_truetype.h
)
set(LN_IMGUI_INCLUDE_DIRECTORY ${LUMINO_ROOT_PATH}/build/ExternalSource/imgui)
file(GLOB LN_SOURCES_Tilemap ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Tilemap/*.hpp src/Tilemap/*.cpp src/Tilemap/*.hpp)

file(GLOB LN_SOURCES_Runtime
	${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Runtime/*.h
	${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Runtime/*.hpp
	src/Runtime/*.cpp
    src/Runtime/*.hpp
    src/Runtime/*.h
)

if (LN_USE_VULKAN)
    if (WIN32)
        set(LN_SOURCES_Graphics
            ${LN_SOURCES_Graphics}
            src/Graphics/VulkanDeviceContext.cpp
            src/Graphics/VulkanDeviceContext.hpp
            src/Graphics/VulkanHelper.cpp
            src/Graphics/VulkanHelper.hpp
        )
        list(APPEND LN_PLATFORM_DEFINITIONS LN_USE_VULKAN=1)
    endif()
endif()


if (WIN32)

elseif (APPLE)
    set(LN_SOURCES_Platform
        ${LN_SOURCES_Platform}
        src/Platform/iOSPlatformInterface.mm
        src/Platform/CocoaPlatformInterface.mm
        src/Platform/PlatformSupport.mm
    )
endif()

# Grouping.
source_group("Base"      FILES ${LN_SOURCES_Base})
source_group("Engine"    FILES ${LN_SOURCES_Engine})
source_group("Platform"  FILES ${LN_SOURCES_Platform})
source_group("Input"     FILES ${LN_SOURCES_Input})
source_group("Grammar"   FILES ${LN_SOURCES_Grammar})
source_group("Animation" FILES ${LN_SOURCES_Animation})
source_group("Audio"     FILES ${LN_SOURCES_Audio})
source_group("Audio\\Decoder" FILES ${LN_SOURCES_Audio_Decoder})
source_group("Audio\\Device" FILES ${LN_SOURCES_Audio_Device})
source_group("Shader"    FILES ${LN_SOURCES_Shader})
source_group("Graphics"  FILES ${LN_SOURCES_Graphics})
source_group("Font"      FILES ${LN_SOURCES_Font})
source_group("Mesh"      FILES ${LN_SOURCES_Mesh})
source_group("Rendering" FILES ${LN_SOURCES_Rendering})
source_group("ImageEffect" FILES ${LN_SOURCES_ImageEffect})
source_group("Effect"    FILES ${LN_SOURCES_Effect})
source_group("Physics"	 FILES ${LN_SOURCES_Physics})
source_group("Asset"	 FILES ${LN_SOURCES_Asset})
source_group("Visual"    FILES ${LN_SOURCES_Visual})
source_group("Scene"     FILES ${LN_SOURCES_Scene})
source_group("UI"        FILES ${LN_SOURCES_UI})
source_group("Tilemap"   FILES ${LN_SOURCES_Tilemap})
source_group("Runtime"   FILES ${LN_SOURCES_Runtime})

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
	${LN_SOURCES_Audio_Device}
    ${LN_SOURCES_Shader}
    ${LN_SOURCES_Graphics}
    ${LN_SOURCES_Font}
    ${LN_SOURCES_Mesh}
    ${LN_SOURCES_Rendering}
    ${LN_SOURCES_ImageEffect}
    ${LN_SOURCES_Effect}
    ${LN_SOURCES_Physics}
    ${LN_SOURCES_Asset}
    ${LN_SOURCES_Visual}
    ${LN_SOURCES_Scene}
    ${LN_SOURCES_UI}
    ${LN_SOURCES_Tilemap}
    ${LN_SOURCES_Runtime}
)

set(LN_SOURCES_SharedLib
    src/SharedLibraryMain.cpp
)



if (LN_EMSCRIPTEN)

elseif (LN_IOS)
    FIND_LIBRARY( OPENGLES_FRAMEWORKS OpenGLES )
    if(OPENGLES_FRAMEWORKS)
        list(APPEND LN_PLATFORM_LIBS "-framework OpenGLES")
    endif()

elseif (WIN32)
    find_package(OpenGL)
    list(APPEND LN_PLATFORM_LIBS glfw GLAD)
    #list(APPEND LN_PLATFORM_LIBS user32 gdi32 winmm imm32 ole32 oleaut32 version uuid)
    list(APPEND LN_PLATFORM_LIBS winmm imm32 version)
    list(APPEND LN_PLATFORM_LIBS OpenAL)
    
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
    VulkanHeaders
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
target_link_libraries(LuminoEngine-static INTERFACE LuminoCore ${OPENGL_LIBRARY} ${glslang_LIBRARIES} ${spirv-cross_LIBRARIES})
target_link_libraries(LuminoEngine-static PRIVATE ${_LN_ENGINE_LINK_LIBS})

#target_link_libraries(LuminoEngine-static PRIVATE glslang)
set_target_properties(LuminoEngine-static PROPERTIES FOLDER "Lumino")
ln_add_pch(LuminoEngine-static "src/LuminoEngine.PCH.h" "src/LuminoEngine.PCH.cpp")


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

# Unit test.
if (LN_BUILD_TESTS)
    add_subdirectory(test)
    add_subdirectory(sandbox)
endif()

#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /INCREMENTAL:NO" )

# Shared library
#add_library(LuminoEngine SHARED ${LN_SOURCES_SharedLib} ${LN_SOURCES})
add_library(LuminoEngine SHARED ${LN_SOURCES_SharedLib})
target_include_directories(LuminoEngine PRIVATE ${LN_ENGINE_INCLUDE_DIR} "${LUMINO_INCLUDE_DIRECTORIES}" "src" ${LN_DEPENDENCIES_INCLUDE_DIRECTORIES} ${glslang_INCLUDE_DIRS} ${spirv-cross_INCLUDE_DIRS} ${LN_IMGUI_INCLUDE_DIRECTORY})
#target_compile_definitions(LuminoEngine PRIVATE LUMINO_BUILD_DLL=1 ${LN_PLATFORM_DEFINITIONS})
target_compile_definitions(LuminoEngine PRIVATE LUMINO_BUILD_DLL=1)
target_link_libraries(LuminoEngine PRIVATE LuminoEngine-static)
#target_link_libraries(LuminoEngine PRIVATE ${_LN_ENGINE_LINK_LIBS} LuminoCore ${OPENGL_LIBRARY} ${glslang_LIBRARIES} ${spirv-cross_LIBRARIES})
set_target_properties(LuminoEngine PROPERTIES FOLDER "Lumino")
#ln_add_pch(LuminoEngine "src/LuminoEngine.PCH.h" "src/LuminoEngine.PCH.cpp")

# Export symbols defined in another static library.
if (MSVC)
    set_target_properties(LuminoEngine PROPERTIES LINK_FLAGS_DEBUG "/WHOLEARCHIVE:LuminoEngine-staticd")
    set_target_properties(LuminoEngine PROPERTIES LINK_FLAGS_RELEASE "/WHOLEARCHIVE:LuminoEngine-static")
else()
    set_target_properties(LuminoEngine PROPERTIES LINK_FLAGS_DEBUG "--no-whole-archive")
    set_target_properties(LuminoEngine PROPERTIES LINK_FLAGS_RELEASE "--no-whole-archive")
endif()

if (LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER)
    target_compile_definitions(LuminoEngine-static PRIVATE LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER=1)
    target_compile_definitions(LuminoEngine PRIVATE LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER=1)
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

install(TARGETS LuminoEngine
    EXPORT ${PROJECT_NAME}Targets
    RUNTIME     DESTINATION bin
    ARCHIVE     DESTINATION lib
    LIBRARY     DESTINATION lib)
