cmake_minimum_required (VERSION 2.8.7)
project (LuminoEngine)

include(Source/LuminoCore/CMake/LNCommon.cmake NO_POLICY_SCOPE)

set(LN_DEPENDENCIES_ROOT_DIR "${PROJECT_SOURCE_DIR}/external/LuminoDependencies/")
include(external/LuminoDependencies/LuminoDependencies.cmake)

set(LN_STATIC_LIB_OUTPUT_NAME "Lumino")
set(LN_LINK_LIBS "")
set(LN_UNIT_TEST_LINK_LIBS "")
set(LN_LIB_POSTFIX "")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "C:/Proj/Lumino/lib/MSVC140")

#------------------------------------------------------------------------------
# Options
#------------------------------------------------------------------------------

# Unicode char set.
option(LN_USE_UNICODE_CHAR_SET "Use unicode char set." ON)

# MSVC Runtime library.
option(LN_MSVC_LINK_MULTI_THREAD_STATIC_RUNTIME "static runtime (/MT, /MTd) or DLL runtime (/MD, /MDd) or to link either." ON)

# Exceprion Backtrace.
option(LN_EXCEPTION_BACKTRACE "In some unix environment there is a possibility that can not be compiled." ON)

option(LN_BUILD_AUDIO_MODULE "Build 'Audio' module." ON)
option(LN_BUILD_SCENE_MODULE "Build 'Scene' module." ON)
option(LN_BUILD_C_API "Build 'C API' module." OFF)
option(LN_USE_EXTERNAL_Effekseer "" ON)

# Architecture.
#option(LN_BUILD_X64 "Architecture." OFF)

option(LN_DxLib_Demo "DX Library demo." OFF)


#------------------------------------------------------------------------------
# Files
#------------------------------------------------------------------------------
#------------------------------
# Source files.

set(LN_SOURCES
	Source/LuminoEngine/Include/LuminoEngine.h
	Source/LuminoEngine/Include/LuminoAudio.h
	Source/LuminoEngine/Include/LuminoDxLib.h
	Source/LuminoEngine/Include/LuminoGraphics.h
	Source/LuminoEngine/Include/LuminoScene.h
	Source/LuminoEngine/Source/LuminoEngine.PCH.cpp
	Source/LuminoEngine/Source/LuminoEngine.PCH.h
)

set(LN_SOURCES_Base
	Source/LuminoEngine/Include/Lumino/Base/Uuid.h
	Source/LuminoEngine/Source/Base/Uuid.cpp
)

set(LN_SOURCES_Animation
	Source/LuminoEngine/Include/Lumino/Animation/AnimatableObject.h
	Source/LuminoEngine/Include/Lumino/Animation/AnimationClock.h
	Source/LuminoEngine/Include/Lumino/Animation/AnimationCurve.h
	Source/LuminoEngine/Include/Lumino/Animation/AnimationUtilities.h
	Source/LuminoEngine/Include/Lumino/Animation/Common.h
	Source/LuminoEngine/Include/Lumino/Animation/EasingFunctions.h
	Source/LuminoEngine/Include/Lumino/Animation/EasingValue.h
	Source/LuminoEngine/Source/Animation/AnimatableObject.cpp
	Source/LuminoEngine/Source/Animation/AnimationClip.cpp
	Source/LuminoEngine/Source/Animation/AnimationClip.h
	Source/LuminoEngine/Source/Animation/AnimationClock.cpp
	Source/LuminoEngine/Source/Animation/AnimationCurve.cpp
	Source/LuminoEngine/Source/Animation/AnimationManager.cpp
	Source/LuminoEngine/Source/Animation/AnimationManager.h
	Source/LuminoEngine/Source/Animation/AnimationState.cpp
	Source/LuminoEngine/Source/Animation/AnimationState.h
	Source/LuminoEngine/Source/Animation/Animator.cpp
	Source/LuminoEngine/Source/Animation/Animator.h
	Source/LuminoEngine/Source/Animation/Detail.h
	Source/LuminoEngine/Source/Animation/Internal.h
	Source/LuminoEngine/Source/Animation/VmdLoader.cpp
	Source/LuminoEngine/Source/Animation/VmdLoader.h
)

set(LN_SOURCES_IO
	Source/LuminoEngine/Include/Lumino/IO/FileManager.h
	Source/LuminoEngine/Include/Lumino/IO/ASyncIOObject.h
	Source/LuminoEngine/Source/IO/ArchiveMaker.cpp
	Source/LuminoEngine/Source/IO/ArchiveMaker.h
	Source/LuminoEngine/Source/IO/ASyncIOObject.cpp
	Source/LuminoEngine/Source/IO/FileManager.cpp
	Source/LuminoEngine/Source/IO/Archive.cpp
	Source/LuminoEngine/Source/IO/Archive.h
	external/camellia/camellia.cpp
	external/camellia/camellia.h
)

set(LN_SOURCES_Platform
	Source/LuminoEngine/Include/Lumino/Platform/Clipboard.h
	Source/LuminoEngine/Include/Lumino/Platform/Common.h
	Source/LuminoEngine/Include/Lumino/Platform/EventListener.h
	Source/LuminoEngine/Include/Lumino/Platform/Ime.h
	Source/LuminoEngine/Include/Lumino/Platform/PlatformEventArgs.h
	Source/LuminoEngine/Include/Lumino/Platform/PlatformFileOpenDialog.h
	Source/LuminoEngine/Include/Lumino/Platform/PlatformSupport.h
	Source/LuminoEngine/Include/Lumino/Platform/PlatformWindow.h
	Source/LuminoEngine/Source/Platform/Internal.h
	Source/LuminoEngine/Source/Platform/MouseCursorVisibility.h
	Source/LuminoEngine/Source/Platform/PlatformFileOpenDialog.cpp
	Source/LuminoEngine/Source/Platform/PlatformManager.cpp
	Source/LuminoEngine/Source/Platform/PlatformManager.h
	Source/LuminoEngine/Source/Platform/PlatformWindowManagerBase.cpp
	Source/LuminoEngine/Source/Platform/PlatformWindowManagerBase.h
	Source/LuminoEngine/Source/Platform/PlatformWindow.cpp
)

set(LN_SOURCES_Input
	Source/LuminoEngine/Include/Lumino/Input/Common.h
	Source/LuminoEngine/Include/Lumino/Input/Input.h
	Source/LuminoEngine/Include/Lumino/Input/InputBinding.h
	Source/LuminoEngine/Include/Lumino/Input/InputController.h
	Source/LuminoEngine/Source/Input/Input.cpp
	Source/LuminoEngine/Source/Input/InputBinding.cpp
	Source/LuminoEngine/Source/Input/InputDriver.cpp
	Source/LuminoEngine/Source/Input/InputDriver.h
	Source/LuminoEngine/Source/Input/InputManager.cpp
	Source/LuminoEngine/Source/Input/InputManager.h
	Source/LuminoEngine/Source/Input/Internal.h
	Source/LuminoEngine/Source/Input/InputController.cpp
)

