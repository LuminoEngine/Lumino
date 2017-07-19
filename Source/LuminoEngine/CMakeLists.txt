cmake_minimum_required (VERSION 3.0.0)
project (LuminoEngine)

include(${PROJECT_SOURCE_DIR}/../LuminoCore/CMake/LNCommon.cmake NO_POLICY_SCOPE)

set(LN_LINK_LIBS "")
set(LN_UNIT_TEST_LINK_LIBS "")

#------------------------------------------------------------------------------
# Options
#------------------------------------------------------------------------------



#------------------------------------------------------------------------------
# Files
#------------------------------------------------------------------------------
#------------------------------
# Source files.

set(LN_SOURCES
	Include/LuminoEngine.h
	Include/LuminoAudio.h
	Include/LuminoDxLib.h
	Include/LuminoGraphics.h
	Include/LuminoScene.h
	Source/LuminoEngine.PCH.cpp
	Source/LuminoEngine.PCH.h
)

set(LN_SOURCES_Base
	Include/Lumino/Base/Uuid.h
	Source/Base/Uuid.cpp
)

set(LN_SOURCES_Animation
	Include/Lumino/Animation/AnimationClock.h
	Include/Lumino/Animation/AnimationCurve.h
	Include/Lumino/Animation/AnimationTimeline.h
	Include/Lumino/Animation/AnimationUtilities.h
	Include/Lumino/Animation/Common.h
	Include/Lumino/Animation/EasingFunctions.h
	Include/Lumino/Animation/EasingValue.h
	Source/Animation/AnimationClip.cpp
	Source/Animation/AnimationClip.h
	Source/Animation/AnimationClock.cpp
	Source/Animation/AnimationCurve.cpp
	Source/Animation/AnimationManager.cpp
	Source/Animation/AnimationManager.h
	Source/Animation/AnimationState.cpp
	Source/Animation/AnimationState.h
	Source/Animation/AnimationTimeline.cpp
	Source/Animation/Animator.cpp
	Source/Animation/Animator.h
	Source/Animation/Detail.h
	Source/Animation/Internal.h
	Source/Animation/VmdLoader.cpp
	Source/Animation/VmdLoader.h
)

set(LN_SOURCES_IO
	Include/Lumino/IO/FileManager.h
	Include/Lumino/IO/ASyncIOObject.h
	Source/IO/ArchiveMaker.cpp
	Source/IO/ArchiveMaker.h
	Source/IO/ASyncIOObject.cpp
	Source/IO/FileManager.cpp
	Source/IO/Archive.cpp
	Source/IO/Archive.h
	../../External/camellia/camellia.cpp
	../../External/camellia/camellia.h
)

set(LN_SOURCES_Platform
	Include/Lumino/Platform/Clipboard.h
	Include/Lumino/Platform/Common.h
	Include/Lumino/Platform/EventListener.h
	Include/Lumino/Platform/Ime.h
	Include/Lumino/Platform/PlatformEventArgs.h
	Include/Lumino/Platform/PlatformFileOpenDialog.h
	Include/Lumino/Platform/PlatformSupport.h
	Include/Lumino/Platform/PlatformWindow.h
	Source/Platform/Internal.h
	Source/Platform/MouseCursorVisibility.h
	Source/Platform/PlatformFileOpenDialog.cpp
	Source/Platform/PlatformManager.cpp
	Source/Platform/PlatformManager.h
	Source/Platform/PlatformWindowManagerBase.cpp
	Source/Platform/PlatformWindowManagerBase.h
	Source/Platform/PlatformWindow.cpp
)

set(LN_SOURCES_Input
	Include/Lumino/Input/Common.h
	Include/Lumino/Input/Input.h
	Include/Lumino/Input/InputBinding.h
	Include/Lumino/Input/InputController.h
	Source/Input/Input.cpp
	Source/Input/InputBinding.cpp
	Source/Input/InputDriver.cpp
	Source/Input/InputDriver.h
	Source/Input/InputManager.cpp
	Source/Input/InputManager.h
	Source/Input/Internal.h
	Source/Input/InputController.cpp
)

