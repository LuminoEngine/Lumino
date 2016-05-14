# e.g.) $ cmake -DLN_EXCEPTION_BACKTRACE=OFF ..


cmake_minimum_required (VERSION 2.8.7)
project (Lumino)

set(LN_STATIC_LIB_OUTPUT_NAME "Lumino")
set(LN_LINK_LIBS "")
set(LN_UNIT_TEST_LINK_LIBS "")
set(LN_LIB_POSTFIX "")

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
option(LN_BUILD_FOUNDATION_MODULE "Build 'Foundation' module." ON)
option(LN_BUILD_C_API "Build 'C API' module." ON)
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
	include/LuminoEngine.h
	include/LuminoAudio.h
	include/LuminoDxLib.h
	include/LuminoGraphics.h
	include/LuminoScene.h
)

set(LN_SOURCES_Animation
	include/Lumino/Animation/AnimatableObject.h
	include/Lumino/Animation/AnimationClock.h
	include/Lumino/Animation/AnimationCurve.h
	include/Lumino/Animation/AnimationUtilities.h
	include/Lumino/Animation/Common.h
	include/Lumino/Animation/EasingFunctions.h
	include/Lumino/Animation/EasingValue.h
	src/Animation/AnimatableObject.cpp
	src/Animation/AnimationClip.cpp
	src/Animation/AnimationClip.h
	src/Animation/AnimationClock.cpp
	src/Animation/AnimationCurve.cpp
	src/Animation/AnimationManager.cpp
	src/Animation/AnimationManager.h
	src/Animation/AnimationState.cpp
	src/Animation/AnimationState.h
	src/Animation/Animator.cpp
	src/Animation/Animator.h
	src/Animation/Internal.h
)

set(LN_SOURCES_IO
	include/Lumino/IO/FileManager.h
	include/Lumino/IO/ASyncIOObject.h
	src/IO/ArchiveMaker.cpp
	src/IO/ArchiveMaker.h
	src/IO/ASyncIOObject.cpp
	src/IO/FileManager.cpp
	src/IO/Archive.cpp
	src/IO/Archive.h
	external/camellia/camellia.cpp
	external/camellia/camellia.h
)

set(LN_SOURCES_Platform
	include/Lumino/Platform/Clipboard.h
	include/Lumino/Platform/Common.h
	include/Lumino/Platform/EventListener.h
	include/Lumino/Platform/Ime.h
	include/Lumino/Platform/PlatformEventArgs.h
	include/Lumino/Platform/PlatformSupport.h
	include/Lumino/Platform/PlatformWindow.h
	src/Platform/Internal.h
	src/Platform/MouseCursorVisibility.h
	src/Platform/PlatformManager.cpp
	src/Platform/PlatformManager.h
	src/Platform/PlatformWindowManagerBase.cpp
	src/Platform/PlatformWindowManagerBase.h
	src/Platform/PlatformWindow.cpp
)

set(LN_SOURCES_Input
	include/Lumino/Input/Common.h
	include/Lumino/Input/Input.h
	include/Lumino/Input/InputBinding.h
	include/Lumino/Input/InputController.h
	src/Input/Input.cpp
	src/Input/InputBinding.cpp
	src/Input/InputDriver.cpp
	src/Input/InputDriver.h
	src/Input/InputManager.cpp
	src/Input/InputManager.h
	src/Input/Internal.h
	src/Input/InputController.cpp
)

if (LN_BUILD_AUDIO_MODULE)
set(LN_SOURCES_Audio
	include/LuminoAudio.h
	include/Lumino/Audio/Common.h
	include/Lumino/Audio/Detail.h
	include/Lumino/Audio/GameAudio.h
	include/Lumino/Audio/Sound.h
	include/Lumino/Audio/SoundListener.h
	src/Audio/AudioDevice.cpp
	src/Audio/AudioDevice.h
	src/Audio/AudioManager.cpp
	src/Audio/AudioManager.h
	src/Audio/AudioPlayer.cpp
	src/Audio/AudioPlayer.h
	src/Audio/AudioStream.cpp
	src/Audio/AudioStream.h
	src/Audio/AudioUtils.cpp
	src/Audio/AudioUtils.h
	src/Audio/GameAudio.cpp
	src/Audio/GameAudioImpl.cpp
	src/Audio/GameAudioImpl.h
	src/Audio/Internal.h
	src/Audio/MidiDecoder.cpp
	src/Audio/MidiDecoder.h
	src/Audio/NullAudioDevice.cpp
	src/Audio/NullAudioDevice.h
	src/Audio/OggDecoder.cpp
	src/Audio/OggDecoder.h
	src/Audio/Sound.cpp
	src/Audio/SoundListener.cpp
	src/Audio/WaveDecoder.cpp
	src/Audio/WaveDecoder.h
)
endif()

