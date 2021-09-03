cmake_minimum_required (VERSION 3.1.0)

# - グローバルな各フラグを書き換える。このファイルの include 後に作られるプロジェクトのビルド設定が変わる。
#	* CMAKE_C_FLAGS, CMAKE_C_FLAGS_DEBUG, CMAKE_C_FLAGS_RELEASE

#------------------------------------------------------------------------------
# options
#------------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 17)

#------------------------------------------------------------------------------
# make lib dir
#------------------------------------------------------------------------------
set(LN_LIB_OUTPUT_LOCAL_DIR "lib/")

if (WIN32)
	# MSVC version
	if (MSVC_VERSION GREATER_EQUAL 1910)
		set(LN_TARGET_ENV "MSVC150")
	elseif (MSVC_VERSION GREATER_EQUAL 1900)
		set(LN_TARGET_ENV "MSVC140")
	elseif (MSVC_VERSION GREATER_EQUAL 1800)
		set(LN_TARGET_ENV "MSVC120")
	elseif (MSVC_VERSION GREATER_EQUAL 1700)
		set(LN_TARGET_ENV "MSVC110")
	elseif (MSVC_VERSION GREATER_EQUAL 1600)
		set(LN_TARGET_ENV "MSVC100")
	elseif (MSVC_VERSION GREATER_EQUAL 1500)
		set(LN_TARGET_ENV "MSVC90")
	elseif (MSVC_VERSION GREATER_EQUAL 1400)
		set(LN_TARGET_ENV "MSVC80")
	endif()
	set(LN_LIB_OUTPUT_LOCAL_DIR "${LN_LIB_OUTPUT_LOCAL_DIR}${LN_TARGET_ENV}")

	# Architecture.
	# http://stackoverflow.com/questions/5334095/cmake-multiarchitecture-compilation
	if (${CMAKE_EXE_LINKER_FLAGS} MATCHES "/machine:x64")	# /machine:x64
		set(LN_ARCH "x64")
	else()
		set(LN_ARCH "x86")
	endif()
	set(LN_LIB_OUTPUT_LOCAL_DIR "${LN_LIB_OUTPUT_LOCAL_DIR}/${LN_ARCH}")

	# Runtime library
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
	if (LN_MSVC_STATIC_RUNTIME)
		set(LN_LIB_OUTPUT_LOCAL_DIR "${LN_LIB_OUTPUT_LOCAL_DIR}_MT/")
	else()
		set(LN_LIB_OUTPUT_LOCAL_DIR "${LN_LIB_OUTPUT_LOCAL_DIR}_MD/")
	endif()

elseif(APPLE)
	set(LN_LIB_OUTPUT_LOCAL_DIR "${LN_LIB_OUTPUT_LOCAL_DIR}OSX")

endif()

# make fullpath
set(LN_LIB_DEBUG_OUTPUT_ROOT_DIR "${CMAKE_SOURCE_DIR}/${LN_LIB_OUTPUT_LOCAL_DIR}Debug/")
set(LN_LIB_RELEASE_OUTPUT_ROOT_DIR "${CMAKE_SOURCE_DIR}/${LN_LIB_OUTPUT_LOCAL_DIR}Release/")

#------------------------------------------------------------------------------
# Options
#------------------------------------------------------------------------------
# Unicode char set.
add_definitions(-DUNICODE)
add_definitions(-D_UNICODE)

if (WIN32)
	add_definitions("/EHsc")	# VS2015, cmake

	# ランタイムライブラリ
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
	
	# Architecture.
	# http://stackoverflow.com/questions/5334095/cmake-multiarchitecture-compilation
	if (${CMAKE_EXE_LINKER_FLAGS} MATCHES "/machine:x64")	# /machine:x64
		set(LN_ARCH "x64")
	else()
		set(LN_ARCH "x86")
	endif()
	
	# MSVC version
	if (MSVC_VERSION GREATER_EQUAL 1910)
		set(LN_TARGET_ENV "MSVC150")
	elseif (MSVC_VERSION GREATER_EQUAL 1900)
		set(LN_TARGET_ENV "MSVC140")
	elseif (MSVC_VERSION GREATER_EQUAL 1800)
		set(LN_TARGET_ENV "MSVC120")
	elseif (MSVC_VERSION GREATER_EQUAL 1700)
		set(LN_TARGET_ENV "MSVC110")
	elseif (MSVC_VERSION GREATER_EQUAL 1600)
		set(LN_TARGET_ENV "MSVC100")
	elseif (MSVC_VERSION GREATER_EQUAL 1500)
		set(LN_TARGET_ENV "MSVC90")
	elseif (MSVC_VERSION GREATER_EQUAL 1400)
		set(LN_TARGET_ENV "MSVC80")
	endif()
endif()

#------------------------------------------------------------------------------
# functions
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# e.g.) ln_add_pch(LuminoCore ${LN_SOURCES} "src/LuminoCore.PCH.h" "src/LuminoCore.PCH.cpp")
function(ln_add_pch project_name header_file_path source_file_path)

	get_filename_component(header_file_name ${header_file_path} NAME)

	if (MSVC)
		set(ln_compile_flags
			#"/Yu\"${header_file_name}\" /FI\"${header_file_name}\""	# use PCH, ForcedIncludeFiles
			"/Yu\"${header_file_name}\""
		)
		#target_compile_definitions(${project_name} PUBLIC /FI\"${header_file_name}\")
		target_compile_options(${project_name} PRIVATE /FI\"${header_file_name}\")

	else()
		# https://github.com/nanoant/CMakePCHCompiler/blob/master/CMakePCHCompiler.cmake

		get_filename_component(result ${header_file_path} ABSOLUTE)

		# force include header
		set(ln_compile_flags "-include \"${result}\"")
	endif()


	# get source files from project (referred LLVM)
	get_property(source_files TARGET ${project_name} PROPERTY SOURCES)

	# set ln_compile_flags to all *.cpp
	foreach (file ${source_files})
		if (${file} MATCHES ".+\\.cpp")
			set_source_files_properties(${file} PROPERTIES COMPILE_FLAGS ${ln_compile_flags})
		endif()
	endforeach()

	# create .pch config
	if (MSVC)
		set_source_files_properties(${source_file_path} PROPERTIES COMPILE_FLAGS "/Yc\"${header_file_name}\"")
	endif()

endfunction()