set(LN_SOURCES_Audio
	Include/LuminoAudio.h
	Include/Lumino/Audio/Common.h
	Include/Lumino/Audio/Detail.h
	Include/Lumino/Audio/GameAudio.h
	Include/Lumino/Audio/Sound.h
	Include/Lumino/Audio/SoundListener.h
	Source/Audio/AudioDevice.cpp
	Source/Audio/AudioDevice.h
	Source/Audio/AudioManager.cpp
	Source/Audio/AudioManager.h
	Source/Audio/AudioPlayer.cpp
	Source/Audio/AudioPlayer.h
	Source/Audio/AudioStream.cpp
	Source/Audio/AudioStream.h
	Source/Audio/AudioUtils.cpp
	Source/Audio/AudioUtils.h
	Source/Audio/GameAudio.cpp
	Source/Audio/GameAudioImpl.cpp
	Source/Audio/GameAudioImpl.h
	Source/Audio/Internal.h
	Source/Audio/MidiDecoder.cpp
	Source/Audio/MidiDecoder.h
	Source/Audio/NullAudioDevice.cpp
	Source/Audio/NullAudioDevice.h
	Source/Audio/OggDecoder.cpp
	Source/Audio/OggDecoder.h
	Source/Audio/Sound.cpp
	Source/Audio/SoundListener.cpp
	Source/Audio/WaveDecoder.cpp
	Source/Audio/WaveDecoder.h
)

set(LN_SOURCES_Physics
	Include/Lumino/Physics/PhysicsObject.h
	Include/Lumino/Physics/CollisionBody.h
	Include/Lumino/Physics/CollisionShape.h
	Include/Lumino/Physics/Common.h
	Include/Lumino/Physics/Joint.h
	Include/Lumino/Physics/PhysicsWorld.h
	Include/Lumino/Physics/RigidBody.h
	Source/Physics/PhysicsObject.cpp
	Source/Physics/BulletUtils.h
	Source/Physics/CollisionBody.cpp
	Source/Physics/CollisionShape.cpp
	Source/Physics/Internal.h
	Source/Physics/Joint.cpp
	Source/Physics/PhysicsManager.cpp
	Source/Physics/PhysicsManager.h
	Source/Physics/PhysicsWorld.cpp
	Source/Physics/RigidBody.cpp
)