if (LN_BUILD_AUDIO_MODULE)
set(LN_SOURCES_Audio
	Source/LuminoEngine/Include/LuminoAudio.h
	Source/LuminoEngine/Include/Lumino/Audio/Common.h
	Source/LuminoEngine/Include/Lumino/Audio/Detail.h
	Source/LuminoEngine/Include/Lumino/Audio/GameAudio.h
	Source/LuminoEngine/Include/Lumino/Audio/Sound.h
	Source/LuminoEngine/Include/Lumino/Audio/SoundListener.h
	Source/LuminoEngine/Source/Audio/AudioDevice.cpp
	Source/LuminoEngine/Source/Audio/AudioDevice.h
	Source/LuminoEngine/Source/Audio/AudioManager.cpp
	Source/LuminoEngine/Source/Audio/AudioManager.h
	Source/LuminoEngine/Source/Audio/AudioPlayer.cpp
	Source/LuminoEngine/Source/Audio/AudioPlayer.h
	Source/LuminoEngine/Source/Audio/AudioStream.cpp
	Source/LuminoEngine/Source/Audio/AudioStream.h
	Source/LuminoEngine/Source/Audio/AudioUtils.cpp
	Source/LuminoEngine/Source/Audio/AudioUtils.h
	Source/LuminoEngine/Source/Audio/GameAudio.cpp
	Source/LuminoEngine/Source/Audio/GameAudioImpl.cpp
	Source/LuminoEngine/Source/Audio/GameAudioImpl.h
	Source/LuminoEngine/Source/Audio/Internal.h
	Source/LuminoEngine/Source/Audio/MidiDecoder.cpp
	Source/LuminoEngine/Source/Audio/MidiDecoder.h
	Source/LuminoEngine/Source/Audio/NullAudioDevice.cpp
	Source/LuminoEngine/Source/Audio/NullAudioDevice.h
	Source/LuminoEngine/Source/Audio/OggDecoder.cpp
	Source/LuminoEngine/Source/Audio/OggDecoder.h
	Source/LuminoEngine/Source/Audio/Sound.cpp
	Source/LuminoEngine/Source/Audio/SoundListener.cpp
	Source/LuminoEngine/Source/Audio/WaveDecoder.cpp
	Source/LuminoEngine/Source/Audio/WaveDecoder.h
)
endif()

set(LN_SOURCES_Physics
	Source/LuminoEngine/Include/Lumino/Physics/BodyBase.h
	Source/LuminoEngine/Include/Lumino/Physics/Collider.h
	Source/LuminoEngine/Include/Lumino/Physics/Common.h
	Source/LuminoEngine/Include/Lumino/Physics/Joint.h
	Source/LuminoEngine/Include/Lumino/Physics/RigidBody.h
	Source/LuminoEngine/Source/Physics/BodyBase.cpp
	Source/LuminoEngine/Source/Physics/BulletUtils.h
	Source/LuminoEngine/Source/Physics/Collider.cpp
	Source/LuminoEngine/Source/Physics/Internal.h
	Source/LuminoEngine/Source/Physics/Joint.cpp
	Source/LuminoEngine/Source/Physics/PhysicsManager.cpp
	Source/LuminoEngine/Source/Physics/PhysicsManager.h
	Source/LuminoEngine/Source/Physics/RigidBody.cpp
)

