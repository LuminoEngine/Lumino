
require 'lumino'
include Lumino

EngineSettings.set_engine_log_enabled(true)
Engine.initialize

texture1 = Texture2D.load("logo.png")
sprite1 = Sprite.new(texture1, 2, 2)

while Engine.update do

end