set(LN_SOURCES_Graphics
	Include/Lumino/Base/GeometryStructs.h
	Include/Lumino/Graphics/Bitmap.h
	Include/Lumino/Graphics/BitmapPainter.h
	Include/Lumino/Graphics/Brush.h
	Include/Lumino/Graphics/Color.h
	Include/Lumino/Graphics/Common.h
	Include/Lumino/Graphics/ContextInterface.h
	Include/Lumino/Graphics/FrameCapturer.h
	Include/Lumino/Graphics/Graphics.h
	Include/Lumino/Graphics/GraphicsException.h
	Include/Lumino/Graphics/GraphicsResourceObject.h
	Include/Lumino/Graphics/IndexBuffer.h
	Include/Lumino/Graphics/Material.h
	Include/Lumino/Graphics/PerlinNoise.h
	Include/Lumino/Graphics/RenderState.h
	Include/Lumino/Graphics/SamplerState.h
	Include/Lumino/Graphics/Shader.h
	Include/Lumino/Graphics/SwapChain.h
	Include/Lumino/Graphics/Texture.h
	Include/Lumino/Graphics/Utils.h
	Include/Lumino/Graphics/Vertex.h
	Include/Lumino/Graphics/VertexBuffer.h
	Include/Lumino/Graphics/VertexDeclaration.h
	Include/Lumino/Graphics/Viewport.h
	Include/Lumino/Graphics/ImageEffect/ImageEffect.h
	Include/Lumino/Graphics/ImageEffect/BlurImageEffect.h
	Include/Lumino/Graphics/ImageEffect/ScreenMotionBlurImageEffect.h
	Include/Lumino/Graphics/ImageEffect/ToneImageEffect.h
	../../External/GL/glew.c
	../../External/nanovg/nanovg.c
	../../External/nanovg/nanovg.h
	../../External/glues/source/glues.h
	../../External/glues/source/libtess/tessmono.c
	../../External/glues/source/libtess/dict.c
	../../External/glues/source/libtess/geom.c
	../../External/glues/source/libtess/memalloc.c
	../../External/glues/source/libtess/mesh.c
	../../External/glues/source/libtess/normal.c
	../../External/glues/source/libtess/priorityq.c
	../../External/glues/source/libtess/render.c
	../../External/glues/source/libtess/sweep.c
	../../External/glues/source/libtess/tess.c
	Source/Base/GeometryStructs.cpp
	Source/Graphics/Bitmap.cpp
	Source/Graphics/BitmapPainter.cpp
	Source/Graphics/Brush.cpp
	Source/Graphics/Color.cpp
	Source/Graphics/ContextInterface.cpp
	Source/Graphics/CoreGraphicsRenderFeature.cpp
	Source/Graphics/CoreGraphicsRenderFeature.h
	Source/Graphics/FrameCapturer.cpp
	Source/Graphics/Graphics.cpp
	Source/Graphics/GraphicsException.cpp
	Source/Graphics/GraphicsManager.cpp
	Source/Graphics/GraphicsManager.h
	Source/Graphics/IndexBuffer.cpp
	Source/Graphics/Internal.h
	Source/Graphics/Material.cpp
	Source/Graphics/PerlinNoise.cpp
	Source/Graphics/PngFile.h
	Source/Graphics/ProfilerRenderer.cpp
	Source/Graphics/ProfilerRenderer.h
	Source/Graphics/RenderingCommand.cpp
	Source/Graphics/RenderingCommand.h
	Source/Graphics/RenderingThread.cpp
	Source/Graphics/RenderingThread.h
	Source/Graphics/RenderState.cpp
	Source/Graphics/RenderTargetTextureCache.cpp
	Source/Graphics/RenderTargetTextureCache.h
	Source/Graphics/SamplerState.cpp
	Source/Graphics/Shader.cpp
	Source/Graphics/ShaderAnalyzer.cpp
	Source/Graphics/ShaderAnalyzer.h
	Source/Graphics/SwapChain.cpp
	Source/Graphics/Texture.cpp
	Source/Graphics/Utils.cpp
	Source/Graphics/VertexBuffer.cpp
	Source/Graphics/VertexDeclaration.cpp
	Source/Graphics/Viewport.cpp
	Source/Graphics/ImageEffect/ImageEffect.cpp
	Source/Graphics/ImageEffect/BlurImageEffect.cpp
	Source/Graphics/ImageEffect/ScreenMotionBlurImageEffect.cpp
	Source/Graphics/ImageEffect/ToneImageEffect.cpp
)
set(LN_SOURCES_Graphics_Device
	Source/Graphics/Device/GraphicsDriverInterface.cpp
	Source/Graphics/Device/GraphicsDriverInterface.h
	Source/Graphics/Device/GraphicsDeviceBase.cpp
	Source/Graphics/Device/GraphicsDeviceBase.h
	Source/Graphics/Device/ShaderVariableBase.cpp
	Source/Graphics/Device/ShaderVariableBase.h
	Source/Graphics/Device/OpenGL/GLCommon.h
	Source/Graphics/Device/OpenGL/GLGraphicsDevice.cpp
	Source/Graphics/Device/OpenGL/GLGraphicsDevice.h
	Source/Graphics/Device/OpenGL/GLIndexBuffer.cpp
	Source/Graphics/Device/OpenGL/GLIndexBuffer.h
	Source/Graphics/Device/OpenGL/GLRenderer.cpp
	Source/Graphics/Device/OpenGL/GLRenderer.h
	Source/Graphics/Device/OpenGL/GLShader.cpp
	Source/Graphics/Device/OpenGL/GLShader.h
	Source/Graphics/Device/OpenGL/GLSwapChain.cpp
	Source/Graphics/Device/OpenGL/GLSwapChain.h
	Source/Graphics/Device/OpenGL/GLTexture.cpp
	Source/Graphics/Device/OpenGL/GLTexture.h
	Source/Graphics/Device/OpenGL/GLVertexBuffer.cpp
	Source/Graphics/Device/OpenGL/GLVertexBuffer.h
)
set(LN_SOURCES_Graphics_Text
	Include/Lumino/Graphics/Text/Font.h
	Include/Lumino/Graphics/Text/GlyphRun.h
	Source/Graphics/Text/BitmapFont.cpp
	Source/Graphics/Text/BitmapFont.h
	Source/Graphics/Text/BitmapTextRenderer.cpp
	Source/Graphics/Text/BitmapTextRenderer.h
	Source/Graphics/Text/Font.cpp
	Source/Graphics/Text/FontGlyphTextureCache.cpp
	Source/Graphics/Text/FontGlyphTextureCache.h
	Source/Graphics/Text/FontManager.cpp
	Source/Graphics/Text/FontManager.h
	Source/Graphics/Text/FontOutlineTessellator.cpp
	Source/Graphics/Text/FontOutlineTessellator.h
	Source/Graphics/Text/FreeTypeFont.cpp
	Source/Graphics/Text/FreeTypeFont.h
	Source/Graphics/Text/GlyphIconFontManager.cpp
	Source/Graphics/Text/GlyphIconFontManager.h
	Source/Graphics/Text/GlyphRun.cpp
	Source/Graphics/Text/SpriteTextRenderFeature.cpp
	Source/Graphics/Text/SpriteTextRenderFeature.h
	Source/Graphics/Text/TextLayoutEngine.cpp
	Source/Graphics/Text/TextLayoutEngine.h
	Source/Graphics/Text/VectorTextRenderFeature.cpp
	Source/Graphics/Text/VectorTextRenderFeature.h
)

