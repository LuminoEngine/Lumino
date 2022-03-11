require "mkmf"

# Use for...
# - .so filename (lumino_ext.so)
# - Ruby ext entry point name (Init_lumino_ext)
#
# パッケージ名とは異なる名前にしておいたほうが良い。
# ユーザプログラムが require "lumino" すると、まずは lib/lumino.rb が読み込まれた後、前処理を経てから lumino_ext.so を読み込む。
# 名前の衝突により混乱する可能性があるため、あえて分けておく。
target_name = "lumino_ext"

# Add __dir__ to $LDFLAGS(-L option)
dir_config(target_name, nil, __dir__)

have_library("stdc++")
have_library("OPENGL32")

# have_library("glfw3")
# have_library("libpng16")
# have_library("ogg")
# have_library("vorbis")
# have_library("vorbisfile")
# have_library("yaml-cpp")
# have_library("zlib1")
# have_library("fmt")
# have_library("freetype")
have_library("LuminoFFI")

create_makefile(target_name)
