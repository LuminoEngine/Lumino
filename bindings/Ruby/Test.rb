require './lnote'

=begin
class Vec3
  def x
    return 10;
  end
  
  
  def x=(v)
    p "set"
  end
end

class ReadOnlyVec3 < Vec3

  private
  def x=(v)
    p "set"
  end
end

v = Vec3.new;
p v.x
v.x = 20

v = ReadOnlyVec3.new;
p v.x
#v.x = 20

Manager.initialize

#Manager.test(1)
#Manager.test(1, 2)
=end

module TEST
  TEST_VALUE = 10
end

p "start3"

vec1 = LN::Vector2.new()
p vec1.x
p vec1.y

vec2 = LN::Vector2.new(1.0, 2.2)
p vec2.x
p vec2.y
vec2.y = 123.987
p vec2.y

size1 = LN::Size.new(1.0, 2.2)
p size1.width
p size1.height
size1.width = 100
p size1.width

p LN
p LN::Config
#p LNote::Config.set_console_enabled
t = true
#LNote::Config.set_console_enabled(TEST::TEST_VALUE)
#LNote::Config.set_console_enabled(t)

# ツクールライクなトランジションのテスト

# 初期化
LN::Manager.initialize

# 音声再生
sound1 = LN::Sound.new("./ZIGG-ZAGG.mp3")
sound1.set_pitch(108)
sound1.play

# スプライト
tex1 = LN::Texture.new("./20140816_1.png")
sprite1 = LN::Sprite.new(tex1)

# レイヤーリストにトランジションフィルタを追加
transition = LN::TransitionFilter.new()
view_pane = LN::ViewPane.get_default_view_pane
layer_list = view_pane.get_layers
layer_list.push(transition)

# トランジション開始
transitionTex = LN::Texture.new("./Transition1.png")
transition.freeze
transition.transition(600, transitionTex, 40)

# メインループ
begin
  LN::Manager.update
end while !LN::Manager.end_requested?

