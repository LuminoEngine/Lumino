cmake_minimum_required(VERSION 3.1.0)
project(lua)

add_library(${PROJECT_NAME} onelua.c)

install(
    TARGETS ${PROJECT_NAME}
    ARCHIVE     DESTINATION ./lib
    INCLUDES    DESTINATION ./include)

install(FILES lua.h luaconf.h DESTINATION include)

