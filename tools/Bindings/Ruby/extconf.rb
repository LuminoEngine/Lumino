require 'mkmf'
have_library('stdc++')
have_library("OPENGL32")
have_library("LuminoRuntime")

$CXXFLAGS += " -I../FlatC/include -I../../../include"
create_makefile("Lumino", ".")