set(LN_SOURCES_Rendering
	Include/Lumino/Rendering/Common.h
	Include/Lumino/Rendering/DrawingContext.h
	Include/Lumino/Rendering/Rendering.h
	Include/Lumino/Rendering/RenderingContext.h
	Include/Lumino/Rendering/SceneRenderer.h
	Source/Rendering/DrawingContext.cpp
	Source/Rendering/FrameRectRenderFeature.cpp
	Source/Rendering/FrameRectRenderFeature.h
	Source/Rendering/Internal.h
	Source/Rendering/MeshRenderFeature.cpp
	Source/Rendering/MeshRenderFeature.h
	Source/Rendering/NanoVGRenderFeature.cpp
	Source/Rendering/NanoVGRenderFeature.h
	Source/Rendering/PrimitiveRenderFeature.cpp
	Source/Rendering/PrimitiveRenderFeature.h
	Source/Rendering/Rendering.cpp
	Source/Rendering/RenderingContext.cpp
	Source/Rendering/SceneRenderer.cpp
	Source/Rendering/ShapesRenderFeature.cpp
	Source/Rendering/ShapesRenderFeature.h
	Source/Rendering/SpriteRenderFeature.cpp
	Source/Rendering/SpriteRenderFeature.h
)

set(LN_SOURCES_Effect
	Include/Lumino/Effect/Common.h
	Include/Lumino/Effect/VisualEffect.h
	Include/Lumino/Effect/VisualEffectInstance.h
	Source/Effect/Effect.cpp
	Source/Effect/EffectDriver.h
	Source/Effect/EffectDriver.cpp
	Source/Effect/EffectManager.h
	Source/Effect/EffectManager.cpp
	Source/Effect/Internal.h
)

set(LN_SOURCES_TileMap
	Include/Lumino/TileMap/Common.h
	Include/Lumino/TileMap/TileLayer.h
	Include/Lumino/TileMap/TileMapModel.h
	Include/Lumino/TileMap/TileMapRenderer.h
	Include/Lumino/TileMap/TileSet.h
	Source/TileMap/TileLayer.cpp
	Source/TileMap/TileMapModel.cpp
	Source/TileMap/TileMapRenderer.cpp
	Source/TileMap/TileSet.cpp
)

set(LN_SOURCES_Mesh
	Include/Lumino/Mesh/GizmoModel.h
	Include/Lumino/Mesh/Mesh.h
	Include/Lumino/Mesh/SkinnedMeshModel.h
	Source/Mesh/Common.h
	Source/Mesh/ModelManager.cpp
	Source/Mesh/ModelManager.h
	Source/Mesh/MqoImporter.cpp
	Source/Mesh/MqoImporter.h
	Source/Mesh/PmxLoader.cpp
	Source/Mesh/PmxLoader.h
	Source/Mesh/PmxSkinnedMesh.cpp
	Source/Mesh/PmxSkinnedMesh.h
	Source/Mesh/GizmoModel.cpp
	Source/Mesh/Mesh.cpp
	Source/Mesh/MeshFactory.h
	Source/Mesh/MeshFactory.cpp
	Source/Mesh/SkinnedMeshModel.cpp
)

