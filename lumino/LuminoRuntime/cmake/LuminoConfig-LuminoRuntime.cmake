
find_package(yaml-cpp CONFIG REQUIRED)
find_package(VulkanHeaders CONFIG REQUIRED)

#if (NOT DEFINED EMSCRIPTEN)
#    find_package(CURL CONFIG REQUIRED)
#endif()

if (LN_OS_DESKTOP)
    find_package(glfw3 CONFIG REQUIRED)
    find_package(glad CONFIG REQUIRED)
endif()

if (LN_OS_DESKTOP)
    find_package(glslang CONFIG REQUIRED)
    #find_package(spirv_cross_c CONFIG REQUIRED)
    find_package(spirv_cross_core CONFIG REQUIRED)
    #find_package(spirv_cross_cpp CONFIG REQUIRED)
    find_package(spirv_cross_glsl CONFIG REQUIRED)
    find_package(spirv_cross_hlsl CONFIG REQUIRED)
    find_package(spirv_cross_msl CONFIG REQUIRED)
    find_package(spirv_cross_reflect CONFIG REQUIRED)
    find_package(spirv_cross_util CONFIG REQUIRED)
endif()

