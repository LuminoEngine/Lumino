require 'Win32API'
AddDllDirectory = Win32API.new("kernel32", "AddDllDirectory", %w(p), "p")
AddDllDirectory.call(__dir__.encode('UTF-16LE'))



require './Lumino'
#require_relative 'Lumino.so'

Lumino::Engine.initialize

while Lumino::Engine.update do
end

Lumino::Engine.finalize


