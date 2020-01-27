
$LUMINO_LOG_LEVEL = 2 # >= Debug
require 'lumino'
include Lumino

class App < Application
    def on_init
        p "🐈"
        texture = Texture2D.load_emoji("🐈")
        
        @sprite1 = Sprite.new(@texture1, 2, 2)
        @sprite1.texture = @texture1;

        Engine.world.add(@sprite1)
    end

    def on_update
        #p "on_update"
    end
end

App.new.run

