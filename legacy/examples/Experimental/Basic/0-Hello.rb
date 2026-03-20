require "lumino"

class App < Application
  def on_init
    text = UIText.new("Hello, Lumino!")
    text.add_into
  end
end

App.new.run
