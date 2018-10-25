
cmake_minimum_required(VERSION 3.0)
project(LuminoEngine)

include(../LuminoCore/LuminoCoreSources.cmake)

set(LN_ENGINE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/../../include)

#------------------------------------------------------------------------------
# Files

set(LN_SOURCES_Engine
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Engine/Common.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Engine/Application.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Engine/Diagnostics.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Engine/Engine.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Engine/EngineSettings.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Engine/Object.hpp
    src/Engine/Internal.hpp
    src/Engine/Application.cpp
    src/Engine/Diagnostics.cpp
    src/Engine/Engine.cpp
    src/Engine/EngineDomain.hpp
    src/Engine/EngineManager.cpp
    src/Engine/EngineManager.hpp
    src/Engine/LinearAllocator.hpp
    src/Engine/RenderingCommandList.cpp
    src/Engine/RenderingCommandList.hpp
    src/Engine/Object.cpp
)

set(LN_SOURCES_Platform
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Platform/CocoaPlatformInterface.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Platform/PlatformEvent.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Platform/PlatformSupport.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Platform/PlatformWindow.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Platform/Win32PlatformInterface.hpp
    src/Platform/Internal.hpp
    src/Platform/EmptyPlatformWindowManager.cpp
    src/Platform/EmptyPlatformWindowManager.hpp
    src/Platform/EmscriptenPlatformInterface.cpp
    src/Platform/GLFWPlatformWindowManager.cpp
    src/Platform/GLFWPlatformWindowManager.hpp
    src/Platform/PlatformEvent.cpp
    src/Platform/PlatformManager.cpp
    src/Platform/PlatformManager.hpp
    src/Platform/PlatformSupport.cpp
    src/Platform/PlatformWindow.cpp
    src/Platform/PlatformWindowManager.cpp
    src/Platform/PlatformWindowManager.hpp
    src/Platform/Win32PlatformInterface.cpp
)

