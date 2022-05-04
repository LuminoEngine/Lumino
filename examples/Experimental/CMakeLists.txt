cmake_minimum_required (VERSION 3.0.0)
project(LuminoEngine-examples)

file(GLOB LOCAL_SOURCES_Common *.cpp *.hpp)
file(GLOB LOCAL_SOURCES_Basic Basic/*.hpp Basic/*.cpp Basic/*.rb Basic/*.hsp)
file(GLOB LOCAL_SOURCES_Features Features/*.hpp Features/*.cpp)
file(GLOB LOCAL_SOURCES_Experimental Experimental/*.hpp Experimental/*.cpp)
file(GLOB LOCAL_SOURCES_Obsolete Obsolete/*.hpp Obsolete/*.cpp)
set(LOCAL_SOURCES
    ${LOCAL_SOURCES_Common}
    ${LOCAL_SOURCES_Basic}
    ${LOCAL_SOURCES_Features}
    ${LOCAL_SOURCES_Experimental}
    ${LOCAL_SOURCES_Obsolete}
)

source_group("" FILES ${LOCAL_SOURCES_Common})
source_group("Basic" FILES ${LOCAL_SOURCES_Basic})
source_group("Features" FILES ${LOCAL_SOURCES_Features})
source_group("Experimental" FILES ${LOCAL_SOURCES_Experimental})
source_group("Obsolete" FILES ${LOCAL_SOURCES_Obsolete})

include_directories(
    ${LUMINO_INCLUDE_DIRECTORIES}
    .
)

add_executable(${PROJECT_NAME} ${LOCAL_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE LuminoEngine)
target_compile_definitions(${PROJECT_NAME} PRIVATE LN_GLTF_SAMPLE_MODELS_DIR="${LN_GLTF_SAMPLE_MODELS_DIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
ln_add_pch(${PROJECT_NAME} "Example.PCH.hpp" "Example.PCH.cpp")
