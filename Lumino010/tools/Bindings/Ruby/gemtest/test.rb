$LUMINO_LOG_LEVEL = 2 # >= Debug
require "lumino"
EngineSettings.set_priority_gpu_name("Microsoft Basic Render Driver")

class App < Application
    def on_init
      Engine.render_view.guide_grid_enabled = true
      Engine.camera.set_position(5, 5, -5)
      Engine.camera.look_at(0, 0, 0)
      
      @box = BoxMesh.new
      @box.add_into
    end
    
    def on_update
      s = Mouse.position.y / 100
      
      @box.set_scale(s)
          
      Debug.print(0, "scale: %f" % s)
    end
  end
  
  App.new.run