set(LN_SOURCES_Graphics
	Source/LuminoEngine/Include/Lumino/Base/GeometryStructs.h
	Source/LuminoEngine/Include/Lumino/Graphics/Bitmap.h
	Source/LuminoEngine/Include/Lumino/Graphics/BitmapPainter.h
	Source/LuminoEngine/Include/Lumino/Graphics/Brush.h
	Source/LuminoEngine/Include/Lumino/Graphics/Color.h
	Source/LuminoEngine/Include/Lumino/Graphics/Common.h
	Source/LuminoEngine/Include/Lumino/Graphics/ContextInterface.h
	Source/LuminoEngine/Include/Lumino/Graphics/FrameCapturer.h
	Source/LuminoEngine/Include/Lumino/Graphics/Graphics.h
	Source/LuminoEngine/Include/Lumino/Graphics/GraphicsException.h
	Source/LuminoEngine/Include/Lumino/Graphics/GraphicsResourceObject.h
	Source/LuminoEngine/Include/Lumino/Graphics/IndexBuffer.h
	Source/LuminoEngine/Include/Lumino/Graphics/Material.h
	Source/LuminoEngine/Include/Lumino/Graphics/PerlinNoise.h
	Source/LuminoEngine/Include/Lumino/Graphics/Renderer.h
	Source/LuminoEngine/Include/Lumino/Graphics/Rendering.h
	Source/LuminoEngine/Include/Lumino/Graphics/RenderingContext.h
	Source/LuminoEngine/Include/Lumino/Graphics/RenderState.h
	Source/LuminoEngine/Include/Lumino/Graphics/SamplerState.h
	Source/LuminoEngine/Include/Lumino/Graphics/Shader.h
	Source/LuminoEngine/Include/Lumino/Graphics/SwapChain.h
	Source/LuminoEngine/Include/Lumino/Graphics/Texture.h
	Source/LuminoEngine/Include/Lumino/Graphics/Utils.h
	Source/LuminoEngine/Include/Lumino/Graphics/Vertex.h
	Source/LuminoEngine/Include/Lumino/Graphics/VertexBuffer.h
	Source/LuminoEngine/Include/Lumino/Graphics/VertexDeclaration.h
	Source/LuminoEngine/Include/Lumino/Graphics/Viewport.h
	Source/LuminoEngine/Include/Lumino/Graphics/ImageEffect/ImageEffect.h
	Source/LuminoEngine/Include/Lumino/Graphics/ImageEffect/BlurImageEffect.h
	Source/LuminoEngine/Include/Lumino/Graphics/ImageEffect/ScreenMotionBlurImageEffect.h
	Source/LuminoEngine/Include/Lumino/Graphics/ImageEffect/ToneImageEffect.h
	external/GL/glew.c
	external/nanovg/nanovg.c
	external/nanovg/nanovg.h
	Source/LuminoEngine/Source/Base/GeometryStructs.cpp
	Source/LuminoEngine/Source/Graphics/Bitmap.cpp
	Source/LuminoEngine/Source/Graphics/BitmapPainter.cpp
	Source/LuminoEngine/Source/Graphics/Brush.cpp
	Source/LuminoEngine/Source/Graphics/Color.cpp
	Source/LuminoEngine/Source/Graphics/ContextInterface.cpp
	Source/LuminoEngine/Source/Graphics/FrameCapturer.cpp
	Source/LuminoEngine/Source/Graphics/FrameRectRenderer.cpp
	Source/LuminoEngine/Source/Graphics/FrameRectRenderer.h
	Source/LuminoEngine/Source/Graphics/GeometryRenderer.cpp
	Source/LuminoEngine/Source/Graphics/GeometryRenderer.h
	Source/LuminoEngine/Source/Graphics/Graphics.cpp
	Source/LuminoEngine/Source/Graphics/GraphicsException.cpp
	Source/LuminoEngine/Source/Graphics/GraphicsManager.cpp
	Source/LuminoEngine/Source/Graphics/GraphicsManager.h
	Source/LuminoEngine/Source/Graphics/IndexBuffer.cpp
	Source/LuminoEngine/Source/Graphics/Internal.h
	Source/LuminoEngine/Source/Graphics/Material.cpp
	Source/LuminoEngine/Source/Graphics/MeshRendererProxy.cpp
	Source/LuminoEngine/Source/Graphics/MeshRendererProxy.h
	Source/LuminoEngine/Source/Graphics/NanoVGRenderer.cpp
	Source/LuminoEngine/Source/Graphics/NanoVGRenderer.h
	Source/LuminoEngine/Source/Graphics/PerlinNoise.cpp
	Source/LuminoEngine/Source/Graphics/PngFile.h
	Source/LuminoEngine/Source/Graphics/PrimitiveRenderer.cpp
	Source/LuminoEngine/Source/Graphics/PrimitiveRenderer.h
	Source/LuminoEngine/Source/Graphics/ProfilerRenderer.cpp
	Source/LuminoEngine/Source/Graphics/ProfilerRenderer.h
	Source/LuminoEngine/Source/Graphics/Renderer.cpp
	Source/LuminoEngine/Source/Graphics/RendererImpl.h
	Source/LuminoEngine/Source/Graphics/Rendering.cpp
	Source/LuminoEngine/Source/Graphics/RenderingCommand.cpp
	Source/LuminoEngine/Source/Graphics/RenderingCommand.h
	Source/LuminoEngine/Source/Graphics/RenderingContext.cpp
	Source/LuminoEngine/Source/Graphics/RenderingThread.cpp
	Source/LuminoEngine/Source/Graphics/RenderingThread.h
	Source/LuminoEngine/Source/Graphics/RenderState.cpp
	Source/LuminoEngine/Source/Graphics/RenderTargetTextureCache.cpp
	Source/LuminoEngine/Source/Graphics/RenderTargetTextureCache.h
	Source/LuminoEngine/Source/Graphics/SamplerState.cpp
	Source/LuminoEngine/Source/Graphics/Shader.cpp
	Source/LuminoEngine/Source/Graphics/SpriteRenderer.cpp
	Source/LuminoEngine/Source/Graphics/SpriteRenderer.h
	Source/LuminoEngine/Source/Graphics/SwapChain.cpp
	Source/LuminoEngine/Source/Graphics/Texture.cpp
	Source/LuminoEngine/Source/Graphics/Utils.cpp
	Source/LuminoEngine/Source/Graphics/VertexBuffer.cpp
	Source/LuminoEngine/Source/Graphics/VertexDeclaration.cpp
	Source/LuminoEngine/Source/Graphics/Viewport.cpp
	Source/LuminoEngine/Source/Graphics/ImageEffect/ImageEffect.cpp
	Source/LuminoEngine/Source/Graphics/ImageEffect/BlurImageEffect.cpp
	Source/LuminoEngine/Source/Graphics/ImageEffect/ScreenMotionBlurImageEffect.cpp
	Source/LuminoEngine/Source/Graphics/ImageEffect/ToneImageEffect.cpp
)
set(LN_SOURCES_Graphics_Device
	Source/LuminoEngine/Source/Graphics/Device/GraphicsDriverInterface.cpp
	Source/LuminoEngine/Source/Graphics/Device/GraphicsDriverInterface.h
	Source/LuminoEngine/Source/Graphics/Device/GraphicsDeviceBase.cpp
	Source/LuminoEngine/Source/Graphics/Device/GraphicsDeviceBase.h
	Source/LuminoEngine/Source/Graphics/Device/ShaderVariableBase.cpp
	Source/LuminoEngine/Source/Graphics/Device/ShaderVariableBase.h
	Source/LuminoEngine/Source/Graphics/Device/OpenGL/GLGraphicsDevice.cpp
	Source/LuminoEngine/Source/Graphics/Device/OpenGL/GLGraphicsDevice.h
	Source/LuminoEngine/Source/Graphics/Device/OpenGL/GLIndexBuffer.cpp
	Source/LuminoEngine/Source/Graphics/Device/OpenGL/GLIndexBuffer.h
	Source/LuminoEngine/Source/Graphics/Device/OpenGL/GLRenderer.cpp
	Source/LuminoEngine/Source/Graphics/Device/OpenGL/GLRenderer.h
	Source/LuminoEngine/Source/Graphics/Device/OpenGL/GLShader.cpp
	Source/LuminoEngine/Source/Graphics/Device/OpenGL/GLShader.h
	Source/LuminoEngine/Source/Graphics/Device/OpenGL/GLSwapChain.cpp
	Source/LuminoEngine/Source/Graphics/Device/OpenGL/GLSwapChain.h
	Source/LuminoEngine/Source/Graphics/Device/OpenGL/GLTexture.cpp
	Source/LuminoEngine/Source/Graphics/Device/OpenGL/GLTexture.h
	Source/LuminoEngine/Source/Graphics/Device/OpenGL/GLVertexBuffer.cpp
	Source/LuminoEngine/Source/Graphics/Device/OpenGL/GLVertexBuffer.h
)
set(LN_SOURCES_Graphics_Text
	Source/LuminoEngine/Include/Lumino/Graphics/Text/Font.h
	Source/LuminoEngine/Include/Lumino/Graphics/Text/GlyphRun.h
	Source/LuminoEngine/Source/Graphics/Text/BitmapFont.cpp
	Source/LuminoEngine/Source/Graphics/Text/BitmapFont.h
	Source/LuminoEngine/Source/Graphics/Text/BitmapTextRenderer.cpp
	Source/LuminoEngine/Source/Graphics/Text/BitmapTextRenderer.h
	Source/LuminoEngine/Source/Graphics/Text/Font.cpp
	Source/LuminoEngine/Source/Graphics/Text/FontGlyphTextureCache.cpp
	Source/LuminoEngine/Source/Graphics/Text/FontGlyphTextureCache.h
	Source/LuminoEngine/Source/Graphics/Text/FontManager.cpp
	Source/LuminoEngine/Source/Graphics/Text/FontManager.h
	Source/LuminoEngine/Source/Graphics/Text/FreeTypeFont.cpp
	Source/LuminoEngine/Source/Graphics/Text/FreeTypeFont.h
	Source/LuminoEngine/Source/Graphics/Text/GlyphRun.cpp
	Source/LuminoEngine/Source/Graphics/Text/TextLayoutEngine.cpp
	Source/LuminoEngine/Source/Graphics/Text/TextLayoutEngine.h
	Source/LuminoEngine/Source/Graphics/Text/TextRenderer.cpp
	Source/LuminoEngine/Source/Graphics/Text/TextRenderer.h
)
set(LN_SOURCES_Graphics_Mesh
	Source/LuminoEngine/Include/Lumino/Graphics/Mesh/GizmoModel.h
	Source/LuminoEngine/Include/Lumino/Graphics/Mesh/Mesh.h
	Source/LuminoEngine/Include/Lumino/Graphics/Mesh/SkinnedMeshModel.h
	Source/LuminoEngine/Source/Graphics/Mesh/GizmoModel.cpp
	Source/LuminoEngine/Source/Graphics/Mesh/Mesh.cpp
	Source/LuminoEngine/Source/Graphics/Mesh/MeshFactory.h
	Source/LuminoEngine/Source/Graphics/Mesh/SkinnedMeshModel.cpp
)