set(LN_SOURCES_Scene
	Include/Lumino/Scene/Camera.h
	Include/Lumino/Scene/Common.h
	Include/Lumino/Scene/Detail.h
	Include/Lumino/Scene/EffectNode.h
	Include/Lumino/Scene/GlyphIcon.h
	Include/Lumino/Scene/GpuParticle.h
	Include/Lumino/Scene/Light.h
	Include/Lumino/Scene/MeshModelObject.h
	Include/Lumino/Scene/OffscreenWorldView.h
#	Include/Lumino/Scene/MMDSceneGraph.h
	Include/Lumino/Scene/SceneGraph.h
	Include/Lumino/Scene/SceneNode.h
	Include/Lumino/Scene/SceneTR.h
	Include/Lumino/Scene/Sprite.h
	#Include/Lumino/Scene/SpriteModelObject.h
	Include/Lumino/Scene/Particle.h
	Include/Lumino/Scene/StaticMesh.h
	Include/Lumino/Scene/TextBlock.h
	Include/Lumino/Scene/TileMap.h
	Include/Lumino/Scene/TransitionPostEffect.h
	Include/Lumino/Scene/VisualNode.h
	Include/Lumino/Scene/WorldObject.h
#	Include/Lumino/Scene/MME/MMESceneObject.h
	Source/Scene/Camera.cpp
	Source/Scene/Internal.h
	Source/Scene/DebugRenderer.h
	Source/Scene/EffectBatchRendererNode.cpp
	Source/Scene/EffectBatchRendererNode.h
	Source/Scene/EffectNode.cpp
	Source/Scene/GlyphIcon.cpp
	Source/Scene/GpuParticle.cpp
	Source/Scene/Light.cpp
	Source/Scene/MeshModelObject.cpp
	Source/Scene/OffscreenWorldView.cpp
#	Source/Scene/MMDSceneGraph.cpp
	Source/Scene/SceneGraph.cpp
	Source/Scene/SceneGraphManager.cpp
	Source/Scene/SceneGraphManager.h
	Source/Scene/SceneNode.cpp
	Source/Scene/SceneTR.cpp
	Source/Scene/Sprite.cpp
	#Source/Scene/SpriteModelObject.cpp
	Source/Scene/Particle.cpp
	Source/Scene/StaticMesh.cpp
	Source/Scene/TextBlock.cpp
	Source/Scene/TileMap.cpp
	Source/Scene/TransitionPostEffect.cpp
	Source/Scene/VisualNode.cpp
	Source/Scene/WorldObject.cpp
	#../../External/ssbpLib/ssShader_frag.h
	#../../External/ssbpLib/SS5Player.cpp
	#../../External/ssbpLib/SS5Player.h
	#../../External/ssbpLib/SS5PlayerData.h
	#../../External/ssbpLib/SS5PlayerPlatform.cpp
	#../../External/ssbpLib/SS5PlayerPlatform.h
	#../../External/ssbpLib/SS5PlayerTypes.h
	#../../External/ssbpLib/common/Animator/ssplayer_PartState.h
	#../../External/ssbpLib/common/Animator/ssplayer_cellmap.h
	#../../External/ssbpLib/common/Animator/ssplayer_effect.cpp
	#../../External/ssbpLib/common/Animator/ssplayer_effect.h
	#../../External/ssbpLib/common/Animator/ssplayer_effectfunction.cpp
	#../../External/ssbpLib/common/Animator/ssplayer_effectfunction.h
	#../../External/ssbpLib/common/Animator/ssplayer_macro.h
	#../../External/ssbpLib/common/Animator/ssplayer_matrix.cpp
	#../../External/ssbpLib/common/Animator/ssplayer_matrix.h
	#../../External/ssbpLib/common/Animator/ssplayer_PartState.cpp
)
set(LN_SOURCES_Scene_MMD
	Source/Scene/MME/MmdMaterial.cpp
	Source/Scene/MME/MmdMaterial.h
	Source/Scene/MME/MMERenderingPass.cpp
	Source/Scene/MME/MMERenderingPass.h
	Source/Scene/MME/MMESceneShaderManager.cpp
	Source/Scene/MME/MMESceneShaderManager.h
	Source/Scene/MME/ShaderScriptCommandList.cpp
	Source/Scene/MME/ShaderScriptCommandList.h
	Source/Scene/MME/MMEShader.cpp
	Source/Scene/MME/MMEShader.h
	Source/Scene/MME/MMEShaderBuilder.cpp
	Source/Scene/MME/MMEShaderBuilder.h
	Source/Scene/MME/MMEShaderErrorInfo.cpp
	Source/Scene/MME/MMEShaderErrorInfo.h
	Source/Scene/MME/MMEShaderTechnique.cpp
	Source/Scene/MME/MMEShaderTechnique.h
	Source/Scene/MME/MMETypes.h
)

