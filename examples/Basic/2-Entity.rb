require "lumino"

class App < Application
    def on_init
        Engine.render_view.guide_grid_enabled = true
        Engine.camera.set_position(5, 5, -5)
        Engine.camera.look_at(0, 0, 0)
        
        box1 = BoxMesh.new
        box1.set_position(1, 0, 0)
        box1.add_into

        box2 = BoxMesh.new
        box2.set_position(0, 2, 0)
        box2.add_into

        box3 = BoxMesh.new
        box3.set_position(0, 0, 3)
        box3.add_into
    end
end

App.new.run
