require 'lumino'

include Lumino

Engine.initialize

button1 = UIButton.new

# プロパティの set は = 演算子で行う
button1.text = "test";

# bool を返すプロパティの get は ? サフィックスになる
t = button1.visible?



Engine.update
Engine.finalize

# a = Lumino::TestSprite.new

# p a

