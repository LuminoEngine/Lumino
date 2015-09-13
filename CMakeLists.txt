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

# Architecture.
#option(LN_BUILD_X64 "Architecture." OFF)

option(LN_DxLib_Demo "DX Library demo." OFF)

#------------------------------------------------------------------------------
# Files
#------------------------------------------------------------------------------
#------------------------------
# Source files.

set(LN_SOURCES
	include/Lumino.h
	include/LuminoAudio.h
	include/LuminoDxLib.h
	include/LuminoGraphics.h
	include/LuminoScene.h
)

set(LN_SOURCES_Animation
	include/Lumino/Animation/AnimationUtilities.h
	include/Lumino/Animation/Common.h
	include/Lumino/Animation/EasingFunctions.h
	include/Lumino/Animation/EasingValue.h
	src/Animation/AnimationCurve.cpp
	src/Animation/AnimationCurve.h
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
	include/Lumino/Platform/EventArgs.h
	include/Lumino/Platform/EventListener.h
	include/Lumino/Platform/Ime.h
	include/Lumino/Platform/PlatformManager.h
	include/Lumino/Platform/PlatformSupport.h
	include/Lumino/Platform/Window.h
	src/Platform/MouseCursorVisibility.h
	src/Platform/PlatformManager.cpp
	src/Platform/WindowManagerBase.cpp
	src/Platform/WindowBase.cpp
	src/Platform/Window.cpp
)

