#--------------------
require 'Win32API'
AddDllDirectory = Win32API.new("kernel32", "AddDllDirectory", %w(p), "p")
AddDllDirectory.call(__dir__.encode('UTF-16LE'))
require './Lumino'
include Lumino
#--------------------

# ruby test.rb

require 'test/unit'

class TestSample < Test::Unit::TestCase
    def test_basic1
        Engine.initialize

        button1 = UIButton.new

        # プロパティの set は = 演算子で行う
        button1.text = "test";

        # bool を返すプロパティの get は ? サフィックスになる
        t = button1.visible?



        Engine.update
        Engine.finalize
    end
end
