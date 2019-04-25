
cmake_minimum_required(VERSION 3.0)
project(LuminoEngine)

include(../LuminoCore/LuminoCoreSources.cmake)

set(LN_ENGINE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/../../include)

#------------------------------------------------------------------------------
# Files

set(LN_SOURCES_Base
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Base/Common.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Base/Regex.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Base/Task.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Base/Variant.hpp
    src/Base/Internal.hpp
    src/Base/RefObjectCache.hpp
    src/Base/Regex.cpp
    src/Base/Task.cpp
    src/Base/Variant.cpp
)

set(LN_SOURCES_Engine
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Engine/Common.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Engine/Application.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Engine/Diagnostics.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Engine/Engine.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Engine/EngineSettings.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Engine/Object.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Engine/Property.hpp
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
    src/Engine/RenderingCommandList.hpp
    src/Engine/Object.cpp
    src/Engine/Property.cpp
)

set(LN_SOURCES_Platform
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Platform/AndroidPlatformInterface.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Platform/CocoaPlatformInterface.hpp
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
    src/Platform/PlatformEvent.cpp
    src/Platform/PlatformManager.cpp
    src/Platform/PlatformManager.hpp
    src/Platform/PlatformSupport.cpp
    src/Platform/PlatformWindow.cpp
    src/Platform/PlatformWindowManager.cpp
    src/Platform/PlatformWindowManager.hpp
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
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Graphics/VertexLayout.hpp
    src/Graphics/Internal.hpp
    src/Graphics/Bitmap.cpp
    src/Graphics/BitmapEncoding.cpp
    src/Graphics/BitmapEncoding.hpp
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
    src/Graphics/MixHash.hpp
    src/Graphics/OpenGLDeviceContext.cpp
    src/Graphics/OpenGLDeviceContext.hpp
    src/Graphics/RenderTargetTextureCache.cpp
    src/Graphics/RenderTargetTextureCache.hpp
    src/Graphics/Texture.cpp
    src/Graphics/VertexBuffer.cpp
    src/Graphics/VertexLayout.cpp
)

set(LN_SOURCES_Shader
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Shader/Common.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Shader/Shader.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Shader/ShaderHelper.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Shader/ShaderInterfaceFramework.hpp
    src/Shader/Internal.hpp
    src/Shader/HLSLMetadataParser.cpp
    src/Shader/HLSLMetadataParser.hpp
    src/Shader/Shader.cpp
    src/Shader/ShaderTranspiler.cpp
    src/Shader/ShaderTranspiler.hpp
    src/Shader/ShaderHelper.cpp
    src/Shader/ShaderInterfaceFramework.cpp
    src/Shader/ShaderManager.cpp
    src/Shader/ShaderManager.hpp
    src/Shader/UnifiedShader.cpp
    src/Shader/UnifiedShader.hpp
)

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

set(LN_SOURCES_Mesh
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Mesh/Common.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Mesh/Mesh.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Mesh/SkinnedMeshModel.hpp
    src/Mesh/Internal.hpp
    src/Mesh/CCDIKSolver.cpp
    src/Mesh/CCDIKSolver.hpp
    src/Mesh/GMesh.cpp
    src/Mesh/GMesh.hpp
    src/Mesh/Mesh.cpp
    src/Mesh/MeshGenerater.cpp
    src/Mesh/MeshGenerater.hpp
    src/Mesh/MeshImporter.cpp
    src/Mesh/MeshImporter.hpp
    src/Mesh/MeshManager.cpp
    src/Mesh/MeshManager.hpp
    src/Mesh/MqoImporter.cpp
    src/Mesh/MqoImporter.hpp
    src/Mesh/ObjMeshImporter.cpp
    src/Mesh/ObjMeshImporter.hpp
    src/Mesh/PmxImporter.cpp
    src/Mesh/PmxImporter.hpp
    src/Mesh/SkinnedMeshModel.cpp
)

