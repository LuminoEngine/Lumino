module Lumino
  class EventSignal
    def initialize
      @handlers = []
    end
    def add(handler)
      @handlers.push(handler)
    end
    def raise(*args)
      @handlers.each do |h|
        h.call(*args)
      end
    end
  end
end
