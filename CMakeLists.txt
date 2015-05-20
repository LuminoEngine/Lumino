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
option(MSVC_LINK_MULTI_THREAD_STATIC_RUNTIME "static runtime (/MT, /MTd) or DLL runtime (/MD, /MDd) or to link either." ON)

# Exceprion Backtrace.
option(LN_EXCEPTION_BACKTRACE "In some unix environment there is a possibility that can not be compiled." ON)

# Architecture.
#option(LN_BUILD_X64 "Architecture." OFF)


#------------------------------------------------------------------------------
# Files
#------------------------------------------------------------------------------
#------------------------------
# Source files.

set(LN_SOURCES_Platform
	include/Lumino/Platform/Application.h
	include/Lumino/Platform/EventArgs.h
	include/Lumino/Platform/EventListener.h
	include/Lumino/Platform/PlatformSupport.h
	include/Lumino/Platform/Window.h
	src/Platform/WindowManagerBase.cpp
	src/Platform/WindowBase.cpp
	src/Platform/Window.cpp
	src/Platform/Application.cpp)

set(LN_SOURCES_Physics
	src/Physics/BodyBase.cpp
	src/Physics/BodyBase.h
	src/Physics/Common.h
	src/Physics/IDebugRenderer.h
	src/Physics/Joint.cpp
	src/Physics/Joint.h
	src/Physics/PhysicsManager.h
	src/Physics/PhysicsManager.cpp
	src/Physics/RigidBody.cpp
	src/Physics/RigidBody.h)
	
set(LN_SOURCES_Graphics
	external/GL/glew.c
	src/Imaging/Bitmap.cpp
	src/Imaging/BitmapPainter.cpp
	src/Graphics/Color.cpp
	src/Graphics/GeometryRenderer.cpp
	src/Graphics/GraphicsException.cpp
	src/Graphics/GraphicsDevice.cpp
	src/Graphics/GraphicsManager.cpp
	src/Graphics/IndexBuffer.cpp
	src/Graphics/Renderer.cpp
	src/Graphics/RenderingCommand.cpp
	src/Graphics/RenderingThread.cpp
	src/Graphics/RenderState.cpp
	src/Graphics/SamplerState.cpp
	src/Graphics/Shader.cpp
	src/Graphics/SwapChain.cpp
	src/Graphics/Texture.cpp
	src/Graphics/Utils.cpp
	src/Graphics/VertexBuffer.cpp
	src/Graphics/Device/DeviceInterface.cpp
	src/Graphics/Device/GraphicsDeviceBase.h
	src/Graphics/Device/GraphicsDeviceBase.cpp
	src/Graphics/Device/ShaderVariableBase.cpp
	src/Graphics/Device/OpenGL/GLGraphicsDevice.cpp
	src/Graphics/Device/OpenGL/GLIndexBuffer.cpp
	src/Graphics/Device/OpenGL/GLRenderer.cpp
	src/Graphics/Device/OpenGL/GLShader.cpp
	src/Graphics/Device/OpenGL/GLSwapChain.cpp
	src/Graphics/Device/OpenGL/GLTexture.cpp
	src/Graphics/Device/OpenGL/GLVertexBuffer.cpp)
	
set(LN_SOURCES_Scene
	src/Scene/Camera.cpp
	src/Scene/Camera.h
	src/Scene/Common.h
	src/Scene/DebugRenderer.h
	src/Scene/InfomationRenderingPass.cpp
	src/Scene/InfomationRenderingPass.h
	src/Scene/Layer.cpp
	src/Scene/Layer.h
	src/Scene/Light.cpp
	src/Scene/Light.h
	src/Scene/RenderingPass.cpp
	src/Scene/RenderingPass.h
	src/Scene/SceneGraphManager.cpp
	src/Scene/SceneGraphManager.h
	src/Scene/SceneNode.cpp
	src/Scene/SceneNode.h
	src/Scene/Sprite.cpp
	src/Scene/Sprite.h
	src/Scene/ViewPane.cpp
	src/Scene/ViewPane.h
	src/Scene/VisualNode.cpp
	src/Scene/VisualNode.h
	src/Scene/VisualNodeParams.h
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
	src/Scene/MME/MMETypes.h)

