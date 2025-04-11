

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

