project(LuminoBitmap)
include(cmake/LuminoConfig-LuminoBitmap.cmake)

#-------------------------------------------------------------------------------
# Files
#-------------------------------------------------------------------------------

file(GLOB LN_SOURCES_Bitmap include/LuminoBitmap/*.hpp src/*.cpp src/*.hpp)

source_group("Bitmap" FILES ${LN_SOURCES_Bitmap})

set(LN_SOURCES
    src/pch.cpp
    src/pch.hpp
    src/Internal.hpp
    ${LN_SOURCES_Bitmap}
)

#-------------------------------------------------------------------------------
# Configuration
#-------------------------------------------------------------------------------


#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------

add_library(${PROJECT_NAME} STATIC ${LN_SOURCES})
target_include_directories(
    ${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_include_directories(${PROJECT_NAME} PRIVATE include src) # for PCH
target_link_libraries(${PROJECT_NAME} PRIVATE
    LuminoCore
    LuminoRuntime
    nanovg::nanovg
    ZLIB::ZLIB
)

# libpng の static 版は名前が違う。
if (LN_LINK_CMAKE_SHARED_LIB_PACKAGE)
    target_link_libraries(${PROJECT_NAME} PRIVATE png)
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE png_static)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
ln_add_pch(${PROJECT_NAME} "src/pch.hpp" "src/pch.cpp")

#-------------------------------------------------------------------------------
# Installation
#-------------------------------------------------------------------------------
install(
    TARGETS     ${PROJECT_NAME}
    EXPORT      LuminoExport
    ARCHIVE     DESTINATION ./lib
    INCLUDES    DESTINATION ./include
)
install(DIRECTORY include DESTINATION .)
install(FILES cmake/LuminoConfig-${PROJECT_NAME}.cmake DESTINATION lib/cmake/Lumino)

#-------------------------------------------------------------------------------
# Sub-projects
#-------------------------------------------------------------------------------

if (LN_BUILD_TESTS)
    add_subdirectory(test)
endif()