set(LN_SOURCES_Grammar
    src/Grammar/Internal.hpp
    src/Grammar/AbstractLexer.cpp
    src/Grammar/AbstractLexer.hpp
    src/Grammar/CppLexer.cpp
    src/Grammar/CppLexer.hpp
    src/Grammar/Token.hpp
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

set(LN_SOURCES_Graphics
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Graphics/Common.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Graphics/Bitmap.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Graphics/ColorStructs.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Graphics/DepthBuffer.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Graphics/GeometryStructs.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Graphics/GraphicsContext.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Graphics/GraphicsResource.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Graphics/IndexBuffer.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Graphics/RenderState.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Graphics/SamplerState.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Graphics/SwapChain.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Graphics/Texture.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Graphics/VertexBuffer.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Graphics/VertexDeclaration.hpp
    src/Graphics/Internal.hpp
    src/Graphics/Bitmap.cpp
    src/Graphics/ColorStructs.cpp
    src/Graphics/DepthBuffer.cpp
    src/Graphics/GeometryStructs.cpp
    src/Graphics/GLFWContext.cpp
    src/Graphics/GLFWContext.hpp
    src/Graphics/GraphicsContext.cpp
    src/Graphics/GraphicsDeviceContext.cpp
    src/Graphics/GraphicsDeviceContext.hpp
    src/Graphics/GraphicsManager.cpp
    src/Graphics/GraphicsManager.hpp
    src/Graphics/GraphicsResource.cpp
    src/Graphics/IndexBuffer.cpp
    src/Graphics/RenderState.cpp
    src/Graphics/SamplerState.cpp
    src/Graphics/SwapChain.cpp
    src/Graphics/OpenGLDeviceContext.cpp
    src/Graphics/OpenGLDeviceContext.hpp
    src/Graphics/Texture.cpp
    src/Graphics/VertexBuffer.cpp
    src/Graphics/VertexDeclaration.cpp
)

set(LN_SOURCES_Shader
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Shader/Common.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Shader/Shader.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Shader/ShaderHelper.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Shader/ShaderInterfaceFramework.hpp
    src/Shader/Internal.hpp
    src/Shader/Shader.cpp
    src/Shader/ShaderHelper.cpp
    src/Shader/ShaderAnalyzer.cpp
    src/Shader/ShaderAnalyzer.hpp
    src/Shader/ShaderInterfaceFramework.cpp
    src/Shader/ShaderManager.cpp
    src/Shader/ShaderManager.hpp
)

set(LN_SOURCES_Mesh
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Mesh/Common.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Mesh/Mesh.hpp
    src/Mesh/Internal.hpp
    src/Mesh/GMesh.cpp
    src/Mesh/GMesh.hpp
    src/Mesh/Mesh.cpp
    src/Mesh/MeshManager.cpp
    src/Mesh/MeshManager.hpp
    src/Mesh/MqoImporter.cpp
    src/Mesh/MqoImporter.hpp
)

set(LN_SOURCES_Rendering
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Rendering/Common.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Rendering/CommandBuffer.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Rendering/Material.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Rendering/RenderFeature.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Rendering/RenderingContext.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Rendering/RenderView.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Rendering/Vertex.hpp
    src/Rendering/Internal.hpp
    src/Rendering/ClusteredShadingSceneRenderer.cpp
    src/Rendering/ClusteredShadingSceneRenderer.hpp
    src/Rendering/CommandBuffer.cpp
    src/Rendering/DrawElementListBuilder.cpp
    src/Rendering/DrawElementListBuilder.hpp
    src/Rendering/LightClusters.cpp
    src/Rendering/LightClusters.hpp
    src/Rendering/Material.cpp
    src/Rendering/MeshRenderFeature.cpp
    src/Rendering/MeshRenderFeature.hpp
    src/Rendering/RenderFeature.cpp
    src/Rendering/RenderingContext.cpp
    src/Rendering/RenderingManager.cpp
    src/Rendering/RenderingManager.hpp
    src/Rendering/RenderStage.cpp
    src/Rendering/RenderStage.hpp
    src/Rendering/RenderTargetTextureCache.cpp
    src/Rendering/RenderTargetTextureCache.hpp
    src/Rendering/RenderView.cpp
    src/Rendering/SceneRenderer.cpp
    src/Rendering/SceneRenderer.hpp
    src/Rendering/SpriteRenderFeature.cpp
    src/Rendering/SpriteRenderFeature.hpp
    src/Rendering/UnLigitingSceneRenderer.cpp
    src/Rendering/UnLigitingSceneRenderer.hpp
)

set(LN_SOURCES_Asset
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Asset/Common.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Asset/Asset.hpp
    src/Asset/Internal.hpp
    src/Asset/Asset.cpp
    src/Asset/AssetManager.cpp
    src/Asset/AssetManager.hpp
)

set(LN_SOURCES_Visual
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Visual/Common.hpp
    src/Visual/Internal.hpp
    src/Visual/VisualManager.cpp
    src/Visual/VisualManager.hpp
)

set(LN_SOURCES_Scene
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Scene/Common.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Scene/Scene.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Scene/SceneConductor.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Scene/World.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Scene/WorldObject.hpp
    src/Scene/Internal.hpp
    src/Scene/Scene.cpp
    src/Scene/SceneConductor.cpp
    src/Scene/SceneManager.cpp
    src/Scene/SceneManager.hpp
    src/Scene/World.cpp
    src/Scene/WorldObject.cpp
)

set(LN_SOURCES_UI
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/UI/Common.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/UI/UIEvents.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/UI/UIFrameWindow.hpp
    src/UI/UIFrameWindow.cpp
    src/UI/UIManager.cpp
    src/UI/UIManager.hpp
)

if (APPLE)
    set(LN_SOURCES_Platform
        ${LN_SOURCES_Platform}
        src/Platform/iOSPlatformInterface.mm
        src/Platform/CocoaPlatformInterface.mm
        src/Platform/PlatformSupport.mm
    )
endif()

# Grouping.
source_group("Engine"                FILES ${LN_SOURCES_Engine})
source_group("Platform"                FILES ${LN_SOURCES_Platform})
source_group("Input"                FILES ${LN_SOURCES_Input})
source_group("Grammar"                FILES ${LN_SOURCES_Grammar})
source_group("Graphics"                FILES ${LN_SOURCES_Graphics})
source_group("Shader"                FILES ${LN_SOURCES_Shader})
source_group("Mesh"                FILES ${LN_SOURCES_Mesh})
source_group("Rendering"                FILES ${LN_SOURCES_Rendering})
source_group("Asset"                FILES ${LN_SOURCES_Asset})
source_group("Visual"                FILES ${LN_SOURCES_Visual})
source_group("Scene"                FILES ${LN_SOURCES_Scene})
source_group("UI"                    FILES ${LN_SOURCES_UI})

set(LN_SOURCES
    src/LuminoEngine.PCH.cpp
    src/LuminoEngine.PCH.h
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine.hpp
    ${LN_SOURCES_Engine}
    ${LN_SOURCES_Platform}
    ${LN_SOURCES_Grammar}
    ${LN_SOURCES_Input}
    ${LN_SOURCES_Graphics}
    ${LN_SOURCES_Shader}
    ${LN_SOURCES_Mesh}
    ${LN_SOURCES_Rendering}
    ${LN_SOURCES_Asset}
    ${LN_SOURCES_Visual}
    ${LN_SOURCES_Scene}
    ${LN_SOURCES_UI}
    #${LN_SOURCES_LuminoCore}
)

set(LN_SOURCES_SharedLib
    src/SharedLib/DllMain.cpp
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

elseif (APPLE)
    list(APPEND LN_PLATFORM_LIBS glfw GLAD)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(CoreVideo_LIBRARY CoreVideo)
    find_library(IOKit_LIBRARY IOKit)
    list(APPEND LN_PLATFORM_LIBS ${COCOA_LIBRARY})
    list(APPEND LN_PLATFORM_LIBS ${CoreVideo_LIBRARY})
    list(APPEND LN_PLATFORM_LIBS ${IOKit_LIBRARY})
    
elseif (UNIX)
    if (LN_ANDROID)
    else()
        list(APPEND LN_PLATFORM_LIBS glfw GLAD)
    endif()
endif()


#------------------------------------------------------------------------------
# Targets

#add_library(${PROJECT_NAME}-static STATIC ${LN_SOURCES})
#target_include_directories(${PROJECT_NAME}-static PUBLIC "${LUMINO_INCLUDE_DIRECTORIES}" "src" ${LN_DEPENDENCIES_INCLUDE_DIRECTORIES})
#target_link_libraries(${PROJECT_NAME}-static LuminoCore ${LN_PLATFORM_LIBS} ${OPENGL_LIBRARY})
#set_target_properties(${PROJECT_NAME}-static PROPERTIES FOLDER "Lumino")
#ln_add_pch(${PROJECT_NAME}-static "src/LuminoEngine.PCH.h" "src/LuminoEngine.PCH.cpp")

#add_library(${PROJECT_NAME} STATIC ${LN_SOURCES})



add_library(${PROJECT_NAME} STATIC ${LN_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC  
    $<BUILD_INTERFACE:${LN_ENGINE_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:../../include>  # <prefix>/include
)
target_include_directories(${PROJECT_NAME} PRIVATE ${LN_ENGINE_INCLUDE_DIR} "${LUMINO_INCLUDE_DIRECTORIES}" "src" ${LN_DEPENDENCIES_INCLUDE_DIRECTORIES} ${glslang_INCLUDE_DIRS} ${spirv-cross_INCLUDE_DIRS})
#target_compile_definitions(${PROJECT_NAME} PUBLIC LUMINO_BUILD_DLL=1)
#target_link_libraries(${PROJECT_NAME} -Wl,--whole-archive ${PROJECT_NAME}-static -Wl,--no-whole-archive)
#target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-static)
target_link_libraries(${PROJECT_NAME} INTERFACE LuminoCore ${OPENGL_LIBRARY} ${glslang_LIBRARIES} ${spirv-cross_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE LuminoCore ZLIB PNG ${LN_PLATFORM_LIBS})
#target_link_libraries(${PROJECT_NAME} PRIVATE glslang)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
ln_add_pch(${PROJECT_NAME} "src/LuminoEngine.PCH.h" "src/LuminoEngine.PCH.cpp")

if (LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER)
    target_compile_definitions(${PROJECT_NAME} PRIVATE LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER=1)
endif()

if (LN_BUILD_XCODE_FRAMEWORK)
    # Build as framework.
    set_target_properties(${PROJECT_NAME} PROPERTIES
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

#------------------------------------------------------------------------------
# Install

install(TARGETS ${PROJECT_NAME}
    EXPORT LuminoEngineTargets
    ARCHIVE     DESTINATION .#lib            # Library installation location (Copied at "INSTALL" build time)
    #INCLUDES    DESTINATION ../../include        # include directory (File is not copied)
    FRAMEWORK DESTINATION .)
    
#install(DIRECTORY ${PROJECT_SOURCE_DIR}/include DESTINATION .)

install(
    EXPORT LuminoEngineTargets
    FILE LuminoEngineTargets.cmake
    EXPORT_LINK_INTERFACE_LIBRARIES
    DESTINATION "cmake")

install(
    FILES "LuminoEngineConfig.cmake"
    DESTINATION "cmake")