set(LN_SOURCES_Rendering
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Rendering/Common.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Rendering/CommandBuffer.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Rendering/Drawing.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Rendering/Material.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Rendering/RenderFeature.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Rendering/RenderingContext.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Rendering/RenderView.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Rendering/Vertex.hpp
    src/Rendering/Internal.hpp
    src/Rendering/BlitRenderFeature.cpp
    src/Rendering/BlitRenderFeature.hpp
    src/Rendering/ClusteredShadingSceneRenderer.cpp
    src/Rendering/ClusteredShadingSceneRenderer.hpp
    src/Rendering/CommandBuffer.cpp
    src/Rendering/DrawElementListBuilder.cpp
    src/Rendering/DrawElementListBuilder.hpp
    src/Rendering/Drawing.cpp
    src/Rendering/FrameRectRenderFeature.cpp
    src/Rendering/FrameRectRenderFeature.hpp
    src/Rendering/LightClusters.cpp
    src/Rendering/LightClusters.hpp
    src/Rendering/Material.cpp
    src/Rendering/MeshRenderFeature.cpp
    src/Rendering/MeshRenderFeature.hpp
    src/Rendering/PrimitiveRenderFeature.cpp
    src/Rendering/PrimitiveRenderFeature.hpp
    src/Rendering/RenderFeature.cpp
    src/Rendering/RenderingContext.cpp
    src/Rendering/RenderingManager.cpp
    src/Rendering/RenderingManager.hpp
    src/Rendering/RenderingPipeline.cpp
    src/Rendering/RenderingPipeline.hpp
    src/Rendering/RenderStage.cpp
    src/Rendering/RenderStage.hpp
    src/Rendering/RenderView.cpp
    src/Rendering/ShapesRenderFeature.cpp
    src/Rendering/ShapesRenderFeature.hpp
    src/Rendering/SceneRenderer.cpp
    src/Rendering/SceneRenderer.hpp
    src/Rendering/SpriteRenderFeature.cpp
    src/Rendering/SpriteRenderFeature.hpp
    src/Rendering/SpriteTextRenderFeature.cpp
    src/Rendering/SpriteTextRenderFeature.hpp
    src/Rendering/UnLigitingSceneRenderer.cpp
    src/Rendering/UnLigitingSceneRenderer.hpp
)

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

set(LN_SOURCES_Asset
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Asset/Common.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Asset/Assets.hpp
    src/Asset/Internal.hpp
    src/Asset/Assets.cpp
    src/Asset/AssetManager.cpp
    src/Asset/AssetManager.hpp
    src/Asset/AssetArchive.cpp
    src/Asset/AssetArchive.hpp
)

set(LN_SOURCES_Visual
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Visual/Common.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Visual/CameraComponent.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Visual/LightComponent.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Visual/MeshPrimitiveComponent.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Visual/SkinnedMeshComponent.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Visual/SpriteComponent.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Visual/StaticMeshComponent.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Visual/VisualComponent.hpp
    src/Visual/Internal.hpp
    src/Visual/CameraComponent.cpp
    src/Visual/LightComponent.cpp
    src/Visual/MeshPrimitiveComponent.cpp
    src/Visual/SkinnedMeshComponent.cpp
    src/Visual/SpriteComponent.cpp
    src/Visual/StaticMeshComponent.cpp
    src/Visual/VisualComponent.cpp
    src/Visual/VisualManager.cpp
    src/Visual/VisualManager.hpp
)

set(LN_SOURCES_Scene
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Scene/Common.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Scene/Camera.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Scene/CameraOrbitControlComponent.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Scene/Component.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Scene/Light.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Scene/OffscreenWorldRenderView.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Scene/RigidBodyComponent.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Scene/Scene.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Scene/SceneConductor.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Scene/SkinnedMesh.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Scene/Sprite.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Scene/StaticMesh.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Scene/VisualObject.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Scene/World.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Scene/WorldObject.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Scene/WorldRenderView.hpp
    src/Scene/Internal.hpp
    src/Scene/Camera.cpp
    src/Scene/CameraOrbitControlComponent.cpp
    src/Scene/Component.cpp
    src/Scene/Light.cpp
    src/Scene/OffscreenWorldRenderView.cpp
    src/Scene/RigidBodyComponent.cpp
    src/Scene/Scene.cpp
    src/Scene/SceneConductor.cpp
    src/Scene/SceneManager.cpp
    src/Scene/SceneManager.hpp
    src/Scene/SkinnedMesh.cpp
    src/Scene/Sprite.cpp
    src/Scene/StaticMesh.cpp
    src/Scene/VisualObject.cpp
    src/Scene/World.cpp
    src/Scene/WorldObject.cpp
    src/Scene/WorldRenderView.cpp
)