set(LN_SOURCES_Physics
	include/Lumino/Physics/BodyBase.h
	include/Lumino/Physics/Collider.h
	include/Lumino/Physics/Common.h
	include/Lumino/Physics/IDebugRenderer.h
	include/Lumino/Physics/Joint.h
	include/Lumino/Physics/PhysicsManager.h
	include/Lumino/Physics/RigidBody.h
	src/Physics/BodyBase.cpp
	src/Physics/BulletUtils.h
	src/Physics/Collider.cpp
	src/Physics/Internal.h
	src/Physics/Joint.cpp
	src/Physics/PhysicsManager.cpp
	src/Physics/RigidBody.cpp
)

set(LN_SOURCES_Graphics
	include/Lumino/Base/Point.h
	include/Lumino/Base/Rect.h
	include/Lumino/Base/Size.h
	include/Lumino/Base/Thickness.h
	include/Lumino/Graphics/Bitmap.h
	include/Lumino/Graphics/BitmapPainter.h
	include/Lumino/Graphics/Brush.h
	include/Lumino/Graphics/Color.h
	include/Lumino/Graphics/Common.h
	include/Lumino/Graphics/ContextInterface.h
	include/Lumino/Graphics/Graphics.h
	include/Lumino/Graphics/GraphicsContext.h
	include/Lumino/Graphics/GraphicsException.h
	include/Lumino/Graphics/GraphicsResourceObject.h
	include/Lumino/Graphics/IndexBuffer.h
	include/Lumino/Graphics/Material.h
	include/Lumino/Graphics/Renderer.h
	include/Lumino/Graphics/RenderingContext.h
	include/Lumino/Graphics/RenderState.h
	include/Lumino/Graphics/SamplerState.h
	include/Lumino/Graphics/Shader.h
	include/Lumino/Graphics/SpriteRenderer.h
	include/Lumino/Graphics/SwapChain.h
	include/Lumino/Graphics/Texture.h
	include/Lumino/Graphics/Utils.h
	include/Lumino/Graphics/VertexBuffer.h
	include/Lumino/Graphics/Viewport.h
	include/Lumino/Graphics/ImageEffect/ImageEffect.h
	include/Lumino/Graphics/ImageEffect/BlurImageEffect.h
	include/Lumino/Graphics/ImageEffect/ScreenMotionBlurImageEffect.h
	include/Lumino/Graphics/ImageEffect/ToneImageEffect.h
	external/GL/glew.c
	src/Base/Point.cpp
	src/Base/Rect.cpp
	src/Base/Size.cpp
	src/Base/Thickness.cpp
	src/Graphics/Bitmap.cpp
	src/Graphics/BitmapPainter.cpp
	src/Graphics/Brush.cpp
	src/Graphics/Color.cpp
	src/Graphics/ContextInterface.cpp
	src/Graphics/GeometryRenderer.cpp
	src/Graphics/GeometryRenderer.h
	src/Graphics/Graphics.cpp
	src/Graphics/GraphicsContext.cpp
	src/Graphics/GraphicsException.cpp
	src/Graphics/GraphicsHelper.h
	src/Graphics/GraphicsManager.cpp
	src/Graphics/GraphicsManager.h
	src/Graphics/IndexBuffer.cpp
	src/Graphics/Internal.h
	src/Graphics/PainterEngine.cpp
	src/Graphics/PainterEngine.h
	src/Graphics/PngFile.h
	src/Graphics/PrimitiveRenderer.cpp
	src/Graphics/PrimitiveRenderer.h
	src/Graphics/ProfilerRenderer.cpp
	src/Graphics/ProfilerRenderer.h
	src/Graphics/Renderer.cpp
	src/Graphics/RendererImpl.h
	src/Graphics/RenderingCommand.cpp
	src/Graphics/RenderingCommand.h
	src/Graphics/RenderingContext.cpp
	src/Graphics/RenderingThread.cpp
	src/Graphics/RenderingThread.h
	src/Graphics/RenderState.cpp
	src/Graphics/RenderTargetTextureCache.cpp
	src/Graphics/RenderTargetTextureCache.h
	src/Graphics/SamplerState.cpp
	src/Graphics/Shader.cpp
	src/Graphics/SpriteRenderer.cpp
	src/Graphics/SpriteRendererImpl.h
	src/Graphics/SwapChain.cpp
	src/Graphics/Texture.cpp
	src/Graphics/Utils.cpp
	src/Graphics/VertexBuffer.cpp
	src/Graphics/Viewport.cpp
	src/Graphics/ImageEffect/ImageEffect.cpp
	src/Graphics/ImageEffect/BlurImageEffect.cpp
	src/Graphics/ImageEffect/ScreenMotionBlurImageEffect.cpp
	src/Graphics/ImageEffect/ToneImageEffect.cpp
)
set(LN_SOURCES_Graphics_Device
	src/Graphics/Device/GraphicsDriverInterface.cpp
	src/Graphics/Device/GraphicsDriverInterface.h
	src/Graphics/Device/GraphicsDeviceBase.cpp
	src/Graphics/Device/GraphicsDeviceBase.h
	src/Graphics/Device/ShaderVariableBase.cpp
	src/Graphics/Device/ShaderVariableBase.h
	src/Graphics/Device/OpenGL/GLContext.cpp
	src/Graphics/Device/OpenGL/GLContext.h
	src/Graphics/Device/OpenGL/GLGraphicsDevice.cpp
	src/Graphics/Device/OpenGL/GLGraphicsDevice.h
	src/Graphics/Device/OpenGL/GLIndexBuffer.cpp
	src/Graphics/Device/OpenGL/GLIndexBuffer.h
	src/Graphics/Device/OpenGL/GLRenderer.cpp
	src/Graphics/Device/OpenGL/GLRenderer.h
	src/Graphics/Device/OpenGL/GLShader.cpp
	src/Graphics/Device/OpenGL/GLShader.h
	src/Graphics/Device/OpenGL/GLSwapChain.cpp
	src/Graphics/Device/OpenGL/GLSwapChain.h
	src/Graphics/Device/OpenGL/GLTexture.cpp
	src/Graphics/Device/OpenGL/GLTexture.h
	src/Graphics/Device/OpenGL/GLVertexBuffer.cpp
	src/Graphics/Device/OpenGL/GLVertexBuffer.h
)
set(LN_SOURCES_Graphics_Text
	include/Lumino/Graphics/Text/Font.h
	include/Lumino/Graphics/Text/GlyphRun.h
	src/Graphics/Text/BitmapFont.cpp
	src/Graphics/Text/BitmapFont.h
	src/Graphics/Text/BitmapTextRenderer.cpp
	src/Graphics/Text/BitmapTextRenderer.h
	src/Graphics/Text/Font.cpp
	src/Graphics/Text/FontGlyphTextureCache.cpp
	src/Graphics/Text/FontGlyphTextureCache.h
	src/Graphics/Text/FontManager.cpp
	src/Graphics/Text/FontManager.h
	src/Graphics/Text/FreeTypeFont.cpp
	src/Graphics/Text/FreeTypeFont.h
	src/Graphics/Text/GlyphRun.cpp
	src/Graphics/Text/TextLayoutEngine.cpp
	src/Graphics/Text/TextLayoutEngine.h
	src/Graphics/Text/TextRenderer.cpp
	src/Graphics/Text/TextRenderer.h
)

