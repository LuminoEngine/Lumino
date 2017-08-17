
# LN_DEPENDENCIES_DEBUG_OUTPUT_DIR
# LN_DEPENDENCIES_RELEASE_OUTPUT_DIR
#   出力フォルダパス。終端は / であること。
# LN_DEPENDENCIES_POSTFIX
#   ライブラリファイルの接尾辞
#
# ParentScope の次の変数に格納する
# LN_DEPENDENCIES_INCLUDE_DIRECTORIES
#   外部ライブラリを使うために必要なインクルードパス一式
# LN_DEPENDENCIES_LIBRARIES
#   外部ライブラリを使うために必要なライブラリ(依存プロジェクト)一式

cmake_minimum_required(VERSION 3.0.0)
project(LuminoDependencies)

#-------------------------------------------------------------------------------

# Visual Studio ソリューションフォルダを作るようにする
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

macro(ln_add_common_property projectName)
	set_target_properties(${projectName} PROPERTIES OUTPUT_NAME "${projectName}${LN_DEPENDENCIES_POSTFIX}")
	set_target_properties(${projectName} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "")
	set_target_properties(${projectName} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${LN_DEPENDENCIES_DEBUG_OUTPUT_DIR}")
	set_target_properties(${projectName} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${LN_DEPENDENCIES_RELEASE_OUTPUT_DIR}")
	set_target_properties(${projectName} PROPERTIES PREFIX "")
	set_target_properties(${projectName} PROPERTIES FOLDER "Dependencies")
	set(LN_DEPENDENCIES_LIBRARIES ${LN_DEPENDENCIES_LIBRARIES} ${projectName})
endmacro()

if (MSVC)
	add_definitions("/wd4996")		# pragma warning disable
endif()

#-------------------------------------------------------------------------------
add_subdirectory(zlib-1.2.7)
ln_add_common_property(zlib)

add_subdirectory(bullet-2.82)
ln_add_common_property(bullet)

add_subdirectory(freetype-2.6)
ln_add_common_property(freetype)

add_subdirectory(giflib-5.1.4)
ln_add_common_property(giflib)

add_subdirectory(libogg-1.3.2)
ln_add_common_property(libogg)

add_subdirectory(libpng-1.6.12)
ln_add_common_property(libpng)

add_subdirectory(libvorbis-1.3.5)
ln_add_common_property(libvorbis)


#---------------------------------------
# Effekseer
if (LN_MSVC_STATIC_RUNTIME)
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

add_subdirectory(EffekseerRuntime101/src)

ln_add_common_property(effekseer)
ln_add_common_property(effekseer_renderer_gl)

if (MSVC)
	ln_add_common_property(effekseer_renderer_dx9)
	ln_add_common_property(effekseer_renderer_dx11)
	ln_add_common_property(effekseer_sound_xaudio2)
endif()

set(LN_DEPENDENCIES_INCLUDE_DIRECTORIES ${LN_DEPENDENCIES_INCLUDE_DIRECTORIES} ${PROJECT_SOURCE_DIR}/EffekseerRuntime101/src/include)

#-------------------------------------------------------------------------------
# finalize

set(LN_DEPENDENCIES_INCLUDE_DIRECTORIES ${LN_DEPENDENCIES_INCLUDE_DIRECTORIES} PARENT_SCOPE)
set(LN_DEPENDENCIES_LIBRARIES ${LN_DEPENDENCIES_LIBRARIES} PARENT_SCOPE)
