require 'mkmf'
have_library('stdc++')
have_library("OPENGL32")
have_library("LuminoRuntime")

create_makefile("Lumino", ".")