set(LN_SOURCES_Effect
	include/Lumino/Effect/Common.h
	include/Lumino/Effect/VisualEffect.h
	include/Lumino/Effect/VisualEffectInstance.h
	src/Effect/Effect.cpp
	src/Effect/EffectDriver.h
	src/Effect/EffectDriver.cpp
	src/Effect/EffectManager.h
	src/Effect/EffectManager.cpp
	src/Effect/Internal.h
)

if (LN_BUILD_SCENE_MODULE)
set(LN_SOURCES_TileMap
	include/Lumino/TileMap/Common.h
	include/Lumino/TileMap/TileLayer.h
	include/Lumino/TileMap/TileMapModel.h
	include/Lumino/TileMap/TileMapRenderer.h
	include/Lumino/TileMap/TileSet.h
	src/TileMap/TileLayer.cpp
	src/TileMap/TileMapModel.cpp
	src/TileMap/TileMapRenderer.cpp
	src/TileMap/TileSet.cpp
)

set(LN_SOURCES_Modeling
	src/Modeling/Common.h
	src/Modeling/Model.cpp
	src/Modeling/Model.h
	src/Modeling/ModelBone.cpp
	src/Modeling/ModelBone.h
	src/Modeling/ModelCore.cpp
	src/Modeling/ModelCore.h
	src/Modeling/ModelManager.cpp
	src/Modeling/ModelManager.h
)