set(LN_SOURCES_Audio
	include/LuminoAudio.h
	include/Lumino/Audio/AudioManager.h
	include/Lumino/Audio/Common.h
	include/Lumino/Audio/GameAudio.h
	include/Lumino/Audio/Sound.h
	include/Lumino/Audio/SoundListener.h
	src/Audio/AudioDevice.cpp
	src/Audio/AudioDevice.h
	src/Audio/AudioManager.cpp
	src/Audio/AudioPlayer.cpp
	src/Audio/AudioPlayer.h
	src/Audio/AudioStream.cpp
	src/Audio/AudioStream.h
	src/Audio/AudioUtils.cpp
	src/Audio/AudioUtils.h
	src/Audio/GameAudio.cpp
	src/Audio/Internal.h
	src/Audio/MidiDecoder.cpp
	src/Audio/MidiDecoder.h
	src/Audio/Mp3Decoder.cpp
	src/Audio/Mp3Decoder.h
	src/Audio/OggDecoder.cpp
	src/Audio/OggDecoder.h
	src/Audio/Sound.cpp
	src/Audio/SoundListener.cpp
	src/Audio/WaveDecoder.cpp
	src/Audio/WaveDecoder.h
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

set(LN_SOURCES_Physics
	include/Lumino/Physics/BodyBase.h
	include/Lumino/Physics/Common.h
	include/Lumino/Physics/IDebugRenderer.h
	include/Lumino/Physics/Joint.h
	include/Lumino/Physics/PhysicsManager.h
	include/Lumino/Physics/RigidBody.h
	src/Physics/BodyBase.cpp
	src/Physics/BulletUtils.h
	src/Physics/Joint.cpp
	src/Physics/PhysicsManager.cpp
	src/Physics/RigidBody.cpp
)

set(LN_SOURCES_Graphics
	include/Lumino/Graphics/Bitmap.h
	include/Lumino/Graphics/BitmapPainter.h
	include/Lumino/Graphics/Color.h
	include/Lumino/Graphics/Common.h
	include/Lumino/Graphics/Font.h
	include/Lumino/Graphics/FontManager.h
	include/Lumino/Graphics/GeometryRenderer.h
	include/Lumino/Graphics/GlyphRun.h
	include/Lumino/Graphics/GraphicsException.h
	include/Lumino/Graphics/GraphicsManager.h
	include/Lumino/Graphics/IndexBuffer.h
	include/Lumino/Graphics/Material.h
	include/Lumino/Graphics/Painter.h
	include/Lumino/Graphics/Renderer.h
	include/Lumino/Graphics/RenderState.h
	include/Lumino/Graphics/SamplerState.h
	include/Lumino/Graphics/Shader.h
	include/Lumino/Graphics/SpriteRenderer.h
	include/Lumino/Graphics/SwapChain.h
	include/Lumino/Graphics/Texture.h
	include/Lumino/Graphics/Utils.h
	include/Lumino/Graphics/VertexBuffer.h
	external/GL/glew.c
	src/Graphics/Bitmap.cpp
	src/Graphics/BitmapFont.cpp
	src/Graphics/BitmapFont.h
	src/Graphics/BitmapPainter.cpp
	src/Graphics/Color.cpp
	src/Graphics/Font.cpp
	src/Graphics/FontGlyphTextureCache.cpp
	src/Graphics/FontGlyphTextureCache.h
	src/Graphics/FontManager.cpp
	src/Graphics/FreeTypeFont.cpp
	src/Graphics/FreeTypeFont.h
	src/Graphics/GeometryRenderer.cpp
	src/Graphics/GeometryRendererImpl.h
	src/Graphics/GlyphRun.cpp
	src/Graphics/GraphicsException.cpp
	src/Graphics/GraphicsHelper.h
	src/Graphics/GraphicsManager.cpp
	src/Graphics/IndexBuffer.cpp
	src/Graphics/Internal.h
	src/Graphics/Painter.cpp
	src/Graphics/PainterEngine.cpp
	src/Graphics/PainterEngine.h
	src/Graphics/PngFile.h
	src/Graphics/ProfilerRenderer.cpp
	src/Graphics/ProfilerRenderer.h
	src/Graphics/Renderer.cpp
	src/Graphics/RenderingCommand.cpp
	src/Graphics/RenderingCommand.h
	src/Graphics/RenderingThread.cpp
	src/Graphics/RenderingThread.h
	src/Graphics/RenderState.cpp
	src/Graphics/SamplerState.cpp
	src/Graphics/Shader.cpp
	src/Graphics/SpriteRenderer.cpp
	src/Graphics/SpriteRendererImpl.h
	src/Graphics/TextLayoutEngine.cpp
	src/Graphics/TextLayoutEngine.h
	src/Graphics/TextRenderer_imaging.cpp
	src/Graphics/TextRenderer.h
	src/Graphics/SwapChain.cpp
	src/Graphics/Texture.cpp
	src/Graphics/Utils.cpp
	src/Graphics/VertexBuffer.cpp
	src/Graphics/Device/DeviceInterface.cpp
	src/Graphics/Device/DeviceInterface.h
	src/Graphics/Device/GraphicsDeviceBase.cpp
	src/Graphics/Device/GraphicsDeviceBase.h
	src/Graphics/Device/ShaderVariableBase.cpp
	src/Graphics/Device/ShaderVariableBase.h
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
	
set(LN_SOURCES_Scene
	include/Lumino/Scene/Camera.h
	include/Lumino/Scene/Collection.h
	include/Lumino/Scene/Common.h
	include/Lumino/Scene/Layer.h
	include/Lumino/Scene/Light.h
	include/Lumino/Scene/Material.h
	include/Lumino/Scene/SceneGraphManager.h
	include/Lumino/Scene/SceneNode.h
	include/Lumino/Scene/Sprite.h
	include/Lumino/Scene/ViewPane.h
	include/Lumino/Scene/VisualNode.h
	include/Lumino/Scene/VisualNodeParams.h
	include/Lumino/Scene/MME/MMESceneObject.h
	src/Scene/Camera.cpp
	src/Scene/Internal.h
	src/Scene/DebugRenderer.h
	src/Scene/InfomationRenderingPass.cpp
	src/Scene/InfomationRenderingPass.h
	src/Scene/Layer.cpp
	src/Scene/Layer.h
	src/Scene/Light.cpp
	src/Scene/RenderingPass.cpp
	src/Scene/RenderingPass.h
	src/Scene/SceneGraphManager.cpp
	src/Scene/SceneGraphManager.h
	src/Scene/SceneHelper.h
	src/Scene/SceneNode.cpp
	src/Scene/Sprite.cpp
	src/Scene/ViewPane.cpp
	src/Scene/ViewPane.h
	src/Scene/VisualNode.cpp
	src/Scene/VisualNodeParams.cpp
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

set(LN_SOURCES_GUI
	include/Lumino/GUI/AnimationClock.h
	include/Lumino/GUI/Button.h
	include/Lumino/GUI/ButtonChrome.h
	include/Lumino/GUI/Common.h
	include/Lumino/GUI/ContentControl.h
	include/Lumino/GUI/ContentElement.h
	include/Lumino/GUI/ContentPresenter.h
	include/Lumino/GUI/Control.h
	include/Lumino/GUI/ControlTemplate.h
	include/Lumino/GUI/CursorImage.h
	include/Lumino/GUI/Decorator.h
	include/Lumino/GUI/EventArgs.h
	include/Lumino/GUI/GUIContext.h
	include/Lumino/GUI/GUIInputGesture.h
	include/Lumino/GUI/GUIManager.h
	include/Lumino/GUI/GUIObjectCollection.h
	include/Lumino/GUI/ItemsControl.h
	include/Lumino/GUI/ItemsPresenter.h
	include/Lumino/GUI/Rectangle.h
	include/Lumino/GUI/RootFrame.h
	include/Lumino/GUI/RoutedCommand.h
	include/Lumino/GUI/TextBlock.h
	include/Lumino/GUI/TextBox.h
	include/Lumino/GUI/UIElement.h
	include/Lumino/GUI/UIElementCollection.h
	include/Lumino/GUI/VisualState.h
	include/Lumino/GUI/VisualTreeHelper.h
	include/Lumino/GUI/Controls/ButtonBase.h
	include/Lumino/GUI/Controls/Grid.h
	include/Lumino/GUI/Controls/Image.h
	include/Lumino/GUI/Controls/IScrollInfo.h
	include/Lumino/GUI/Controls/ListBox.h
	include/Lumino/GUI/Controls/Panel.h
	include/Lumino/GUI/Controls/RepeatButton.h
	include/Lumino/GUI/Controls/ScrollBar.h
	include/Lumino/GUI/Controls/ScrollViewer.h
	include/Lumino/GUI/Controls/StackPanel.h
	include/Lumino/GUI/Controls/Thumb.h
	include/Lumino/GUI/Controls/Track.h
	src/GUI/AnimationClock.cpp
	src/GUI/Button.cpp
	src/GUI/ButtonChrome.cpp
	src/GUI/ContentControl.cpp
	src/GUI/ContentElement.cpp
	src/GUI/ContentPresenter.cpp
	src/GUI/Control.cpp
	src/GUI/ControlTemplate.cpp
	src/GUI/CursorImage.cpp
	src/GUI/Decorator.cpp
	src/GUI/EventArgs.cpp
	src/GUI/GUIContext.cpp
	src/GUI/GUIHelper.h
	src/GUI/GUIInputGesture.cpp
	src/GUI/GUIManager.cpp
	src/GUI/GUIPainter.cpp
	src/GUI/GUIPainter.h
	src/GUI/ItemsControl.cpp
	src/GUI/ItemsPresenter.cpp
	src/GUI/Rectangle.cpp
	src/GUI/RootFrame.cpp
	src/GUI/RoutedCommand.cpp
	src/GUI/TextBlock.cpp
	src/GUI/TextBox.cpp
	src/GUI/UIElement.cpp
	src/GUI/UIElementCollection.cpp
	src/GUI/VisualState.cpp
	src/GUI/VisualTreeHelper.cpp
	src/GUI/Controls/ButtonBase.cpp
	src/GUI/Controls/Grid.cpp
	src/GUI/Controls/Image.cpp
	src/GUI/Controls/ListBox.cpp
	src/GUI/Controls/Panel.cpp
	src/GUI/Controls/RepeatButton.cpp
	src/GUI/Controls/ScrollBar.cpp
	src/GUI/Controls/ScrollViewer.cpp
	src/GUI/Controls/StackPanel.cpp
	src/GUI/Controls/Thumb.cpp
	src/GUI/Controls/Track.cpp
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

set(LN_SOURCES_Application
	include/Lumino/Application.h
	include/Lumino/ApplicationSettings.h
	include/Lumino/BindingSupport.h
	include/Lumino/Common.h
	include/Lumino/CoreObject.h
	include/Lumino/MultiThreadingObjectList.h
	include/Lumino/Profiler.h
	include/Lumino/Property.h
	include/Lumino/RoutedEvent.h
	include/Lumino/Variant.h
	src/Application.cpp
	src/ApplicationContext.cpp
	src/ApplicationContext.h
	src/ApplicationImpl.h
	src/CoreObject.cpp
	src/Internal.h
	src/Profiler.cpp
	src/Property.cpp
	src/RoutedEvent.cpp
	src/Variant.cpp
)

#--------------------------------------
# Platform source files.
if (WIN32)
	set(LN_SOURCES_Platform
		${LN_SOURCES_Platform}
		src/Platform/Win32/Clipboard_Win32.cpp
		src/Platform/Win32/Ime_Win32.cpp
		src/Platform/Win32/PlatformSupport_Win32.cpp
		src/Platform/Win32/Win32Window.cpp
		src/Platform/Win32/Win32Window.h
		src/Platform/Win32/Win32WindowBase.cpp
		src/Platform/Win32/Win32WindowBase.h
		src/Platform/Win32/Win32WindowManager.cpp
		src/Platform/Win32/Win32WindowManager.h)

 	set(LN_SOURCES_Graphics
 		${LN_SOURCES_Graphics}
		src/Graphics/Device/DirectX9/DX9Texture.h
		src/Graphics/Device/DirectX9/DX9VertexBuffer.h
		src/Graphics/Device/DirectX9/DX9GraphicsDevice.h
		src/Graphics/Device/DirectX9/DX9IndexBuffer.h
		src/Graphics/Device/DirectX9/DX9Module.h
		src/Graphics/Device/DirectX9/DX9Renderer.h
		src/Graphics/Device/DirectX9/DX9Shader.h
		src/Graphics/Device/DirectX9/DX9SwapChain.h
		src/Graphics/Device/OpenGL/WGLGraphicsDevice.h
		src/Graphics/Device/OpenGL/WGLSwapChain.h
	 	src/Graphics/Device/DirectX9/DX9Texture.cpp
		src/Graphics/Device/DirectX9/DX9VertexBuffer.cpp
		src/Graphics/Device/DirectX9/DX9GraphicsDevice.cpp
		src/Graphics/Device/DirectX9/DX9IndexBuffer.cpp
		src/Graphics/Device/DirectX9/DX9Module.cpp
		src/Graphics/Device/DirectX9/DX9Renderer.cpp
		src/Graphics/Device/DirectX9/DX9Shader.cpp
		src/Graphics/Device/DirectX9/DX9SwapChain.cpp
		src/Graphics/Device/OpenGL/WGLGraphicsDevice.cpp
		src/Graphics/Device/OpenGL/WGLSwapChain.cpp)

elseif (APPLE)

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
source_group("Animation"	FILES ${LN_SOURCES_Animation})
source_group("IO"			FILES ${LN_SOURCES_IO})
source_group("Platform"		FILES ${LN_SOURCES_Platform})
source_group("Audio"		FILES ${LN_SOURCES_Audio})
source_group("Physics"		FILES ${LN_SOURCES_Physics})
source_group("Graphics"		FILES ${LN_SOURCES_Graphics})
source_group("Scene"		FILES ${LN_SOURCES_Scene})
source_group("GUI"			FILES ${LN_SOURCES_GUI})
source_group("Documents"	FILES ${LN_SOURCES_Documents})
source_group("Game"			FILES ${LN_SOURCES_Game})
source_group("Application"	FILES ${LN_SOURCES_Application})

set(LN_SOURCES
	${LN_SOURCES}
	${LN_SOURCES_Animation}
	${LN_SOURCES_IO}
	${LN_SOURCES_Platform}
	${LN_SOURCES_Audio}
	${LN_SOURCES_Physics}
	${LN_SOURCES_Graphics}
	${LN_SOURCES_Scene}
	${LN_SOURCES_GUI}
	${LN_SOURCES_Documents}
	${LN_SOURCES_Game}
	${LN_SOURCES_Application}
	${LN_SOURCES_C_API})

#------------------------------------------------------------------------------
# Apply options
#------------------------------------------------------------------------------
# Unicode char set.
if (LN_USE_UNICODE_CHAR_SET)
	add_definitions(-DUNICODE)
	add_definitions(-D_UNICODE)
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

elseif (APPLE)
	set(LN_UNIT_TEST_LINK_LIBS "pthread")
	
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
	${PROJECT_SOURCE_DIR}/external/bullet-2.82
	${PROJECT_SOURCE_DIR}/external/freetype-2.6/include
	${PROJECT_SOURCE_DIR}/external/libogg/include)
include_directories(${LN_INCLUDE_DIRECTORIES} ${LN_PLATFORM_INCLUDE_DIRS})

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
# external deps.
add_subdirectory(external/Lumino.Core)
add_subdirectory(external/Lumino.Math)

# Make postfix.
ln_make_postfix(LN_LIB_POSTFIX)

add_subdirectory(external/zlib)
add_subdirectory(external/libpng)
add_subdirectory(external/bullet-2.82)
add_subdirectory(external/freetype-2.6)
add_subdirectory(external/libogg)
add_subdirectory(external/libvorbis)
link_libraries(LuminoCore_Static)
link_libraries(LuminoMath_Static)
link_libraries(zlib)
link_libraries(libpng)
link_libraries(bullet)
link_libraries(freetype)
link_libraries(libogg)
link_libraries(libvorbis)

#------------------------------
# Static library.
add_library(${PROJECT_NAME}_Static STATIC ${LN_SOURCES})
set_target_properties(${PROJECT_NAME}_Static PROPERTIES OUTPUT_NAME "${LN_STATIC_LIB_OUTPUT_NAME}${LN_LIB_POSTFIX}")
#set_target_properties(${PROJECT_NAME}_Static PROPERTIES INCLUDE_DIRECTORIES "${LN_INCLUDE_DIRECTORIES}")
target_link_libraries(${PROJECT_NAME}_Static ${LN_LINK_LIBS})
add_custom_command(
	TARGET ${PROJECT_NAME}_Static
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}_Static> "${PROJECT_SOURCE_DIR}/lib")

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
add_subdirectory(bindings/LightNote)
