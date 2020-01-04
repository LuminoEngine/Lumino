
$LUMINO_LOG_LEVEL = 2 # >= Debug
require 'lumino'
include Lumino

Engine.initialize

class TestSample < Test::Unit::TestCase
    def test_basic1
        p "------------ test"
    end
end


Engine.update
Engine.finalize

