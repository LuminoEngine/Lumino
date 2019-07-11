require 'Win32API'
AddDllDirectory = Win32API.new("kernel32", "AddDllDirectory", %w(p), "p")
AddDllDirectory.call(__dir__.encode('UTF-16LE'))
p __dir__


require './Lumino'
#require 'C:/Proj/LN/Lumino/src/Bindings/Ruby/Lumino.so'
#require_relative 'Lumino.so'
include Lumino

Engine.initialize

texture1 = Texture2D.new("D:/tmp/lnpoi.png", TextureFormat::RGBA8)
sprite1 = Sprite.new(texture1, 2, 2)

while Engine.update do
end

Engine.finalize


