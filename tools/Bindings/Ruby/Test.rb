require 'Win32API'
AddDllDirectory = Win32API.new("kernel32", "AddDllDirectory", %w(p), "p")
AddDllDirectory.call(__dir__.encode('UTF-16LE'))
p __dir__


require './Lumino'
#require 'C:/Proj/LN/Lumino/src/Bindings/Ruby/Lumino.so'
#require_relative 'Lumino.so'
include Lumino



#ev = Event_Void.new
def on_call(e)
    p "call on....."
    p e
end
#ev.add(method(:on_call))
#ev.raise

Engine.initialize

=begin
mainView = Engine.main_ui_view
p mainView

button1 = UIButton.new
button1.set_text("Hello, Ruby!")
button1.connect_on_clicked(method(:on_call))
button1.connect_on_clicked do |e|
    p "call on block."
    p e
end
mainView.add_child(button1)
=end


def test
    button2 = UIButton.new
    button2 = nil
    p GC.start  # => nil
    sleep 1

    button3 = UIButton.new
    button3 = nil
    p GC.start  # => nil
    sleep 1
    
    button4 = UIButton.new
    button4 = nil

    p GC.count  # => 4
    p GC.start
    sleep 1
    p GC.count
    p GC.start
    sleep 1
    p GC.count
    p GC.start
    sleep 1
    p GC.count
    p GC.start
    sleep 1
    p GC.count
end

test
p GC.start
sleep 1
p GC.count
p GC.start
sleep 1
p GC.count
p "============="
Engine.finalize

=begin
texture1 = Texture2D.new("D:/tmp/lnpoi.png")
sprite1 = Sprite.new(texture1, 2, 2)


list = sprite1.components
p list
p list.get_length
p list.get_item(0)


while Engine.update do
end

p ">>> Engine.finalize s"
Engine.finalize
p ">>> Engine.finalize e"

=end