set(LN_SOURCES_Scene
	include/Lumino/Scene/Camera.h
	include/Lumino/Scene/Common.h
	include/Lumino/Scene/Detail.h
	include/Lumino/Scene/EffectNode.h
	include/Lumino/Scene/GpuParticle.h
	include/Lumino/Scene/Grid.h
	include/Lumino/Scene/Layer.h
	include/Lumino/Scene/Light.h
	include/Lumino/Scene/Material.h
	include/Lumino/Scene/MeshModelObject.h
	include/Lumino/Scene/Material.h
	include/Lumino/Scene/MMDSceneGraph.h
	include/Lumino/Scene/SceneGraph.h
	include/Lumino/Scene/SceneGraphRenderingContext.h
	include/Lumino/Scene/SceneNode.h
	include/Lumino/Scene/Sprite2D.h
	include/Lumino/Scene/Sprite3D.h
	include/Lumino/Scene/SpriteBase.h
	include/Lumino/Scene/SpriteModelObject.h
	include/Lumino/Scene/SpriteParticle.h
	include/Lumino/Scene/TileMapNode.h
	include/Lumino/Scene/ViewPane.h
	include/Lumino/Scene/VisualNode.h
	include/Lumino/Scene/MME/MMESceneObject.h
	src/Scene/Camera.cpp
	src/Scene/Internal.h
	src/Scene/DebugRenderer.h
	src/Scene/EffectBatchRendererNode.cpp
	src/Scene/EffectBatchRendererNode.h
	src/Scene/EffectNode.cpp
	src/Scene/GpuParticle.cpp
	src/Scene/Grid.cpp
	src/Scene/InfomationRenderingPass.cpp
	src/Scene/InfomationRenderingPass.h
	src/Scene/Layer.cpp
	src/Scene/Light.cpp
	src/Scene/Material.cpp
	src/Scene/MeshModelObject.cpp
	src/Scene/MMDSceneGraph.cpp
	src/Scene/RenderingPass.cpp
	src/Scene/RenderingPass.h
	src/Scene/SceneGraph.cpp
	src/Scene/SceneGraphManager.cpp
	src/Scene/SceneGraphManager.h
	src/Scene/SceneGraphRenderingContext.cpp
	src/Scene/SceneHelper.h
	src/Scene/SceneNode.cpp
	src/Scene/Sprite2D.cpp
	src/Scene/Sprite3D.cpp
	src/Scene/SpriteBase.cpp
	src/Scene/SpriteModelObject.cpp
	src/Scene/SpriteParticle.cpp
	src/Scene/TileMapNode.cpp
	src/Scene/ViewPane.cpp
	src/Scene/ViewPane.h
	src/Scene/VisualNode.cpp
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
	src/Scene/MME/MmdMaterial.cpp
	src/Scene/MME/MmdMaterial.h
	src/Scene/MME/MMERenderingPass.cpp
	src/Scene/MME/MMERenderingPass.h
	src/Scene/MME/MMESceneShaderManager.cpp
	src/Scene/MME/MMESceneShaderManager.h
	src/Scene/MME/ShaderScriptCommandList.cpp
	src/Scene/MME/ShaderScriptCommandList.h
	src/Scene/MME/MMEShader.cpp
	src/Scene/MME/MMEShader.h
	src/Scene/MME/MMEShaderBuilder.cpp
	src/Scene/MME/MMEShaderBuilder.h
	src/Scene/MME/MMEShaderErrorInfo.cpp
	src/Scene/MME/MMEShaderErrorInfo.h
	src/Scene/MME/MMEShaderTechnique.cpp
	src/Scene/MME/MMEShaderTechnique.h
	src/Scene/MME/MMETypes.h
)
endif()

