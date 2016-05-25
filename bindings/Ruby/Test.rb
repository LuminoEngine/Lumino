require 'test/unit'
require './Lumino'
include Lumino


#-------------------------------------------------------------------------------
class Test_Struct < Test::Unit::TestCase
  class << self
    # テスト群の実行前に呼ばれる
    def startup
      p :_startup
    end

    # テスト群の実行後に呼ばれる
    def shutdown
      p :_shutdown
    end
  end

  # 毎回テスト実行前に呼ばれる
  def setup
    p :setup
  end

  # テストがpassedになっている場合に，テスト実行後に呼ばれる．テスト後の状態確認とかに使える
  def cleanup
    p :cleanup
  end

  # 毎回テスト実行後に呼ばれる
  def teardown
    p :treadown
  end

  def test_new
    # デフォルトコンストラクタ
    vec1 = Vector3.new
    assert_equal(0, vec1.x)
    assert_equal(0, vec1.y)
    assert_equal(0, vec1.z)

    # フィールド初期値指定のコンストラクタ
    vec2 = Vector3.new(1, 2, 3)
    assert_equal(1, vec2.x)
    assert_equal(2, vec2.y)
    assert_equal(3, vec2.z)

    # フィールドへの代入
    vec2.z = 100
    assert_equal(100, vec2.z)

    # インスタンスメソッドの呼び出し
    vec2.normalize
    assert_equal(true, vec2.x > 0.009)
    assert_equal(true, vec2.y > 0.01)
    assert_equal(true, vec2.z > 0.999)

    # クラスメソッドの呼び出し（インスタンスメソッドとのオーバーライド）
    vec3 = Vector3.normalize(vec2)
    assert_equal(true, vec3.x > 0.009)
    assert_equal(true, vec3.y > 0.01)
    assert_equal(true, vec3.z > 0.999)
  end
end
