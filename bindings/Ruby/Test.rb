require './Lumino'
include Lumino

def assert_eq(expected, actual)
  if expected != actual
    raise "Failed test."
  end
end

#---------------------------------------------------------------
def test_new
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
def test_constructor_overload
  tex1 = Texture2D.new(32, 32)
  
  # 実行エラーになら名ければOK
  spr1 = Sprite2D.new()
  spr2 = Sprite2D.new(tex1)
end

#---------------------------------------------------------------
def test_get_set_value

end

#---------------------------------------------------------------
def test_get_set_struct

end

#---------------------------------------------------------------
def test_get_set_object
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

# 実行
p "test_new"
test_new

p "test_constructor_overload"
test_constructor_overload

p "test_get_set_object"
test_get_set_object

# TODO: GC のテストするなら
#GC.stress = true	# http://www.slideshare.net/tarui/ss-23517958

p "Test success."

