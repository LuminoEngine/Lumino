require 'test/unit'

$LUMINO_LOG_LEVEL = 2 # >= Debug
require 'lumino'
include Lumino


class Test_Engine < Test::Unit::TestCase

  class << self
    def startup
      Engine.initialize
    end
    def shutdown
      p "call Engine.finalize"
      Engine.terminate
    end
  end


  def callback1(v_int)
    @value = v_int
  end

  def callback2(a, b)
    @value = a + b
    return @value
  end

  def callback3(internal_obj)
    @value = internal_obj.class.name
  end

  def test_delegate_object
    delegate1 = ZVTestDelegate1.new(method(:callback1))
    delegate2 = ZVTestDelegate2.new(method(:callback2))
    delegate3 = ZVTestDelegate3.new(method(:callback3))

    class1 = ZVTestClass1.new
    class1.set_test_delegate_1(delegate1)
    class1.set_test_delegate_2(delegate2)
    class1.set_test_delegate_3(delegate3)

    @value = 0
    class1.call_test_delegate_1(5)
    assert_equal(5, @value)
    
    @value = 0
    r = class1.call_test_delegate_2(10, 20)
    assert_equal(30, @value)
    assert_equal(30, r)

    @value = nil
    class1.call_test_delegate_3()
    assert_equal("Lumino::ZVTestClass1", @value)
  end

  def test_delegate_object_block
    delegate1 = ZVTestDelegate1.new do |a|
      @value = a + 10
    end

    class1 = ZVTestClass1.new
    class1.set_test_delegate_1(delegate1)

    @value = 0
    class1.call_test_delegate_1(5)
    assert_equal(15, @value)
  end

  def test_promise
    @value = nil
    promise1 = ZVTestClass1.load_async("test")

    #d = ZVTestDelegate3.new do |obj|
    #  @value = obj
    #end
    #promise1.then_with(d)
    promise1.then_with do |obj|
      @value = obj
    end

    while @value == nil do
      Engine.update
    end

    assert_equal("Lumino::ZVTestClass1", @value.class.name)
  end

end



