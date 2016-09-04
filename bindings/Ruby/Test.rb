require './Lumino'
include Lumino

def assert_eq(expected, actual)
  if expected != actual
    raise "Failed test."
  end
end

#---------------------------------------------------------------
def test_struct
  # デフォルトコンストラクタ
  vec1 = Vector3.new
  assert_eq(0, vec1.x)
  assert_eq(0, vec1.y)
  assert_eq(0, vec1.z)

  # フィールド初期値指定のコンストラクタ
  vec2 = Vector3.new(1, 2, 3)
  assert_eq(1, vec2.x)
  assert_eq(2, vec2.y)
  assert_eq(3, vec2.z)

  # フィールドへの代入
  vec2.z = 100
  assert_eq(100, vec2.z)

  # インスタンスメソッドの呼び出し
  vec2.normalize
  assert_eq(true, vec2.x > 0.009)
  assert_eq(true, vec2.y > 0.01)
  assert_eq(true, vec2.z > 0.999)

  # クラスメソッドの呼び出し（インスタンスメソッドとのオーバーライド）
  vec3 = Vector3.normalize(vec2)
  assert_eq(true, vec3.x > 0.009)
  assert_eq(true, vec3.y > 0.01)
  assert_eq(true, vec3.z > 0.999)
end


#---------------------------------------------------------------
def test_refobject_constructor_overload
  tex1 = Texture2D.new(32, 32)

  # 実行エラーになら名ければOK
  spr1 = Sprite2D.new()
  spr2 = Sprite2D.new(tex1)
end

#---------------------------------------------------------------
def test_refobject_get_set_value
  spr1 = Sprite2D.new()

  # get
  assert_eq(true, spr1.visible?)

  # set
  spr1.visible = false
  assert_eq(false, spr1.visible?)
end

#---------------------------------------------------------------
def test_refobject_get_set_struct
  spr1 = Sprite2D.new()

  # get
  pos = spr1.position
  assert_eq(0.0, pos.x)
  assert_eq(0.0, pos.y)
  assert_eq(0.0, pos.z)

  # set
  spr1.position = Vector3.new(1.0, 2.0, 3.0)
  assert_eq(1.0, spr1.position.x)
  assert_eq(2.0, spr1.position.y)
  assert_eq(3.0, spr1.position.z)
end

#---------------------------------------------------------------
def test_refobject_get_set_object
  tex1 = Texture2D.new(32, 32)
  spr1 = Sprite2D.new()

  # set したもとの get で取得したものは同一
  spr1.texture = tex1;
  assert_eq(tex1, spr1.texture)

  # nil にしてみる
  spr1.texture = nil;
  assert_eq(nil, spr1.texture)

  # コンストラクタにセットしたものと同一
  spr2 = Sprite2D.new(tex1)
  assert_eq(tex1, spr2.texture)
end

#---------------------------------------------------------------
# 明示的破棄のテスト (TODO)
def test_dispose
end

#---------------------------------------------------------------
# ブジェクトリストのテスト
def test_object_list
  tex1 = Texture2D.new(32, 32)
  spr1 = Sprite2D.new(tex1)
  spr2 = Sprite2D.new(tex1)
  spr3 = Sprite2D.new(tex1)
  children = spr1.children;

  # とりあえず機能を呼び出してみる
  assert_eq(0, children.count);

  # 要素を追加する
  children.add(spr2);

  # 要素数が増えている
  assert_eq(1, children.count);

  # 要素を get してみる
  assert_eq(spr2, children[0]);

  # 要素を set してみる
  children[0] = spr3;

  # 要素を get してみる
  assert_eq(spr3, children[0]);

  # 要素を削除してみる
  children.remove_at(0);
  assert_eq(0, children.count);

  # foreach
=begin
  children.push(spr2);  # ruby 的には push があったほうがいいかな
  children.push(spr3);
  int i = 0;
  for item in children
    if (i == 0) assert_eq(spr2, item);
    if (i == 1) assert_eq(spr3, item);
    i++;
  end
=end

  # 別の変数に取り出してみる
  children2 = spr1.children;
  assert_eq(children, children2);
end

#---------------------------------------------------------------
# デフォルトで作成されるオブジェクトリストのテスト
def test_default_object_list
  vp1 = Viewport.main_viewport
  list1 = vp1.layers

  # 最初から何個か入っている
  assert_eq(true, list1.size > 0)  # Ruby 的には count じゃなくて size も使えたほうがいいかな。

  # 要素を get してみる
  l1 = list1[0]

  # もう一度 get してみる
  l2 = list1[0]
  assert_eq(l1, l2)
end

#---------------------------------------------------------------
# オーバーライド
class MySprite < Sprite2D
  def on_render
    p "dd onrender"
  end
end
def test_method_override
  spr1 = MySprite.new
  Engine.update_frame
end

Engine.initialize

# 実行
p "test_struct"
test_struct

p "test_refobject_constructor_overload"
test_refobject_constructor_overload

p "test_refobject_get_set_value"
test_refobject_get_set_value

p "test_refobject_get_set_struct"
test_refobject_get_set_struct

p "test_refobject_get_set_object"
test_refobject_get_set_object

p "test_object_list"
test_object_list

p "test_default_object_list"
test_default_object_list

p "test_method_override"
test_method_override

# TODO: GC のテストするなら
#GC.stress = true	# http://www.slideshare.net/tarui/ss-23517958

p "Test succeeded."
