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
    def test_greeting
        Engine.initialize
        Engine.update
        Engine.finalize
    end
end