#--------------------------------------
# Platform source files.
if (WIN32)
	set(LN_SOURCES_Platform
		${LN_SOURCES_Platform}
		src/Platform/Win32/Win32WindowManager.cpp
		src/Platform/Win32/Win32WindowBase.cpp
		src/Platform/Win32/Win32Window.cpp
		src/Platform/Win32/PlatformSupport_Win32.cpp)

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
		src/Platform/X11/PlatformSupport_X11.cpp)
		
	set(LN_SOURCES_Graphics
		${LN_SOURCES_Graphics}
		src/Graphics/Device/OpenGL/GLXGraphicsDevice.cpp
		src/Graphics/Device/OpenGL/GLXSwapChain.cpp)
		
	add_definitions(-DGLEW_NO_GLU)
	#add_definitions(-DGLEW_MX)
	add_definitions(-DGLEW_BUILD)
	#add_definitions(-DGLEW_STATIC)
endif()

#--------------------------------------
# Grouping.
source_group("Platform"	FILES ${LN_SOURCES_Platform})
source_group("Physics"	FILES ${LN_SOURCES_Physics})
source_group("Graphics"	FILES ${LN_SOURCES_Graphics})
source_group("Scene"	FILES ${LN_SOURCES_Scene})

set(LN_SOURCES
	${LN_SOURCES_Platform}
	${LN_SOURCES_Physics}
	${LN_SOURCES_Graphics}
	${LN_SOURCES_Scene})

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
	#------------------------------------------------------
	# Make static lib name.
	#	{ProjectName}_{msvcVer}{Arch}_static_{Runtime}
	#	e.g)	LuminoMath_msvc120x86_static_MTd.lib
	if (MSVC_VERSION EQUAL 1400)
		set(LN_LIB_POSTFIX "${LN_LIB_POSTFIX}_msvc80")
	elseif (MSVC_VERSION EQUAL 1500)
		set(LN_LIB_POSTFIX "${LN_LIB_POSTFIX}_msvc90")
	elseif (MSVC_VERSION EQUAL 1600)
		set(LN_LIB_POSTFIX "${LN_LIB_POSTFIX}_msvc100")
	elseif (MSVC_VERSION EQUAL 1700)
		set(LN_LIB_POSTFIX "${LN_LIB_POSTFIX}_msvc110")
	elseif (MSVC_VERSION EQUAL 1800)
		set(LN_LIB_POSTFIX "${LN_LIB_POSTFIX}_msvc120")
	endif()
	
	# Architecture.
	# http://stackoverflow.com/questions/5334095/cmake-multiarchitecture-compilation
	if (${CMAKE_EXE_LINKER_FLAGS} MATCHES "/machine:x64")	# /machine:x64 
		set(LN_LIB_POSTFIX "${LN_LIB_POSTFIX}x64")
	else()
		set(LN_LIB_POSTFIX "${LN_LIB_POSTFIX}x86")
	endif()
	
	set(LN_LIB_POSTFIX "${LN_LIB_POSTFIX}_static")
	
	# MSVC Runtime library.
	if (MSVC_LINK_MULTI_THREAD_STATIC_RUNTIME)
		set(LN_LIB_POSTFIX "${LN_LIB_POSTFIX}_MT")
		# Overwrite options.
		foreach (flag
			CMAKE_C_FLAGS
			CMAKE_C_FLAGS_DEBUG
			CMAKE_C_FLAGS_RELEASE
			CMAKE_CXX_FLAGS
			CMAKE_CXX_FLAGS_DEBUG
			CMAKE_CXX_FLAGS_RELEASE)
			if (${flag} MATCHES "/MD")
				string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
			endif()
			if (${flag} MATCHES "/MDd")
				string(REGEX REPLACE "/MDd" "/MTd" ${flag} "${${flag}}")
			endif()
		endforeach()
	else()
		set(LN_LIB_POSTFIX "${LN_LIB_POSTFIX}_MD")
	endif()
	set(CMAKE_DEBUG_POSTFIX "d")
	# end   Make static lib name.
	#------------------------------------------------------

	# platform include dirs.
	set(LN_PLATFORM_INCLUDE_DIRS "$ENV{DXSDK_DIR}Include")
	
	# link librarys.
	list(APPEND LN_LINK_LIBS
		"imagehlp.lib"
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
	${PROJECT_SOURCE_DIR}/external/bullet-2.82)
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
add_subdirectory(external/zlib)
add_subdirectory(external/libpng)
add_subdirectory(external/bullet-2.82)
add_subdirectory(external/Lumino.Core)
add_subdirectory(external/Lumino.Math)
link_libraries(zlib)
link_libraries(libpng)
link_libraries(bullet)
link_libraries(LuminoCore_Static)
link_libraries(LuminoMath_Static)

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
# samples.
#add_subdirectory(sample)
