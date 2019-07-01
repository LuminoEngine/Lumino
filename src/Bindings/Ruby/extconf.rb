require 'mkmf'
have_library('stdc++')
have_library("OPENGL32")

#have_library("LuminoC")

create_makefile("Lumino", "src")