set(LN_SOURCES_UI
	Include/Lumino/UI/Common.h
	Include/Lumino/UI/Detail.h
	Include/Lumino/UI/LayoutElement.h
	Include/Lumino/UI/UIButton.h
	Include/Lumino/UI/UIComboBox.h
	Include/Lumino/UI/UICommands.h
	Include/Lumino/UI/UIContentControl.h
	Include/Lumino/UI/UIContext.h
	Include/Lumino/UI/UIControl.h
	Include/Lumino/UI/UIElement.h
	Include/Lumino/UI/UIElementCollection.h
	Include/Lumino/UI/UIEvent.h
	Include/Lumino/UI/UIEventArgs.h
	Include/Lumino/UI/UIFrameWindow.h
	Include/Lumino/UI/UIImage.h
	Include/Lumino/UI/UIInjectedInputReceiver.h
	Include/Lumino/UI/UIItemsControl.h
	Include/Lumino/UI/UILayoutPanel.h
	Include/Lumino/UI/UILayoutView.h
	Include/Lumino/UI/UIListBox.h
	Include/Lumino/UI/UIRangeBase.h
	Include/Lumino/UI/UIScrollViewer.h
	Include/Lumino/UI/UISlider.h
	Include/Lumino/UI/UIStyle.h
	Include/Lumino/UI/UITextBlock.h
	Include/Lumino/UI/UITextBox.h
	Include/Lumino/UI/UITextElement.h
	Include/Lumino/UI/UITreeView.h
	Include/Lumino/UI/UITypeInfo.h
	Include/Lumino/UI/UIViewport.h
	Include/Lumino/UI/UIWindow.h
	Source/UI/EventArgsPool.h
	Source/UI/Internal.h
	Source/UI/LayoutElement.cpp
	Source/UI/LayoutImpl.h
	Source/UI/UIButton.cpp
	Source/UI/UIColorsDefine.inl
	Source/UI/UIComboBox.cpp
	Source/UI/UICommands.cpp
	Source/UI/UIContentControl.cpp
	Source/UI/UIContext.cpp
	Source/UI/UIControl.cpp
	Source/UI/UIElement.cpp
	Source/UI/UIElementCollection.cpp
	Source/UI/UIEvent.cpp
	Source/UI/UIEventArgs.cpp
	Source/UI/UIFrameWindow.cpp
	Source/UI/UIHelper.h
	Source/UI/UIImage.cpp
	Source/UI/UIItemsControl.cpp
	Source/UI/UILayoutPanel.cpp
	Source/UI/UILayoutView.cpp
	Source/UI/UIListBox.cpp
	Source/UI/UIManager.cpp
	Source/UI/UIManager.h
	Source/UI/UIRangeBase.cpp
	Source/UI/UIScrollViewer.cpp
	Source/UI/UISlider.cpp
	Source/UI/UIStyle.cpp
	Source/UI/UITextBlock.cpp
	Source/UI/UITextBox.cpp
	Source/UI/UITextElement.cpp
	Source/UI/UITreeView.cpp
	Source/UI/UITypeInfo.cpp
	Source/UI/UIViewport.cpp
	Source/UI/UIWindow.cpp
)

set(LN_SOURCES_Documents
	Include/Lumino/Documents/Common.h
	Include/Lumino/Documents/Documents.h
	Source/Documents/DocumentElements.cpp
	Source/Documents/DocumentElements.h
	Source/Documents/Documents.cpp
	Source/Documents/DocumentsManager.cpp
	Source/Documents/DocumentsManager.h
	Source/Documents/Internal.h
)

set(LN_SOURCES_Game
	Include/Lumino/Game/Component.h
	Include/Lumino/Game/FpsController.h
	Source/Game/Component.cpp
	Source/Game/FpsController.cpp
)

if (MSVC_VERSION GREATER 1800)		# (MSVC > vs2013)
set(LN_SOURCES_Scripting
	Source/Scripting/Common.h
	Source/Scripting/NlFunctionNodes.cpp
	Source/Scripting/NlFunctionNodes.h
	Source/Scripting/NlVariant.cpp
	Source/Scripting/NlVariant.h
	Source/Scripting/ScriptingManager.cpp
	Source/Scripting/ScriptingManager.h
)
endif()

set(LN_SOURCES_Engine
	Include/Lumino/Assets.h
	Include/Lumino/BindingSupport.h
	Include/Lumino/Common.h
	Include/Lumino/Engine.h
	Include/Lumino/EngineDiag.h
	Include/Lumino/EngineSettings.h
	Include/Lumino/GameTime.h
	Include/Lumino/MultiThreadingObjectList.h
	Include/Lumino/Profiler.h
	Include/Lumino/RoutedEvent.h
	Include/Lumino/Version.h
	Include/Lumino/World.h
	Source/AssetsManager.cpp
	Source/AssetsManager.h
	Source/Engine.cpp
	Source/EngineDiagCore.cpp
	Source/EngineDiagCore.h
	Source/EngineDiagViewer.h
	Source/EngineManager.cpp
	Source/EngineManager.h
	Source/GameTime.cpp
	Source/Internal.h
	Source/Profiler.cpp
	Source/Property.cpp
	Source/Resource.cpp
	Source/Resource.h
	Source/RoutedEvent.cpp
	Source/Version.cpp
	Source/World.cpp
)

set(LN_SOURCES_Framework
	Include/Lumino/Framework/Application.h
	Include/Lumino/Framework/GameScene.h
	Source/Framework/Application.cpp
	Source/Framework/GameSceneManager.cpp
	Source/Framework/GameSceneManager.h
)

#--------------------------------------
# External linker source files.
if (LN_USE_EXTERNAL_Effekseer)
	set(LN_SOURCES_Effect
		${LN_SOURCES_Effect}
		Source/Effect/Effekseer/EffekseerDriver.h
		Source/Effect/Effekseer/EffekseerDriver.cpp
	)
