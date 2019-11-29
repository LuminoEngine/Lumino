

#require 'Win32API'
#AddDllDirectory = Win32API.new("kernel32", "AddDllDirectory", %w(p), "p")
#AddDllDirectory.call("C:/tools/ruby26/lib/ruby/gems/2.6.0/gems/lumino-0.9.0/ext/lumino_ruby_core".encode('UTF-16LE'))


require 'fiddle/import'
extend Fiddle::Importer
#dlload 'C:/tools/ruby26/lib/ruby/gems/2.6.0/gems/lumino-0.9.0/ext/lumino_ruby_core/LuminoEngine.dll'
dlload __dir__ + "/../ext/lumino_ruby_core/LuminoEngine.dll"

require "lumino/version"

require "lumino_ruby_core/Lumino_RubyExt"

module Lumino
  class Error < StandardError; end
  # Your code goes here...

  class TestSprite
  end
end
