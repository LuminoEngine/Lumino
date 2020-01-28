
$LUMINO_LOG_LEVEL = 2 # >= Debug
require 'lumino'
include Lumino

class App < Application
    def on_init
        p "🐈"
        texture = Texture2D.load_emoji("🐈")
        p "fshgf"
        
        sprite1 = Sprite.new(texture, 2, 2)

        Engine.world.add(sprite1)
    end

    def on_update
        #p "on_update"
    end
end

App.new.run

p "endddddddd"