set(LN_SOURCES_UI
	include/Lumino/UI/Common.h
	include/Lumino/UI/Detail.h
	include/Lumino/UI/UIButton.h
	include/Lumino/UI/UIContentControl.h
	include/Lumino/UI/UIContext.h
	include/Lumino/UI/UIControl.h
	include/Lumino/UI/UIElement.h
	include/Lumino/UI/UIElementCollection.h
	include/Lumino/UI/UIEvent.h
	include/Lumino/UI/UIEventArgs.h
	include/Lumino/UI/UIFrameWindow.h
	include/Lumino/UI/UIInjectedInputReceiver.h
	include/Lumino/UI/UIItemsControl.h
	include/Lumino/UI/UILayoutRoot.h
	include/Lumino/UI/UILayoutView.h
	include/Lumino/UI/UIPanel.h
	include/Lumino/UI/UIStyle.h
	include/Lumino/UI/UITextBlock.h
	include/Lumino/UI/UITextElement.h
	include/Lumino/UI/UITypeInfo.h
	include/Lumino/UI/UIWindow.h
	src/UI/EventArgsPool.h
	src/UI/Internal.h
	src/UI/UIButton.cpp
	src/UI/UIContentControl.cpp
	src/UI/UIContext.cpp
	src/UI/UIControl.cpp
	src/UI/UIElement.cpp
	src/UI/UIElementCollection.cpp
	src/UI/UIEvent.cpp
	src/UI/UIEventArgs.cpp
	src/UI/UIFrameWindow.cpp
	src/UI/UIHelper.h
	src/UI/UIItemsControl.cpp
	src/UI/UILayoutRoot.cpp
	src/UI/UILayoutView.cpp
	src/UI/UIManager.cpp
	src/UI/UIManager.h
	src/UI/UIPanel.cpp
	src/UI/UIStyle.cpp
	src/UI/UITextBlock.cpp
	src/UI/UITextElement.cpp
	src/UI/UITypeInfo.cpp
	src/UI/UIWindow.cpp
)

set(LN_SOURCES_Documents
	include/Lumino/Documents/Bold.h
	include/Lumino/Documents/Common.h
	include/Lumino/Documents/DocumentsManager.h
	include/Lumino/Documents/DocumentsRenderer.h
	include/Lumino/Documents/Inline.h
	include/Lumino/Documents/Paragraph.h
	include/Lumino/Documents/Run.h
	include/Lumino/Documents/Span.h
	include/Lumino/Documents/TextElement.h
	src/Documents/Bold.cpp
	src/Documents/DocumentsHelper.h
	src/Documents/DocumentsManager.cpp
	src/Documents/DocumentsRenderer.cpp
	src/Documents/Inline.cpp
	src/Documents/Paragraph.cpp
	src/Documents/Run.cpp
	src/Documents/Span.cpp
	src/Documents/TextElement.cpp
)

set(LN_SOURCES_Game
	include/Lumino/Game/FpsController.h
	src/Game/FpsController.cpp
)

set(LN_SOURCES_Engine
	include/Lumino/Assets.h
	include/Lumino/BindingSupport.h
	include/Lumino/Common.h
	include/Lumino/Engine.h
	include/Lumino/EngineDiag.h
	include/Lumino/EngineSettings.h
	include/Lumino/GameTime.h
	include/Lumino/MultiThreadingObjectList.h
	include/Lumino/Profiler.h
	include/Lumino/RoutedEvent.h
	include/Lumino/Version.h
	src/AssetsManager.cpp
	src/AssetsManager.h
	src/Engine.cpp
	src/EngineDiagCore.cpp
	src/EngineDiagCore.h
	src/EngineDiagRenderer.h
	src/EngineManager.cpp
	src/EngineManager.h
	src/GameTime.cpp
	src/Internal.h
	src/Profiler.cpp
	src/Property.cpp
	src/RoutedEvent.cpp
	src/Version.cpp
)

if (LN_BUILD_FOUNDATION_MODULE)
set(LN_SOURCES_Foundation
	include/Lumino/Foundation/Application.h
	include/Lumino/Foundation/GameScene.h
	src/Foundation/Application.cpp
	src/Foundation/EntryPoint.cpp
	src/Foundation/EntryPoint.h
	src/Foundation/GameSceneManager.cpp
	src/Foundation/GameSceneManager.h
	src/Foundation/Main_Win32.cpp
)
endif()

#--------------------------------------
# External linker source files.
if (LN_USE_EXTERNAL_Effekseer)
	set(LN_SOURCES_Effect
		${LN_SOURCES_Effect}
		src/Effect/Effekseer/EffekseerDriver.h
		src/Effect/Effekseer/EffekseerDriver.cpp
	)
endif()

