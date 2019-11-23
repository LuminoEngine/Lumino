require 'Win32API'
AddDllDirectory = Win32API.new("kernel32", "AddDllDirectory", %w(p), "p")
AddDllDirectory.call(__dir__.encode('UTF-16LE'))


require './Lumino'
include Lumino


class MySprite < Sprite
    def initialize(texture, w, h)
        super(texture, w, h)
    end

    def on_update(elapsed_seconds)
        super(elapsed_seconds)
        p elapsed_seconds
    end
end



Engine.initialize

texture1 = Texture2D.new("D:/tmp/lnpoi.png")
sprite1 = MySprite.new(texture1, 2, 2)

while Engine.update do
end

Engine.finalize