set(LN_SOURCES_UI
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/UI/Common.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/UI/UIContainerElement.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/UI/UIContext.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/UI/UIControl.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/UI/UIElement.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/UI/UIEvents.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/UI/UIFrameWindow.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/UI/UILayoutElement.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/UI/UILayoutPanel.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/UI/UIRenderingContext.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/UI/UIRenderView.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/UI/UISprite.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/UI/UIStyle.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/UI/UITextBlock.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/UI/UIViewport.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/UI/UIWindow.hpp
    src/UI/UIContainerElement.cpp
    src/UI/UIContext.cpp
    src/UI/UIControl.cpp
    src/UI/UIElement.cpp
    src/UI/UIEventArgsPool.hpp
    src/UI/UIEvents.cpp
    src/UI/UIFrameWindow.cpp
    src/UI/UILayoutElement.cpp
    src/UI/UILayoutPanel.cpp
    src/UI/UIManager.cpp
    src/UI/UIManager.hpp
    src/UI/UIRenderingContext.cpp
    src/UI/UIRenderView.cpp
    src/UI/UISprite.cpp
    src/UI/UIStyle.cpp
    src/UI/UITextBlock.cpp
    src/UI/UIViewport.cpp
    src/UI/UIWindow.cpp
)

set(LN_SOURCES_Tilemap
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Tilemap/Common.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Tilemap/Tilemap.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Tilemap/TilemapComponent.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Tilemap/TilemapLayer.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Tilemap/TilemapModel.hpp
    ${LN_ENGINE_INCLUDE_DIR}/LuminoEngine/Tilemap/Tileset.hpp
    src/Tilemap/Internal.hpp
    src/Tilemap/Tilemap.cpp
    src/Tilemap/TilemapComponent.cpp
    src/Tilemap/TilemapLayer.cpp
    src/Tilemap/TilemapModel.cpp
    src/Tilemap/Tileset.cpp
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
source_group("Graphics"  FILES ${LN_SOURCES_Graphics})
source_group("Shader"    FILES ${LN_SOURCES_Shader})
source_group("Font"      FILES ${LN_SOURCES_Font})
source_group("Mesh"      FILES ${LN_SOURCES_Mesh})
source_group("Rendering" FILES ${LN_SOURCES_Rendering})
source_group("ImageEffect" FILES ${LN_SOURCES_ImageEffect})
source_group("Physics"	 FILES ${LN_SOURCES_Physics})
source_group("Asset"	 FILES ${LN_SOURCES_Asset})
source_group("Visual"    FILES ${LN_SOURCES_Visual})
source_group("Scene"     FILES ${LN_SOURCES_Scene})
source_group("UI"        FILES ${LN_SOURCES_UI})
source_group("Tilemap"   FILES ${LN_SOURCES_Tilemap})


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
    ${LN_SOURCES_Graphics}
    ${LN_SOURCES_Shader}
    ${LN_SOURCES_Font}
    ${LN_SOURCES_Mesh}
    ${LN_SOURCES_Rendering}
    ${LN_SOURCES_ImageEffect}
    ${LN_SOURCES_Physics}
    ${LN_SOURCES_Asset}
    ${LN_SOURCES_Visual}
    ${LN_SOURCES_Scene}
    ${LN_SOURCES_UI}
    ${LN_SOURCES_Tilemap}
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
target_compile_definitions(${PROJECT_NAME} PRIVATE ${LN_PLATFORM_DEFINITIONS})
#target_link_libraries(${PROJECT_NAME} -Wl,--whole-archive ${PROJECT_NAME}-static -Wl,--no-whole-archive)
#target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-static)
target_link_libraries(${PROJECT_NAME} INTERFACE LuminoCore ${OPENGL_LIBRARY} ${glslang_LIBRARIES} ${spirv-cross_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE LuminoCore pcre PNG ZLIB vorbisfile vorbis ogg Box2D BulletSoftBody BulletDynamics BulletCollision LinearMath tmxlite VulkanHeaders ${LN_PLATFORM_LIBS})
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
    ARCHIVE     DESTINATION lib            # Library installation location (Copied at "INSTALL" build time)
    #INCLUDES    DESTINATION ../../include        # include directory (File is not copied)
    FRAMEWORK DESTINATION .)
    
#install(DIRECTORY ${PROJECT_SOURCE_DIR}/include DESTINATION .)

install(
    EXPORT LuminoEngineTargets
    FILE LuminoEngineTargets.cmake
    EXPORT_LINK_INTERFACE_LIBRARIES
    DESTINATION lib/cmake)

install(
    FILES LuminoEngineConfig.cmake
    DESTINATION lib/cmake)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ../../../../lib/${LN_TARGET_ARCH})
