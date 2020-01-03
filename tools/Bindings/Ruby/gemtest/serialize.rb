require 'lumino'

include Lumino

Engine.initialize



class MySprite < Sprite
    def on_serialize(ar)
        p ar
        #ar.write_int("test", 10)
        p "on_serialize"
    end
end

p "a1===================="

p "a===================="

texture1 = Texture2D.load("logo.png")
p "a2===================="
sprite1 = MySprite.new(texture1, 2, 2)

asset = AssetModel.new(sprite1)

Assets.save_asset_to_local_file(asset, "test.json")


p "b===================="
Engine.update
p "c===================="
Engine.finalize

# a = Lumino::TestSprite.new

# p a

