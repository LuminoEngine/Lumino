require 'lumino'
include Lumino

class App < Application
  def on_init()
    @texture1 = Texture2D.load("logo.png")
    @sprite1 = Sprite.new(@texture1, 2, 2)
  end

  def on_update()
    # Call per frames.
  end
end

App.new.run
