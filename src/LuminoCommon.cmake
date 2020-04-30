

if (DEFINED EMSCRIPTEN)
    set(LN_EMSCRIPTEN ON)
    #set(LN_USE_SDL ON)

elseif (ANDROID_ABI)
	set(LN_ANDROID ON)

elseif (DEFINED IOS_PLATFORM)
    set(LN_IOS ON)
    set(LN_USE_OPENAL ON)

elseif(WIN32 OR APPLE OR UNIX)
    set(LN_OS_DESKTOP ON)
    set(LN_USE_OPENAL ON)

endif()



#------------------------------------------------------------------------------
function(ln_add_msvc_runtime_flag proj)
  if(${LN_USE_MSVC_STATIC_RUNTIME})
    target_compile_options(${proj} PRIVATE $<$<NOT:$<CONFIG:Debug>>:/MT> $<$<CONFIG:Debug>:/MTd>)
  else()
    target_compile_options(${proj} PRIVATE $<$<NOT:$<CONFIG:Debug>>:/MD> $<$<CONFIG:Debug>:/MDd>)
  endif()
endfunction()


#------------------------------------------------------------------------------


function(ln_add_pch project_name header_file_path source_file_path)

	get_filename_component(header_file_name ${header_file_path} NAME)

	if (MSVC)
		set(ln_compile_flags
			#"/Yu\"${header_file_name}\" /FI\"${header_file_name}\""	# use PCH, ForcedIncludeFiles
			"/Yu\"${header_file_name}\" /FI\"${header_file_name}\""
		)
		#target_compile_definitions(${project_name} PUBLIC /FI\"${header_file_name}\")
		#target_compile_options(${project_name} PRIVATE /FI\"${header_file_name}\")

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