#--------------------------------------
# Platform source files.
if (WIN32)
	set(LN_SOURCES_Platform
		${LN_SOURCES_Platform}
		include/Lumino/Platform/Win32/Win32PlatformWindow.h
		src/Platform/Win32/Clipboard_Win32.cpp
		src/Platform/Win32/Ime_Win32.cpp
		src/Platform/Win32/PlatformSupport_Win32.cpp
		src/Platform/Win32/Win32PlatformWindow.cpp
		src/Platform/Win32/Win32PlatformWindowManager.cpp
		src/Platform/Win32/Win32PlatformWindowManager.h
	)
	set(LN_SOURCES_Input
		${LN_SOURCES_Input}
		src/Input/Win32InputDriver.cpp
		src/Input/Win32InputDriver.h
		src/Input/Win32JoystickDriver.cpp
		src/Input/Win32JoystickDriver.h
	)
 	set(LN_SOURCES_Graphics_Device
 		${LN_SOURCES_Graphics_Device}
		src/Graphics/Device/DirectX9/DX9Texture.h
		src/Graphics/Device/DirectX9/DX9VertexBuffer.h
		src/Graphics/Device/DirectX9/DX9GraphicsDevice.h
		src/Graphics/Device/DirectX9/DX9IndexBuffer.h
		src/Graphics/Device/DirectX9/DX9Module.h
		src/Graphics/Device/DirectX9/DX9Renderer.h
		src/Graphics/Device/DirectX9/DX9Shader.h
		src/Graphics/Device/DirectX9/DX9SwapChain.h
	 	src/Graphics/Device/DirectX9/DX9Texture.cpp
		src/Graphics/Device/DirectX9/DX9VertexBuffer.cpp
		src/Graphics/Device/DirectX9/DX9GraphicsDevice.cpp
		src/Graphics/Device/DirectX9/DX9IndexBuffer.cpp
		src/Graphics/Device/DirectX9/DX9Module.cpp
		src/Graphics/Device/DirectX9/DX9Renderer.cpp
		src/Graphics/Device/DirectX9/DX9Shader.cpp
		src/Graphics/Device/DirectX9/DX9SwapChain.cpp
		src/Graphics/Device/OpenGL/WGLGraphicsDevice.cpp
		src/Graphics/Device/OpenGL/WGLGraphicsDevice.h
	)
	set(LN_SOURCES_Modeling
 		${LN_SOURCES_Modeling}
		src/Modeling/XFileLoader.cpp
		src/Modeling/XFileLoader.h
	)
	set(LN_SOURCES_Audio
 		${LN_SOURCES_Audio}
		src/Audio/Mp3Decoder.cpp
		src/Audio/Mp3Decoder.h
		src/Audio/XAudio2/XAudio2AudioDevice.cpp
		src/Audio/XAudio2/XAudio2AudioDevice.h
		src/Audio/XAudio2/XAudio2AudioPlayer.cpp
		src/Audio/XAudio2/XAudio2AudioPlayer.h
		src/Audio/XAudio2/XAudio2Module.cpp
		src/Audio/XAudio2/XAudio2Module.h
		src/Audio/DirectMusic/DirectMusic.cpp
		src/Audio/DirectMusic/DirectMusic.h
		src/Audio/DirectMusic/DirectMusicAudioDevice.cpp
		src/Audio/DirectMusic/DirectMusicAudioDevice.h
		src/Audio/DirectMusic/DirectMusicAudioPlayer.cpp
		src/Audio/DirectMusic/DirectMusicAudioPlayer.h
	)

elseif (APPLE)
	set(LN_SOURCES_Platform
		${LN_SOURCES_Platform}
		src/Platform/Cocoa/CocoaPlatformWindow.mm
		src/Platform/Cocoa/CocoaPlatformWindow.h
		src/Platform/Cocoa/CocoaPlatformWindowManager.mm
		src/Platform/Cocoa/CocoaPlatformWindowManager.h
	)
	set(LN_SOURCES_Input
		${LN_SOURCES_Input}
		src/Input/CocoaInputDriver.cpp
		src/Input/CocoaInputDriver.h
	)
	set(LN_SOURCES_Graphics_Device
		${LN_SOURCES_Graphics_Device}
		src/Graphics/Device/OpenGL/NSGLGraphicsDevice.mm
		src/Graphics/Device/OpenGL/NSGLGraphicsDevice.h
	)

