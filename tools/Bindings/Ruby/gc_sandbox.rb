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

def gc_test
    view = Engine.main_ui_view
    p "view 1: "
    p view
    view = nil
end

def gc_test2
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


def gc_test3
    p "gc_test3 ============="
    view1 = Engine.main_ui_view
    #p view1
    view1 = nil
    p GC.start  # => nil
    sleep 1

    view2 = Engine.main_ui_view
    #p view2
    view2 = nil
    p GC.start  # => nil
    sleep 1

    view3 = Engine.main_ui_view
    #p view3
    view3 = nil
    p GC.start  # => nil
    sleep 1
end

=begin
for i in 0..10
    gc_test
    GC.start
    sleep(1)
end


p GC.start
sleep 1
p GC.count
p GC.start
sleep 1
p GC.count
=end


#gc_test2
gc_test3


view3 = Engine.main_ui_view
p view3
view3 = nil
GC.start  # => nil
sleep 1

p "============="



mainView = Engine.main_ui_view


p '-- 1'
button1 = UIButton.new
p '-- 2'
button1.set_text("Hello, Ruby!")
p '-- 3'
button1.connect_on_clicked(method(:on_call))
button1.connect_on_clicked do |e|
    p "call on block."
    p e
end

p '-- 4'
mainView.add_child(button1)

p '-- 5'




texture1 = Texture2D.new("D:/tmp/lnpoi.png")
p '-- 6'
sprite1 = Sprite.new(texture1, 2, 2)
p '-- 7'


list = sprite1.components
p list
p list.get_length
p list.get_item(0)


while Engine.update do
end

Engine.finalize


