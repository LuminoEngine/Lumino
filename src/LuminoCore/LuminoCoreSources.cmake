
#--------------------------------------
# Source files.

file(
    GLOB
    LN_SOURCES_Base
    "${CMAKE_CURRENT_LIST_DIR}/include/Lumino/Base/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/Base/*.cpp")

file(
    GLOB
    LN_SOURCES_IO
    "${CMAKE_CURRENT_LIST_DIR}/include/Lumino/IO/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/IO/*.cpp")

file(
    GLOB
    LN_SOURCES_Json
    "${CMAKE_CURRENT_LIST_DIR}/include/Lumino/Json/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/Json/*.cpp")

file(
    GLOB
    LN_SOURCES_Math
    "${CMAKE_CURRENT_LIST_DIR}/include/Lumino/Math/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/Math/*.cpp")

file(
    GLOB
    LN_SOURCES_Serialization
    "${CMAKE_CURRENT_LIST_DIR}/include/Lumino/Serialization/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/Serialization/*.cpp")

file(
    GLOB
    LN_SOURCES_Testing
    "${CMAKE_CURRENT_LIST_DIR}/include/Lumino/Testing/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/Testing/*.cpp")

file(
    GLOB
    LN_SOURCES_Text
    "${CMAKE_CURRENT_LIST_DIR}/include/Lumino/Text/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/Text/*.cpp")

file(
    GLOB
    LN_SOURCES_Threading
    "${CMAKE_CURRENT_LIST_DIR}/include/Lumino/Threading/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/Threading/*.cpp")

file(
    GLOB
    LN_SOURCES_Xml
    "${CMAKE_CURRENT_LIST_DIR}/include/Lumino/Xml/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/Xml/*.cpp")

#--------------------------------------
# Platform source files.

if (WIN32)
    set(LN_SOURCES_Text
        ${LN_SOURCES_Text}
        ${CMAKE_CURRENT_LIST_DIR}/src/Text/Win32CodePageEncoding.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/Text/Win32CodePageEncoding.hpp
    )

elseif (APPLE)
    set(LN_SOURCES_Base
        ${LN_SOURCES_Base}
        ${CMAKE_CURRENT_LIST_DIR}/src/Base/Foundation_macOS.mm
    )

elseif (UNIX)

endif()

#--------------------------------------
# Grouping.
source_group("Base"             FILES ${LN_SOURCES_Base})
source_group("Text"             FILES ${LN_SOURCES_Text})
source_group("IO"               FILES ${LN_SOURCES_IO})
source_group("Json"             FILES ${LN_SOURCES_Json})
source_group("Threading"        FILES ${LN_SOURCES_Threading})
source_group("Xml"              FILES ${LN_SOURCES_Xml})
source_group("Math"             FILES ${LN_SOURCES_Math})
source_group("Serialization"    FILES ${LN_SOURCES_Serialization})
source_group("Testing"          FILES ${LN_SOURCES_Testing})

set(LN_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/include/LuminoCore.hpp
    ${LN_SOURCES_Base}
    ${LN_SOURCES_Text}
    ${LN_SOURCES_IO}
    ${LN_SOURCES_Json}
    ${LN_SOURCES_Threading}
    ${LN_SOURCES_Xml}
    ${LN_SOURCES_Math}
    ${LN_SOURCES_Serialization}
    ${LN_SOURCES_Testing})

set(LN_SOURCES_LuminoCore ${LN_SOURCES})