set(LN_SOURCES_Effect
	Source/LuminoEngine/Include/Lumino/Effect/Common.h
	Source/LuminoEngine/Include/Lumino/Effect/VisualEffect.h
	Source/LuminoEngine/Include/Lumino/Effect/VisualEffectInstance.h
	Source/LuminoEngine/Source/Effect/Effect.cpp
	Source/LuminoEngine/Source/Effect/EffectDriver.h
	Source/LuminoEngine/Source/Effect/EffectDriver.cpp
	Source/LuminoEngine/Source/Effect/EffectManager.h
	Source/LuminoEngine/Source/Effect/EffectManager.cpp
	Source/LuminoEngine/Source/Effect/Internal.h
)

if (LN_BUILD_SCENE_MODULE)
set(LN_SOURCES_TileMap
	Source/LuminoEngine/Include/Lumino/TileMap/Common.h
	Source/LuminoEngine/Include/Lumino/TileMap/TileLayer.h
	Source/LuminoEngine/Include/Lumino/TileMap/TileMapModel.h
	Source/LuminoEngine/Include/Lumino/TileMap/TileMapRenderer.h
	Source/LuminoEngine/Include/Lumino/TileMap/TileSet.h
	Source/LuminoEngine/Source/TileMap/TileLayer.cpp
	Source/LuminoEngine/Source/TileMap/TileMapModel.cpp
	Source/LuminoEngine/Source/TileMap/TileMapRenderer.cpp
	Source/LuminoEngine/Source/TileMap/TileSet.cpp
)

set(LN_SOURCES_Modeling
	Source/LuminoEngine/Source/Modeling/Common.h
	Source/LuminoEngine/Source/Modeling/ModelManager.cpp
	Source/LuminoEngine/Source/Modeling/ModelManager.h
	Source/LuminoEngine/Source/Modeling/PmxLoader.cpp
	Source/LuminoEngine/Source/Modeling/PmxLoader.h
	Source/LuminoEngine/Source/Modeling/PmxSkinnedMesh.cpp
	Source/LuminoEngine/Source/Modeling/PmxSkinnedMesh.h
)

set(LN_SOURCES_Scene
	Source/LuminoEngine/Include/Lumino/Scene/Camera.h
	Source/LuminoEngine/Include/Lumino/Scene/Common.h
	Source/LuminoEngine/Include/Lumino/Scene/Detail.h
	Source/LuminoEngine/Include/Lumino/Scene/EffectNode.h
	Source/LuminoEngine/Include/Lumino/Scene/GpuParticle.h
	Source/LuminoEngine/Include/Lumino/Scene/Grid.h
	Source/LuminoEngine/Include/Lumino/Scene/Layer.h
	Source/LuminoEngine/Include/Lumino/Scene/Light.h
	Source/LuminoEngine/Include/Lumino/Scene/MeshModelObject.h
#	Source/LuminoEngine/Include/Lumino/Scene/MMDSceneGraph.h
	Source/LuminoEngine/Include/Lumino/Scene/SceneGraph.h
	Source/LuminoEngine/Include/Lumino/Scene/SceneNode.h
	Source/LuminoEngine/Include/Lumino/Scene/SceneTR.h
	Source/LuminoEngine/Include/Lumino/Scene/Sprite.h
	Source/LuminoEngine/Include/Lumino/Scene/SpriteModelObject.h
	Source/LuminoEngine/Include/Lumino/Scene/Particle.h
	Source/LuminoEngine/Include/Lumino/Scene/StaticMesh.h
	Source/LuminoEngine/Include/Lumino/Scene/TextBlock.h
	Source/LuminoEngine/Include/Lumino/Scene/TileMap.h
	Source/LuminoEngine/Include/Lumino/Scene/VisualNode.h
#	Source/LuminoEngine/Include/Lumino/Scene/MME/MMESceneObject.h
	Source/LuminoEngine/Source/Scene/Camera.cpp
	Source/LuminoEngine/Source/Scene/Internal.h
	Source/LuminoEngine/Source/Scene/DebugRenderer.h
	Source/LuminoEngine/Source/Scene/EffectBatchRendererNode.cpp
	Source/LuminoEngine/Source/Scene/EffectBatchRendererNode.h
	Source/LuminoEngine/Source/Scene/EffectNode.cpp
	Source/LuminoEngine/Source/Scene/GpuParticle.cpp
	Source/LuminoEngine/Source/Scene/Grid.cpp
	Source/LuminoEngine/Source/Scene/InfomationRenderingPass.cpp
	Source/LuminoEngine/Source/Scene/InfomationRenderingPass.h
	Source/LuminoEngine/Source/Scene/Layer.cpp
	Source/LuminoEngine/Source/Scene/Light.cpp
	Source/LuminoEngine/Source/Scene/MeshModelObject.cpp
#	Source/LuminoEngine/Source/Scene/MMDSceneGraph.cpp
	Source/LuminoEngine/Source/Scene/RenderingPass.cpp
	Source/LuminoEngine/Source/Scene/RenderingPass.h
	Source/LuminoEngine/Source/Scene/SceneGraph.cpp
	Source/LuminoEngine/Source/Scene/SceneGraphManager.cpp
	Source/LuminoEngine/Source/Scene/SceneGraphManager.h
	Source/LuminoEngine/Source/Scene/SceneNode.cpp
	Source/LuminoEngine/Source/Scene/SceneTR.cpp
	Source/LuminoEngine/Source/Scene/Sprite.cpp
	Source/LuminoEngine/Source/Scene/SpriteModelObject.cpp
	Source/LuminoEngine/Source/Scene/Particle.cpp
	Source/LuminoEngine/Source/Scene/StaticMesh.cpp
	Source/LuminoEngine/Source/Scene/TextBlock.cpp
	Source/LuminoEngine/Source/Scene/TileMap.cpp
	Source/LuminoEngine/Source/Scene/VisualNode.cpp
	external/ssbpLib/ssShader_frag.h
	external/ssbpLib/SS5Player.cpp
	external/ssbpLib/SS5Player.h
	external/ssbpLib/SS5PlayerData.h
	external/ssbpLib/SS5PlayerPlatform.cpp
	external/ssbpLib/SS5PlayerPlatform.h
	external/ssbpLib/SS5PlayerTypes.h
	external/ssbpLib/common/Animator/ssplayer_PartState.h
	external/ssbpLib/common/Animator/ssplayer_cellmap.h
	external/ssbpLib/common/Animator/ssplayer_effect.cpp
	external/ssbpLib/common/Animator/ssplayer_effect.h
	external/ssbpLib/common/Animator/ssplayer_effectfunction.cpp
	external/ssbpLib/common/Animator/ssplayer_effectfunction.h
	external/ssbpLib/common/Animator/ssplayer_macro.h
	external/ssbpLib/common/Animator/ssplayer_matrix.cpp
	external/ssbpLib/common/Animator/ssplayer_matrix.h
	external/ssbpLib/common/Animator/ssplayer_PartState.cpp
)
set(LN_SOURCES_Scene_MMD
	Source/LuminoEngine/Source/Scene/MME/MmdMaterial.cpp
	Source/LuminoEngine/Source/Scene/MME/MmdMaterial.h
	Source/LuminoEngine/Source/Scene/MME/MMERenderingPass.cpp
	Source/LuminoEngine/Source/Scene/MME/MMERenderingPass.h
	Source/LuminoEngine/Source/Scene/MME/MMESceneShaderManager.cpp
	Source/LuminoEngine/Source/Scene/MME/MMESceneShaderManager.h
	Source/LuminoEngine/Source/Scene/MME/ShaderScriptCommandList.cpp
	Source/LuminoEngine/Source/Scene/MME/ShaderScriptCommandList.h
	Source/LuminoEngine/Source/Scene/MME/MMEShader.cpp
	Source/LuminoEngine/Source/Scene/MME/MMEShader.h
	Source/LuminoEngine/Source/Scene/MME/MMEShaderBuilder.cpp
	Source/LuminoEngine/Source/Scene/MME/MMEShaderBuilder.h
	Source/LuminoEngine/Source/Scene/MME/MMEShaderErrorInfo.cpp
	Source/LuminoEngine/Source/Scene/MME/MMEShaderErrorInfo.h
	Source/LuminoEngine/Source/Scene/MME/MMEShaderTechnique.cpp
	Source/LuminoEngine/Source/Scene/MME/MMEShaderTechnique.h
	Source/LuminoEngine/Source/Scene/MME/MMETypes.h
)
endif()

