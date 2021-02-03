require "lumino"

class App < Application
    def on_init
        box = BoxMesh.new(1, 1, 1)
        box.add_into
    end
end

App.new.run