endif()

#--------------------------------------
# Platform source files.
if (WIN32)
	set(LN_SOURCES_Platform
		${LN_SOURCES_Platform}
		Include/Lumino/Platform/Win32/Win32PlatformWindow.h
		Source/Platform/Win32/Clipboard_Win32.cpp
		Source/Platform/Win32/Ime_Win32.cpp
		Source/Platform/Win32/PlatformSupport_Win32.cpp
		Source/Platform/Win32/Win32PlatformFileOpenDialog.h
		Source/Platform/Win32/Win32PlatformWindow.cpp
		Source/Platform/Win32/Win32PlatformWindowManager.cpp
		Source/Platform/Win32/Win32PlatformWindowManager.h
	)
	set(LN_SOURCES_Input
		${LN_SOURCES_Input}
		Source/Input/Win32InputDriver.cpp
		Source/Input/Win32InputDriver.h
		Source/Input/Win32JoystickDriver.cpp
		Source/Input/Win32JoystickDriver.h
	)
 	set(LN_SOURCES_Graphics_Device
 		${LN_SOURCES_Graphics_Device}
		Source/Graphics/Device/DirectX9/DX9Texture.h
		Source/Graphics/Device/DirectX9/DX9VertexBuffer.h
		Source/Graphics/Device/DirectX9/DX9GraphicsDevice.h
		Source/Graphics/Device/DirectX9/DX9IndexBuffer.h
		Source/Graphics/Device/DirectX9/DX9Module.h
		Source/Graphics/Device/DirectX9/DX9Renderer.h
		Source/Graphics/Device/DirectX9/DX9Shader.h
		Source/Graphics/Device/DirectX9/DX9SwapChain.h
	 	Source/Graphics/Device/DirectX9/DX9Texture.cpp
		Source/Graphics/Device/DirectX9/DX9VertexBuffer.cpp
		Source/Graphics/Device/DirectX9/DX9GraphicsDevice.cpp
		Source/Graphics/Device/DirectX9/DX9IndexBuffer.cpp
		Source/Graphics/Device/DirectX9/DX9Module.cpp
		Source/Graphics/Device/DirectX9/DX9Renderer.cpp
		Source/Graphics/Device/DirectX9/DX9Shader.cpp
		Source/Graphics/Device/DirectX9/DX9SwapChain.cpp
		Source/Graphics/Device/OpenGL/WGLGraphicsDevice.cpp
		Source/Graphics/Device/OpenGL/WGLGraphicsDevice.h
	)
	set(LN_SOURCES_Mesh
 		${LN_SOURCES_Mesh}
		Source/Mesh/XFileLoader.cpp
		Source/Mesh/XFileLoader.h
	)
	set(LN_SOURCES_Audio
 		${LN_SOURCES_Audio}
		Source/Audio/Mp3Decoder.cpp
		Source/Audio/Mp3Decoder.h
		Source/Audio/XAudio2/XAudio2AudioDevice.cpp
		Source/Audio/XAudio2/XAudio2AudioDevice.h
		Source/Audio/XAudio2/XAudio2AudioPlayer.cpp
		Source/Audio/XAudio2/XAudio2AudioPlayer.h
		Source/Audio/XAudio2/XAudio2Module.cpp
		Source/Audio/XAudio2/XAudio2Module.h
		Source/Audio/DirectMusic/DirectMusic.cpp
		Source/Audio/DirectMusic/DirectMusic.h
		Source/Audio/DirectMusic/DirectMusicAudioDevice.cpp
		Source/Audio/DirectMusic/DirectMusicAudioDevice.h
		Source/Audio/DirectMusic/DirectMusicAudioPlayer.cpp
		Source/Audio/DirectMusic/DirectMusicAudioPlayer.h
	)


elseif (APPLE)
	set(LN_SOURCES_Platform
		${LN_SOURCES_Platform}
		Source/Platform/Cocoa/CocoaPlatformWindow.mm
		Source/Platform/Cocoa/CocoaPlatformWindow.h
		Source/Platform/Cocoa/CocoaPlatformWindowManager.mm
		Source/Platform/Cocoa/CocoaPlatformWindowManager.h
	)
	set(LN_SOURCES_Input
		${LN_SOURCES_Input}
		Source/Input/CocoaInputDriver.cpp
		Source/Input/CocoaInputDriver.h
	)
	set(LN_SOURCES_Graphics_Device
		${LN_SOURCES_Graphics_Device}
		Source/Graphics/Device/OpenGL/NSGLGraphicsDevice.mm
		Source/Graphics/Device/OpenGL/NSGLGraphicsDevice.h
	)