set(LN_SOURCES_UI
	Source/LuminoEngine/Include/Lumino/UI/Common.h
	Source/LuminoEngine/Include/Lumino/UI/Detail.h
	Source/LuminoEngine/Include/Lumino/UI/LayoutElement.h
	Source/LuminoEngine/Include/Lumino/UI/UIButton.h
	Source/LuminoEngine/Include/Lumino/UI/UIContentControl.h
	Source/LuminoEngine/Include/Lumino/UI/UIContext.h
	Source/LuminoEngine/Include/Lumino/UI/UIControl.h
	Source/LuminoEngine/Include/Lumino/UI/UIElement.h
	Source/LuminoEngine/Include/Lumino/UI/UIElementCollection.h
	Source/LuminoEngine/Include/Lumino/UI/UIEvent.h
	Source/LuminoEngine/Include/Lumino/UI/UIEventArgs.h
	Source/LuminoEngine/Include/Lumino/UI/UIFrameWindow.h
	Source/LuminoEngine/Include/Lumino/UI/UIInjectedInputReceiver.h
	Source/LuminoEngine/Include/Lumino/UI/UIItemsControl.h
	Source/LuminoEngine/Include/Lumino/UI/UILayoutPanel.h
	Source/LuminoEngine/Include/Lumino/UI/UILayoutRoot.h
	Source/LuminoEngine/Include/Lumino/UI/UILayoutView.h
	Source/LuminoEngine/Include/Lumino/UI/UIListBox.h
	Source/LuminoEngine/Include/Lumino/UI/UIStyle.h
	Source/LuminoEngine/Include/Lumino/UI/UITextBlock.h
	Source/LuminoEngine/Include/Lumino/UI/UITextElement.h
	Source/LuminoEngine/Include/Lumino/UI/UITypeInfo.h
	Source/LuminoEngine/Include/Lumino/UI/UIWindow.h
	Source/LuminoEngine/Source/UI/EventArgsPool.h
	Source/LuminoEngine/Source/UI/Internal.h
	Source/LuminoEngine/Source/UI/LayoutElement.cpp
	Source/LuminoEngine/Source/UI/LayoutImpl.h
	Source/LuminoEngine/Source/UI/UIButton.cpp
	Source/LuminoEngine/Source/UI/UIContentControl.cpp
	Source/LuminoEngine/Source/UI/UIContext.cpp
	Source/LuminoEngine/Source/UI/UIControl.cpp
	Source/LuminoEngine/Source/UI/UIElement.cpp
	Source/LuminoEngine/Source/UI/UIElementCollection.cpp
	Source/LuminoEngine/Source/UI/UIEvent.cpp
	Source/LuminoEngine/Source/UI/UIEventArgs.cpp
	Source/LuminoEngine/Source/UI/UIFrameWindow.cpp
	Source/LuminoEngine/Source/UI/UIHelper.h
	Source/LuminoEngine/Source/UI/UIItemsControl.cpp
	Source/LuminoEngine/Source/UI/UILayoutPanel.cpp
	Source/LuminoEngine/Source/UI/UILayoutRoot.cpp
	Source/LuminoEngine/Source/UI/UILayoutView.cpp
	Source/LuminoEngine/Source/UI/UIListBox.cpp
	Source/LuminoEngine/Source/UI/UIManager.cpp
	Source/LuminoEngine/Source/UI/UIManager.h
	Source/LuminoEngine/Source/UI/UIStyle.cpp
	Source/LuminoEngine/Source/UI/UITextBlock.cpp
	Source/LuminoEngine/Source/UI/UITextElement.cpp
	Source/LuminoEngine/Source/UI/UITypeInfo.cpp
	Source/LuminoEngine/Source/UI/UIWindow.cpp
)

set(LN_SOURCES_Documents
	Source/LuminoEngine/Include/Lumino/Documents/Common.h
	Source/LuminoEngine/Source/Documents/DocumentElements.cpp
	Source/LuminoEngine/Source/Documents/DocumentElements.h
	Source/LuminoEngine/Source/Documents/DocumentsManager.cpp
	Source/LuminoEngine/Source/Documents/DocumentsManager.h
	Source/LuminoEngine/Source/Documents/Internal.h
)

set(LN_SOURCES_Game
	Source/LuminoEngine/Include/Lumino/Game/Component.h
	Source/LuminoEngine/Include/Lumino/Game/FpsController.h
	Source/LuminoEngine/Source/Game/Component.cpp
	Source/LuminoEngine/Source/Game/FpsController.cpp
)

