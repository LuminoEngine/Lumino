require "lumino"

class App < Application
  def on_init
    text = UITextBlock.new("Hello, Lumino!")
  end

  def on_update
  end
end

App.new.run
