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

if save

    p "a1===================="

    p "a===================="

    texture1 = Texture2D.load("logo.png")
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

