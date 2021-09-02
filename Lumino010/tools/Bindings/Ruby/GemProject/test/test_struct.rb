require 'test/unit'

$LUMINO_LOG_LEVEL = 2 # >= Debug
require 'lumino'
include Lumino

#Engine.initialize

class Test_Struct < Test::Unit::TestCase

  class << self
    def startup
      p :_startup
    end
    def shutdown
      p :_shutdown
    end
  end

  def test_constructor_and_accessor
    v1 = Vector3.new    # zeros
    assert_equal(0.0, v1.x)
    assert_equal(0.0, v1.y)
    assert_equal(0.0, v1.z)

    v1.x = 1.0
    v1.y = 10.0
    v1.z = 100.0
    assert_equal(1.0, v1.x)
    assert_equal(10.0, v1.y)
    assert_equal(100.0, v1.z)

    v2 = Vector3.new(1.0, 10.0, 100.0)
    assert_equal(1.0, v2.x)
    assert_equal(10.0, v2.y)
    assert_equal(100.0, v2.z)
  end

end


#Engine.update
#Engine.finalize

