require 'lumino'

include Lumino

Engine.initialize



class MySprite < Sprite
    def on_serialize
        p "on_serialize"
    end
end

p "a===================="

texture1 = Texture2D.load("logo.png")
p "a2===================="
sprite1 = MySprite.new(@texture1, 2, 2)

p "b===================="
Engine.update
p "c===================="
Engine.finalize

# a = Lumino::TestSprite.new

# p a