if (MSVC_VERSION GREATER 1800)		# (MSVC > vs2013)
set(LN_SOURCES_Scripting
	Source/LuminoEngine/Source/Scripting/Common.h
	Source/LuminoEngine/Source/Scripting/NlFunctionNodes.cpp
	Source/LuminoEngine/Source/Scripting/NlFunctionNodes.h
	Source/LuminoEngine/Source/Scripting/NlVariant.cpp
	Source/LuminoEngine/Source/Scripting/NlVariant.h
	Source/LuminoEngine/Source/Scripting/ScriptingManager.cpp
	Source/LuminoEngine/Source/Scripting/ScriptingManager.h
)
endif()

set(LN_SOURCES_Engine
	Source/LuminoEngine/Include/Lumino/Assets.h
	Source/LuminoEngine/Include/Lumino/BindingSupport.h
	Source/LuminoEngine/Include/Lumino/Common.h
	Source/LuminoEngine/Include/Lumino/Engine.h
	Source/LuminoEngine/Include/Lumino/EngineDiag.h
	Source/LuminoEngine/Include/Lumino/EngineSettings.h
	Source/LuminoEngine/Include/Lumino/GameTime.h
	Source/LuminoEngine/Include/Lumino/MultiThreadingObjectList.h
	Source/LuminoEngine/Include/Lumino/Profiler.h
	Source/LuminoEngine/Include/Lumino/RoutedEvent.h
	Source/LuminoEngine/Include/Lumino/Version.h
	Source/LuminoEngine/Source/AssetsManager.cpp
	Source/LuminoEngine/Source/AssetsManager.h
	Source/LuminoEngine/Source/Engine.cpp
	Source/LuminoEngine/Source/EngineDiagCore.cpp
	Source/LuminoEngine/Source/EngineDiagCore.h
	Source/LuminoEngine/Source/EngineDiagViewer.h
	Source/LuminoEngine/Source/EngineManager.cpp
	Source/LuminoEngine/Source/EngineManager.h
	Source/LuminoEngine/Source/GameTime.cpp
	Source/LuminoEngine/Source/Internal.h
	Source/LuminoEngine/Source/Profiler.cpp
	Source/LuminoEngine/Source/Property.cpp
	Source/LuminoEngine/Source/Resource.cpp
	Source/LuminoEngine/Source/Resource.h
	Source/LuminoEngine/Source/RoutedEvent.cpp
	Source/LuminoEngine/Source/Version.cpp
)

set(LN_SOURCES_Foundation
	Source/LuminoEngine/Include/Lumino/Foundation/Application.h
	Source/LuminoEngine/Include/Lumino/Foundation/GameScene.h
	Source/LuminoEngine/Source/Foundation/Application.cpp
	Source/LuminoEngine/Source/Foundation/EntryPoint.cpp
	Source/LuminoEngine/Source/Foundation/EntryPoint.h
	Source/LuminoEngine/Source/Foundation/GameSceneManager.cpp
	Source/LuminoEngine/Source/Foundation/GameSceneManager.h
)

#--------------------------------------
# External linker source files.
if (LN_USE_EXTERNAL_Effekseer)
	set(LN_SOURCES_Effect
		${LN_SOURCES_Effect}
		Source/LuminoEngine/Source/Effect/Effekseer/EffekseerDriver.h
		Source/LuminoEngine/Source/Effect/Effekseer/EffekseerDriver.cpp
	)
endif()

#--------------------------------------
# Platform source files.
if (WIN32)
	set(LN_SOURCES_Platform
		${LN_SOURCES_Platform}
		Source/LuminoEngine/Include/Lumino/Platform/Win32/Win32PlatformWindow.h
		Source/LuminoEngine/Source/Platform/Win32/Clipboard_Win32.cpp
		Source/LuminoEngine/Source/Platform/Win32/Ime_Win32.cpp
		Source/LuminoEngine/Source/Platform/Win32/PlatformSupport_Win32.cpp
		Source/LuminoEngine/Source/Platform/Win32/Win32PlatformFileOpenDialog.h
		Source/LuminoEngine/Source/Platform/Win32/Win32PlatformWindow.cpp
		Source/LuminoEngine/Source/Platform/Win32/Win32PlatformWindowManager.cpp
		Source/LuminoEngine/Source/Platform/Win32/Win32PlatformWindowManager.h
	)
	set(LN_SOURCES_Input
		${LN_SOURCES_Input}
		Source/LuminoEngine/Source/Input/Win32InputDriver.cpp
		Source/LuminoEngine/Source/Input/Win32InputDriver.h
		Source/LuminoEngine/Source/Input/Win32JoystickDriver.cpp
		Source/LuminoEngine/Source/Input/Win32JoystickDriver.h
	)
 	set(LN_SOURCES_Graphics_Device
 		${LN_SOURCES_Graphics_Device}
		Source/LuminoEngine/Source/Graphics/Device/DirectX9/DX9Texture.h
		Source/LuminoEngine/Source/Graphics/Device/DirectX9/DX9VertexBuffer.h
		Source/LuminoEngine/Source/Graphics/Device/DirectX9/DX9GraphicsDevice.h
		Source/LuminoEngine/Source/Graphics/Device/DirectX9/DX9IndexBuffer.h
		Source/LuminoEngine/Source/Graphics/Device/DirectX9/DX9Module.h
		Source/LuminoEngine/Source/Graphics/Device/DirectX9/DX9Renderer.h
		Source/LuminoEngine/Source/Graphics/Device/DirectX9/DX9Shader.h
		Source/LuminoEngine/Source/Graphics/Device/DirectX9/DX9SwapChain.h
	 	Source/LuminoEngine/Source/Graphics/Device/DirectX9/DX9Texture.cpp
		Source/LuminoEngine/Source/Graphics/Device/DirectX9/DX9VertexBuffer.cpp
		Source/LuminoEngine/Source/Graphics/Device/DirectX9/DX9GraphicsDevice.cpp
		Source/LuminoEngine/Source/Graphics/Device/DirectX9/DX9IndexBuffer.cpp
		Source/LuminoEngine/Source/Graphics/Device/DirectX9/DX9Module.cpp
		Source/LuminoEngine/Source/Graphics/Device/DirectX9/DX9Renderer.cpp
		Source/LuminoEngine/Source/Graphics/Device/DirectX9/DX9Shader.cpp
		Source/LuminoEngine/Source/Graphics/Device/DirectX9/DX9SwapChain.cpp
		Source/LuminoEngine/Source/Graphics/Device/OpenGL/WGLGraphicsDevice.cpp
		Source/LuminoEngine/Source/Graphics/Device/OpenGL/WGLGraphicsDevice.h
	)
	set(LN_SOURCES_Modeling
 		${LN_SOURCES_Modeling}
		Source/LuminoEngine/Source/Modeling/XFileLoader.cpp
		Source/LuminoEngine/Source/Modeling/XFileLoader.h
	)
	set(LN_SOURCES_Audio
 		${LN_SOURCES_Audio}
		Source/LuminoEngine/Source/Audio/Mp3Decoder.cpp
		Source/LuminoEngine/Source/Audio/Mp3Decoder.h
		Source/LuminoEngine/Source/Audio/XAudio2/XAudio2AudioDevice.cpp
		Source/LuminoEngine/Source/Audio/XAudio2/XAudio2AudioDevice.h
		Source/LuminoEngine/Source/Audio/XAudio2/XAudio2AudioPlayer.cpp
		Source/LuminoEngine/Source/Audio/XAudio2/XAudio2AudioPlayer.h
		Source/LuminoEngine/Source/Audio/XAudio2/XAudio2Module.cpp
		Source/LuminoEngine/Source/Audio/XAudio2/XAudio2Module.h
		Source/LuminoEngine/Source/Audio/DirectMusic/DirectMusic.cpp
		Source/LuminoEngine/Source/Audio/DirectMusic/DirectMusic.h
		Source/LuminoEngine/Source/Audio/DirectMusic/DirectMusicAudioDevice.cpp
		Source/LuminoEngine/Source/Audio/DirectMusic/DirectMusicAudioDevice.h
		Source/LuminoEngine/Source/Audio/DirectMusic/DirectMusicAudioPlayer.cpp
		Source/LuminoEngine/Source/Audio/DirectMusic/DirectMusicAudioPlayer.h
	)

	set(LN_SOURCES_Foundation
		${LN_SOURCES_Foundation}
		Source/LuminoEngine/Source/Foundation/Main_Win32.cpp
	)


