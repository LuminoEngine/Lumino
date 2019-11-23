include("${CMAKE_CURRENT_LIST_DIR}/LuminoCoreTargets.cmake")

#------------------------------------------------------------------------------
function(ln_add_pch project_name header_file_path source_file_path)
	get_filename_component(header_file_name ${header_file_path} NAME)
	if (MSVC)
		set(ln_compile_flags "/Yu\"${header_file_name}\"")
		target_compile_options(${project_name} PRIVATE /FI\"${header_file_name}\")

	else()
		get_filename_component(result ${header_file_path} ABSOLUTE)
		set(ln_compile_flags "-include \"${result}\"")
	endif()

	# get source files from project
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

