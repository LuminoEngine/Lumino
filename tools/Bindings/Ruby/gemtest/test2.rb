
$LUMINO_LOG_LEVEL = 2 # >= Debug
require 'lumino'
include Lumino

#EngineSettings.set_engine_log_enabled(true)
#Engine.initialize

class App < Application
    def on_init()
        #@callback = TestDelegate.new do
        #    p "called!!!!!!!!!!!!!!!!!!!!1"
        #end

        @texture1 = Texture2D.load("logo.png")
        @sprite1 = Sprite.new(@texture1, 2, 2)
        #@sprite1.set_caller_test(@callback)
        p "ccccc"
        @sprite1.texture = @texture1;
        p "aaaaaaaaaaaaaaaa"

        Engine.world.add(@sprite1)
    end

    def on_update()
        #p "on_update"
    end
end

App.new.run

#Lumino::Application.run_app_internal(App.new)

#texture1 = Texture2D.load("logo.png")
#sprite1 = Sprite.new(texture1, 2, 2)

#while Engine.update do

#end