elseif (APPLE)
	set(LN_SOURCES_Platform
		${LN_SOURCES_Platform}
		Source/LuminoEngine/Source/Platform/Cocoa/CocoaPlatformWindow.mm
		Source/LuminoEngine/Source/Platform/Cocoa/CocoaPlatformWindow.h
		Source/LuminoEngine/Source/Platform/Cocoa/CocoaPlatformWindowManager.mm
		Source/LuminoEngine/Source/Platform/Cocoa/CocoaPlatformWindowManager.h
	)
	set(LN_SOURCES_Input
		${LN_SOURCES_Input}
		Source/LuminoEngine/Source/Input/CocoaInputDriver.cpp
		Source/LuminoEngine/Source/Input/CocoaInputDriver.h
	)
	set(LN_SOURCES_Graphics_Device
		${LN_SOURCES_Graphics_Device}
		Source/LuminoEngine/Source/Graphics/Device/OpenGL/NSGLGraphicsDevice.mm
		Source/LuminoEngine/Source/Graphics/Device/OpenGL/NSGLGraphicsDevice.h
	)	
	set(LN_SOURCES_Foundation
		${LN_SOURCES_Foundation}
		Source/LuminoEngine/Source/Foundation/Main_Mac.cpp
	)


elseif (UNIX)
	set(LN_SOURCES_Platform
		${LN_SOURCES_Platform}
		Source/LuminoEngine/Source/Platform/X11/X11WindowManager.cpp
		Source/LuminoEngine/Source/Platform/X11/X11Window.cpp
		Source/LuminoEngine/Source/Platform/X11/PlatformSupport_X11.cp
	)

	set(LN_SOURCES_Graphics
		${LN_SOURCES_Graphics}
		Source/LuminoEngine/Source/Graphics/Device/OpenGL/GLXGraphicsDevice.cpp
		Source/LuminoEngine/Source/Graphics/Device/OpenGL/GLXSwapChain.cpp
	)

	set(LN_SOURCES_Audio
		${LN_SOURCES_Audio}
		Source/LuminoEngine/Source/Audio/XAudio2/XAudio2AudioDevice.cpp
		Source/LuminoEngine/Source/Audio/XAudio2/XAudio2AudioDevice.h
		Source/LuminoEngine/Source/Audio/XAudio2/XAudio2AudioPlayer.cpp
		Source/LuminoEngine/Source/Audio/XAudio2/XAudio2AudioPlayer.h
		Source/LuminoEngine/Source/Audio/DirectMusic/DirectMusic.cpp
		Source/LuminoEngine/Source/Audio/DirectMusic/DirectMusic.h
		Source/LuminoEngine/Source/Audio/DirectMusic/DirectMusicAudioDevice.cpp
		Source/LuminoEngine/Source/Audio/DirectMusic/DirectMusicAudioDevice.h
		Source/LuminoEngine/Source/Audio/DirectMusic/DirectMusicAudioPlayer.cpp
		Source/LuminoEngine/Source/Audio/DirectMusic/DirectMusicAudioPlayer.h
	)

	add_definitions(-DGLEW_NO_GLU)
	#add_definitions(-DGLEW_MX)
	add_definitions(-DGLEW_BUILD)
	#add_definitions(-DGLEW_STATIC)
	add_definitions("-std=c++11")
endif()

#--------------------------------------
# Grouping.
source_group("Base"				FILES ${LN_SOURCES_Base})
source_group("Animation"		FILES ${LN_SOURCES_Animation})
source_group("IO"				FILES ${LN_SOURCES_IO})
source_group("Platform"			FILES ${LN_SOURCES_Platform})
source_group("Input"			FILES ${LN_SOURCES_Input})
source_group("Audio"			FILES ${LN_SOURCES_Audio})
source_group("Physics"			FILES ${LN_SOURCES_Physics})
source_group("Graphics"			FILES ${LN_SOURCES_Graphics})
source_group("Graphics_Device"	FILES ${LN_SOURCES_Graphics_Device})
source_group("Graphics_Text"	FILES ${LN_SOURCES_Graphics_Text})
source_group("Graphics_Mesh"	FILES ${LN_SOURCES_Graphics_Mesh})
source_group("Effect"			FILES ${LN_SOURCES_Effect})
source_group("Tilemap"			FILES ${LN_SOURCES_TileMap})
source_group("Modeling"			FILES ${LN_SOURCES_Modeling})
source_group("Scene"			FILES ${LN_SOURCES_Scene})
source_group("Scene_MMD"		FILES ${LN_SOURCES_Scene_MMD})
source_group("UI"				FILES ${LN_SOURCES_UI})
source_group("Documents"		FILES ${LN_SOURCES_Documents})
source_group("Game"				FILES ${LN_SOURCES_Game})
source_group("Scripting"		FILES ${LN_SOURCES_Scripting})
source_group("Engine"			FILES ${LN_SOURCES_Engine})
source_group("Foundation"		FILES ${LN_SOURCES_Foundation})

set(LN_SOURCES
	${LN_SOURCES}
	${LN_SOURCES_Base}
	${LN_SOURCES_Animation}
	${LN_SOURCES_IO}
	${LN_SOURCES_Platform}
	${LN_SOURCES_Input}
	${LN_SOURCES_Audio}
	${LN_SOURCES_Physics}
	${LN_SOURCES_Graphics}
	${LN_SOURCES_Graphics_Device}
	${LN_SOURCES_Graphics_Text}
	${LN_SOURCES_Graphics_Mesh}
	${LN_SOURCES_Effect}
	${LN_SOURCES_TileMap}
	${LN_SOURCES_Modeling}
	${LN_SOURCES_Scene}
#	${LN_SOURCES_Scene_MMD}
	${LN_SOURCES_UI}
	${LN_SOURCES_Documents}
	${LN_SOURCES_Game}
	${LN_SOURCES_Scripting}
	${LN_SOURCES_Engine}
	${LN_SOURCES_C_API}
)

