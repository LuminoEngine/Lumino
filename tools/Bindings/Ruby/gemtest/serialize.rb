
$LUMINO_LOG_LEVEL = 2 # >= Debug
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

save = false

texture1 = Texture2D.load("logo.png")

sprite1 = Sprite.new do |s|
    p "s init with block."
    p s
    p texture1
    s.texture=texture1
    p "init with block."
end
#p sprite1.texture

if save

    p "a1===================="

    p "a===================="

    p "a2===================="
    sprite1 = MySprite.new(texture1, 2, 2)

    asset = AssetModel.new(sprite1)

    Assets.save_asset_to_local_file(asset, "test.json")
else
    p "g===================="
    asset = Assets.load_asset_from_local_file("test.json")
    p "m===================="
    p asset
    p asset.target

end

p "b===================="
Engine.update
p "c===================="
Engine.finalize

# a = Lumino::TestSprite.new

# p a

