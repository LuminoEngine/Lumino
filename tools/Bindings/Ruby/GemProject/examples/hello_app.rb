$LUMINO_LOG_LEVEL = 2 # >= Debug
require "lumino"

class App < Application

  def on_init
    texture = Texture2D.load_emoji("🐈")
    sprite1 = Sprite.new(texture, 2, 2)
    Engine.world.add(sprite1)
  end

  def on_update
  end

end

App.new.run

  