elseif (UNIX)
	set(LN_SOURCES_Platform
		${LN_SOURCES_Platform}
		Source/Platform/X11/X11WindowManager.cpp
		Source/Platform/X11/X11Window.cpp
		Source/Platform/X11/PlatformSupport_X11.cp
	)

	set(LN_SOURCES_Graphics
		${LN_SOURCES_Graphics}
		Source/Graphics/Device/OpenGL/GLXGraphicsDevice.cpp
		Source/Graphics/Device/OpenGL/GLXSwapChain.cpp
	)

	set(LN_SOURCES_Audio
		${LN_SOURCES_Audio}
		Source/Audio/XAudio2/XAudio2AudioDevice.cpp
		Source/Audio/XAudio2/XAudio2AudioDevice.h
		Source/Audio/XAudio2/XAudio2AudioPlayer.cpp
		Source/Audio/XAudio2/XAudio2AudioPlayer.h
		Source/Audio/DirectMusic/DirectMusic.cpp
		Source/Audio/DirectMusic/DirectMusic.h
		Source/Audio/DirectMusic/DirectMusicAudioDevice.cpp
		Source/Audio/DirectMusic/DirectMusicAudioDevice.h
		Source/Audio/DirectMusic/DirectMusicAudioPlayer.cpp
		Source/Audio/DirectMusic/DirectMusicAudioPlayer.h
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
source_group("Rendering"		FILES ${LN_SOURCES_Rendering})
source_group("Effect"			FILES ${LN_SOURCES_Effect})
source_group("Tilemap"			FILES ${LN_SOURCES_TileMap})
source_group("Mesh"				FILES ${LN_SOURCES_Mesh})
source_group("Scene"			FILES ${LN_SOURCES_Scene})
source_group("Scene_MMD"		FILES ${LN_SOURCES_Scene_MMD})
source_group("UI"				FILES ${LN_SOURCES_UI})
source_group("Documents"		FILES ${LN_SOURCES_Documents})
source_group("Game"				FILES ${LN_SOURCES_Game})
source_group("Scripting"		FILES ${LN_SOURCES_Scripting})
source_group("Engine"			FILES ${LN_SOURCES_Engine})
source_group("Framework"		FILES ${LN_SOURCES_Framework})

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
	${LN_SOURCES_Rendering}
	${LN_SOURCES_Effect}
	${LN_SOURCES_TileMap}
	${LN_SOURCES_Mesh}
	${LN_SOURCES_Scene}
#	${LN_SOURCES_Scene_MMD}
	${LN_SOURCES_UI}
	${LN_SOURCES_Documents}
	${LN_SOURCES_Game}
	${LN_SOURCES_Scripting}
	${LN_SOURCES_Engine}
	${LN_SOURCES_Framework}
	${LN_SOURCES_C_API}
)

#------------------------------------------------------------------------------
# Apply options
#------------------------------------------------------------------------------
find_package(OpenGL REQUIRED)

if (WIN32)
	if (LN_MSVC_STATIC_RUNTIME)
		foreach (flag CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE)
			if (${flag} MATCHES "/MD")
				string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
			endif()
			if (${flag} MATCHES "/MDd")
				string(REGEX REPLACE "/MDd" "/MTd" ${flag} "${${flag}}")
			endif()
		endforeach()
	endif()

	#set(CMAKE_DEBUG_POSTFIX "d")

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
	${PROJECT_SOURCE_DIR}/Include
	${PROJECT_SOURCE_DIR}/../LuminoCore/Include
	${PROJECT_SOURCE_DIR}/../Fluorite/Include
	../../External
	../../External/glfw-3.1.2/include
)
include_directories(${LN_INCLUDE_DIRECTORIES} ${LN_PLATFORM_INCLUDE_DIRS})


#------------------------------------------------------------------------------
# External
#------------------------------------------------------------------------------

link_libraries(LuminoCore)

# Make postfix.
#ln_make_postfix(LN_LIB_POSTFIX)


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
if (LN_USE_EXTERNAL_Effekseer)
	add_definitions(-DLN_USE_EXTERNAL_Effekseer)
endif()


# LuminoEngine
add_library(LuminoEngine STATIC ${LN_SOURCES})
ln_add_common_property(LuminoEngine)
target_include_directories(LuminoEngine PUBLIC ${LN_DEPENDENCIES_INCLUDES})
target_link_libraries(LuminoEngine Fluorite ${LN_LINK_LIBS} ${LN_DEPENDENCIES_LIBRARIES})
ln_add_pch(LuminoEngine "LuminoEngine.PCH.h" "Source/LuminoEngine.PCH.cpp")

# Unit test.
add_subdirectory(Test)