#------------------------------------------------------------------------------
# Apply options
#------------------------------------------------------------------------------
# Unicode char set.
if (LN_USE_UNICODE_CHAR_SET)
	#add_definitions(-DUNICODE)
	#add_definitions(-D_UNICODE)
endif()

find_package(OpenGL REQUIRED)

if (WIN32)
	if (LN_MSVC_LINK_MULTI_THREAD_STATIC_RUNTIME)
		foreach (flag CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE)
			if (${flag} MATCHES "/MD")
				string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
			endif()
			if (${flag} MATCHES "/MDd")
				string(REGEX REPLACE "/MDd" "/MTd" ${flag} "${${flag}}")
			endif()
		endforeach()
	endif()

	set(CMAKE_DEBUG_POSTFIX "d")

	# platform include dirs.
	set(LN_PLATFORM_INCLUDE_DIRS "$ENV{DXSDK_DIR}Include")

	# link librarys.
	list(APPEND LN_LINK_LIBS
		"imagehlp.lib"
		"msacm32.lib"
		"Opengl32.lib")


	# effekseer
	link_directories("$ENV{DXSDK_DIR}Lib/x86")

	add_definitions("/EHsc")	# for VS2015 error

elseif (APPLE)
	set(LN_UNIT_TEST_LINK_LIBS "pthread")
	set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")

	find_library(COCOA_FRAMEWORK Cocoa)
    find_library(IOKIT_FRAMEWORK IOKit)
    find_library(CORE_FOUNDATION_FRAMEWORK CoreFoundation)
    find_library(CORE_VIDEO_FRAMEWORK CoreVideo)
	list(APPEND LN_LINK_LIBS "${COCOA_FRAMEWORK}"
			  "${OPENGL_gl_LIBRARY}"
			  "${IOKIT_FRAMEWORK}"
			  "${CORE_FOUNDATION_FRAMEWORK}"
			  "${CORE_VIDEO_FRAMEWORK}")

elseif (UNIX)
	set(LN_UNIT_TEST_LINK_LIBS "pthread")

	# link librarys.
	list(APPEND LN_LINK_LIBS "dl")
	list(APPEND LN_LINK_LIBS "bfd")
    find_package(X11 REQUIRED)

    #list(APPEND glfw_INCLUDE_DIRS ${X11_X11_INCLUDE_PATH})
    list(APPEND LN_LINK_LIBS ${X11_X11_LIB})

	#list(APPEND glfw_INCLUDE_DIRS ${OPENGL_INCLUDE_DIR})
    list(APPEND LN_LINK_LIBS ${OPENGL_gl_LIBRARY})

else()
	# Not support.
    message(FATAL_ERROR "No supported platform was detected.")
endif()

set(LN_INCLUDE_DIRECTORIES
	${PROJECT_SOURCE_DIR}/Source/LuminoEngine/Include
	${PROJECT_SOURCE_DIR}/external
	${PROJECT_SOURCE_DIR}/Source/LuminoCore/Include
	${PROJECT_SOURCE_DIR}/external/libpng
	${PROJECT_SOURCE_DIR}/external/bullet-2.82
	${PROJECT_SOURCE_DIR}/external/freetype-2.6/include
	${PROJECT_SOURCE_DIR}/external/libogg/include
	${PROJECT_SOURCE_DIR}/external/glfw-3.1.2/include
	${PROJECT_SOURCE_DIR}/external/EffekseerRuntime101/Source/LuminoEngine/Source/Effekseer
)
include_directories(${LN_INCLUDE_DIRECTORIES} ${LN_PLATFORM_INCLUDE_DIRS})


#------------------------------------------------------------------------------
# External
#------------------------------------------------------------------------------

add_subdirectory(Source/LuminoCore)
link_libraries(LuminoCore_Static)
link_libraries(LuminoMath)

add_subdirectory(Source/Fluorite)

# Make postfix.
ln_make_postfix(LN_LIB_POSTFIX)



#------------------------------------------------------------------------------
# Output
#------------------------------------------------------------------------------

add_definitions(-DLUMINO_ROOT_DIR="${PROJECT_SOURCE_DIR}")

#set(CMAKE_CXX_FLAGS "")
#set(CMAKE_CXX_FLAGS_DEBUG "-g -pg")
#set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -pg -O3")
#set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -O3")
#set(CMAKE_CXX_FLAGS_MINSIZEREL "")

if (LN_EXCEPTION_BACKTRACE)
	add_definitions(-DLN_EXCEPTION_BACKTRACE)
endif()

#------------------------------
# library. (Unicode)

if (LN_USE_UNICODE_CHAR_SET)
	add_definitions(-DUNICODE)
	add_definitions(-D_UNICODE)
endif()
if (LN_BUILD_AUDIO_MODULE)
	add_definitions(-DLN_BUILD_AUDIO_MODULE)
endif()
if (LN_BUILD_SCENE_MODULE)
	add_definitions(-DLN_BUILD_SCENE_MODULE)
endif()
if (LN_USE_EXTERNAL_Effekseer)
	add_definitions(-DLN_USE_EXTERNAL_Effekseer)
endif()


# LuminoEngine
add_library(LuminoEngine STATIC ${LN_SOURCES})
set_target_properties(LuminoEngine PROPERTIES OUTPUT_NAME "LuminoEngine${LN_LIB_POSTFIX}")
set_target_properties(LuminoEngine PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${LN_LIB_OUTPUT_DIR}")
target_include_directories(LuminoEngine PUBLIC ${LN_DEPENDENCIES_INCLUDES})
target_link_libraries(LuminoEngine ${LN_LINK_LIBS} ${LN_DEPENDENCIES_LIBRARIES})
ln_add_pch(LuminoEngine "LuminoEngine.PCH.h" "Source/LuminoEngine/Source/LuminoEngine.PCH.cpp")

# Foundation
add_library(LuminoFoundation STATIC ${LN_SOURCES_Foundation})
set_target_properties(LuminoFoundation PROPERTIES OUTPUT_NAME "LuminoFoundation${LN_LIB_POSTFIX}")
set_target_properties(LuminoFoundation PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${LN_LIB_OUTPUT_DIR}")

#------------------------------
# Unit test.
add_subdirectory(Source/LuminoEngine/Test)

#------------------------------
# Demo.
if (LN_DxLib_Demo)
	add_subdirectory(demo/DxLib)
endif()

#------------------------------
#
add_subdirectory(bindings/Runtime)
#add_subdirectory(bindings/C/Test)
if (MSVC)
	#add_subdirectory(tools/ArchiveMaker)
endif()

if (NOT LN_USE_UNICODE_CHAR_SET)
	#add_subdirectory(tools/LNSL)
	add_subdirectory(tools/LWIG)
endif()