elseif (UNIX)
	set(LN_SOURCES_Platform
		${LN_SOURCES_Platform}
		src/Platform/X11/X11WindowManager.cpp
		src/Platform/X11/X11Window.cpp
		src/Platform/X11/PlatformSupport_X11.cp
	)

	set(LN_SOURCES_Graphics
		${LN_SOURCES_Graphics}
		src/Graphics/Device/OpenGL/GLXGraphicsDevice.cpp
		src/Graphics/Device/OpenGL/GLXSwapChain.cpp
	)

	set(LN_SOURCES_Audio
		${LN_SOURCES_Audio}
		src/Audio/XAudio2/XAudio2AudioDevice.cpp
		src/Audio/XAudio2/XAudio2AudioDevice.h
		src/Audio/XAudio2/XAudio2AudioPlayer.cpp
		src/Audio/XAudio2/XAudio2AudioPlayer.h
		src/Audio/DirectMusic/DirectMusic.cpp
		src/Audio/DirectMusic/DirectMusic.h
		src/Audio/DirectMusic/DirectMusicAudioDevice.cpp
		src/Audio/DirectMusic/DirectMusicAudioDevice.h
		src/Audio/DirectMusic/DirectMusicAudioPlayer.cpp
		src/Audio/DirectMusic/DirectMusicAudioPlayer.h
	)

	add_definitions(-DGLEW_NO_GLU)
	#add_definitions(-DGLEW_MX)
	add_definitions(-DGLEW_BUILD)
	#add_definitions(-DGLEW_STATIC)
	add_definitions("-std=c++11")
endif()

#--------------------------------------
# Grouping.
source_group("Animation"		FILES ${LN_SOURCES_Animation})
source_group("IO"				FILES ${LN_SOURCES_IO})
source_group("Platform"			FILES ${LN_SOURCES_Platform})
source_group("Input"			FILES ${LN_SOURCES_Input})
source_group("Audio"			FILES ${LN_SOURCES_Audio})
source_group("Physics"			FILES ${LN_SOURCES_Physics})
source_group("Graphics"			FILES ${LN_SOURCES_Graphics})
source_group("Graphics_Device"	FILES ${LN_SOURCES_Graphics_Device})
source_group("Graphics_Text"	FILES ${LN_SOURCES_Graphics_Text})
source_group("Effect"			FILES ${LN_SOURCES_Effect})
source_group("Tilemap"			FILES ${LN_SOURCES_TileMap})
source_group("Modeling"			FILES ${LN_SOURCES_Modeling})
source_group("Scene"			FILES ${LN_SOURCES_Scene})
source_group("Scene_MMD"		FILES ${LN_SOURCES_Scene_MMD})
source_group("UI"				FILES ${LN_SOURCES_UI})
source_group("Documents"		FILES ${LN_SOURCES_Documents})
source_group("Game"				FILES ${LN_SOURCES_Game})
source_group("Engine"			FILES ${LN_SOURCES_Engine})
source_group("Foundation"		FILES ${LN_SOURCES_Foundation})

set(LN_SOURCES
	${LN_SOURCES}
	${LN_SOURCES_Animation}
	${LN_SOURCES_IO}
	${LN_SOURCES_Platform}
	${LN_SOURCES_Input}
	${LN_SOURCES_Audio}
	${LN_SOURCES_Physics}
	${LN_SOURCES_Graphics}
	${LN_SOURCES_Graphics_Device}
	${LN_SOURCES_Graphics_Text}
	${LN_SOURCES_Effect}
	${LN_SOURCES_TileMap}
	${LN_SOURCES_Modeling}
	${LN_SOURCES_Scene}
	${LN_SOURCES_Scene_MMD}
	${LN_SOURCES_UI}
	#${LN_SOURCES_Documents}
	${LN_SOURCES_Game}
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
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/external
	${PROJECT_SOURCE_DIR}/external/Lumino.Core/include
	${PROJECT_SOURCE_DIR}/external/Lumino.Math/include
	${PROJECT_SOURCE_DIR}/external/libpng
	${PROJECT_SOURCE_DIR}/external/bullet-2.82
	${PROJECT_SOURCE_DIR}/external/freetype-2.6/include
	${PROJECT_SOURCE_DIR}/external/libogg/include
	${PROJECT_SOURCE_DIR}/external/glfw-3.1.2/include
	${PROJECT_SOURCE_DIR}/external/EffekseerRuntime101/src/Effekseer
)
include_directories(${LN_INCLUDE_DIRECTORIES} ${LN_PLATFORM_INCLUDE_DIRS})


