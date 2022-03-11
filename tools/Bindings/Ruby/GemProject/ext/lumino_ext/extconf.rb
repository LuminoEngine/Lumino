require "mkmf"

# Use for...
# - .so filename (lumino_ext.so)
# - Ruby ext entry point name (Init_lumino_ext)
target_name = "lumino_ext"

# Add __dir__ to $LDFLAGS(-L option)
dir_config(target_name, nil, __dir__)

have_library("stdc++")
have_library("OPENGL32")

have_library("glfw3")
have_library("libpng16")
have_library("LuminoFFI")
have_library("ogg")
have_library("vorbis")
have_library("vorbisfile")
have_library("yaml-cpp")
have_library("zlib1")
have_library("fmt")
have_library("freetype")

create_makefile(target_name)
