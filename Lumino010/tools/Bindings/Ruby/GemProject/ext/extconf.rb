require 'mkmf'

dir_config('lumino')

have_library('stdc++')
have_library('OPENGL32')
have_library('LuminoEngine')

create_makefile('Lumino_RubyExt')
