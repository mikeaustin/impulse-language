
class Frame

  attr :proto, true
  attr :slots, true

  def self.new(*args)
    result = super(*args)
    
    return Value.new(result)
  end

  def initialize(proto)
    @proto, @slots, @methods = proto, {}, {}
  end

  def init_slots()
  end

  def set_slot(symbol, value)
    return @slots[symbol] = value
  end

  def get_slot(symbol)
    return @slots[symbol]
  end

  def send_(selector, receiver, args)
    trace "Frame::send()"

    frame = receiver

    begin
      value = frame.get_slot(selector)
      frame = frame.proto
    end while value == nil && frame != nil
    
    if value
      return value.eval_(receiver, args, nil)
    else
      puts "Slot not found: " + self.class.name + " " + selector.to_s
    end
    
    return nil
  end

  def eval_(receiver, args, locals)
    return Value.new(self)
  end
  
end

