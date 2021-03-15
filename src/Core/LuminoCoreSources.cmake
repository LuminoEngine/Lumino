
set(LN_CORE_INCLUDE_DIR ${LUMINO_ROOT_PATH}/include)

#--------------------------------------
# Source files.

file(
    GLOB
    LN_SOURCES_Base
    "${LN_CORE_INCLUDE_DIR}/LuminoCore/Base/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/Base/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/Base/*.hpp")

file(
    GLOB
    LN_SOURCES_IO
    "${LN_CORE_INCLUDE_DIR}/LuminoCore/IO/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/IO/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/IO/*.hpp")

file(
    GLOB
    LN_SOURCES_Json
    "${LN_CORE_INCLUDE_DIR}/LuminoCore/Json/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/Json/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/Json/*.hpp")

file(
    GLOB
    LN_SOURCES_Math
    "${LN_CORE_INCLUDE_DIR}/LuminoCore/Math/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/Math/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/Math/*.hpp")

file(
    GLOB
    LN_SOURCES_Serialization
    "${LN_CORE_INCLUDE_DIR}/LuminoCore/Serialization/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/Serialization/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/Serialization/*.hpp")

file(
    GLOB
    LN_SOURCES_Testing
    "${LN_CORE_INCLUDE_DIR}/LuminoCore/Testing/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/Testing/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/Testing/*.hpp")

file(
    GLOB
    LN_SOURCES_Text
    "${LN_CORE_INCLUDE_DIR}/LuminoCore/Text/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/Text/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/Text/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/Text/*.c")

file(
    GLOB
    LN_SOURCES_Threading
    "${LN_CORE_INCLUDE_DIR}/LuminoCore/Threading/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/Threading/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/Threading/*.hpp")

file(
    GLOB
    LN_SOURCES_Xml
    "${LN_CORE_INCLUDE_DIR}/LuminoCore/Xml/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/Xml/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/Xml/*.hpp")

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
    ${LN_CORE_INCLUDE_DIR}/LuminoCore.hpp
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