#------------------------------------------------------------------------------
# External
#------------------------------------------------------------------------------

add_subdirectory(external/Lumino.Core)
set_target_properties(LuminoCore_Static PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
link_libraries(LuminoCore_Static)

add_subdirectory(external/Lumino.Math)
set_target_properties(LuminoMath_Static PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
link_libraries(LuminoMath_Static)

# Make postfix.
ln_make_postfix(LN_LIB_POSTFIX)

add_subdirectory(external/zlib)
set_target_properties(zlib PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
link_libraries(zlib)

add_subdirectory(external/libpng)
set_target_properties(libpng PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
link_libraries(libpng)

add_subdirectory(external/freetype-2.6)
set_target_properties(freetype PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
link_libraries(freetype)

# Audio
if (LN_BUILD_AUDIO_MODULE)
	add_subdirectory(external/libogg)
	set_target_properties(libogg PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
	link_libraries(libogg)
	
	add_subdirectory(external/libvorbis)
	set_target_properties(libvorbis PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
	link_libraries(libvorbis)
endif()

# Scene
if (LN_BUILD_SCENE_MODULE)
	add_subdirectory(external/bullet-2.82)
	set_target_properties(bullet PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
	link_libraries(bullet)
endif()


# Effekseer
if (LN_MSVC_LINK_MULTI_THREAD_STATIC_RUNTIME)
	set(USE_MSVC_RUNTIME_LIBRARY_DLL OFF)
else()
	set(USE_MSVC_RUNTIME_LIBRARY_DLL ON)
endif()
set(USE_INTERNAL_LOADER ON)
set(USE_GLEW_STATIC OFF)
set(USE_GLEW_DLL ON)
set(USE_OPENAL OFF)
set(USE_OPENGLES2 OFF)
set(USE_OPENGLES3 OFF)
set(USE_OPENGL3 OFF)
#mark_as_advanced(USE_OPENGLES3)


if (LN_USE_EXTERNAL_Effekseer)
	add_subdirectory(external/EffekseerRuntime101/src)
	link_libraries(effekseer)
	link_libraries(effekseer_renderer_gl)
	if (MSVC)
		link_libraries(effekseer_renderer_dx9)
		link_libraries(effekseer_sound_xaudio2)
	endif()
endif()

set_target_properties(effekseer PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set_target_properties(effekseer_renderer_gl PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set_target_properties(effekseer_renderer_dx9 PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set_target_properties(effekseer_sound_xaudio2 PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")


#------------------------------------------------------------------------------
# Output
#------------------------------------------------------------------------------

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
	#list(APPEND my_definitions "UNICODE")
	#list(APPEND my_definitions "_UNICODE")
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


# Lumino_static
add_library(${PROJECT_NAME}_Static STATIC ${LN_SOURCES})
set_target_properties(${PROJECT_NAME}_Static PROPERTIES COMPILE_DEFINITIONS "${my_definitions}")
set_target_properties(${PROJECT_NAME}_Static PROPERTIES OUTPUT_NAME "${LN_STATIC_LIB_OUTPUT_NAME}${LN_LIB_POSTFIX}")
set_target_properties(${PROJECT_NAME}_Static PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
target_link_libraries(${PROJECT_NAME}_Static ${LN_LINK_LIBS})

# Foundation
add_library(LuminoFoundation STATIC ${LN_SOURCES_Foundation})
set_target_properties(LuminoFoundation PROPERTIES COMPILE_DEFINITIONS "${my_definitions}")
set_target_properties(LuminoFoundation PROPERTIES OUTPUT_NAME "LuminoFoundation${LN_LIB_POSTFIX}")
set_target_properties(LuminoFoundation PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
#target_link_libraries(${PROJECT_NAME}_Static ${LN_LINK_LIBS})

#------------------------------
# Unit test.
add_subdirectory(test)

#------------------------------
# Demo.
add_subdirectory(demo/LiveExplorer)
if (LN_DxLib_Demo)
	add_subdirectory(demo/DxLib)
endif()

#------------------------------
# samples.
#add_subdirectory(sample)

#------------------------------
#
if (LN_BUILD_C_API)
	add_subdirectory(bindings/C_API)
	add_subdirectory(bindings/C/Test)
	add_subdirectory(tools/ArchiveMaker)
endif()

#add_subdirectory(tools/LNSL)
