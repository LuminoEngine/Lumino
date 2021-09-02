require 'Win32API'
AddDllDirectory = Win32API.new("kernel32", "AddDllDirectory", %w(p), "p")
AddDllDirectory.call(__dir__.encode('UTF-16LE'))
p __dir__


require './Lumino'
#require 'C:/Proj/LN/Lumino/src/Bindings/Ruby/Lumino.so'
#require_relative 'Lumino.so'
include Lumino




def on_call(e)
    p "call on....."
    p e
end

Engine.initialize

mainView = Engine.main_ui_view


button1 = UIButton.new
button1.set_text("Hello, Ruby!")
button1.connect_on_clicked(method(:on_call))
button1.connect_on_clicked do |e|
    p "call on block."
    p e
end

mainView.add_child(button1)





texture1 = Texture2D.new("logo.png")
sprite1 = Sprite.new(texture1, 2, 2)


list = sprite1.components
p list
p list.get_length
p list.get_item(0)

while Engine.update do
end

p "call Engine.finalize"
Engine.finalize
p "called Engine.finalize"


