
class Hash

  attr :parent, true
  
  def find(symbol)
    hash = self
    
    begin
      value = hash[symbol]
      hash  = hash.parent
    end while value == nil && hash != nil
    
    if value
      return value
    else
      raise "Slot not found: " + self.class.name + "." + symbol.to_s
    end

    return nil
  end

end


class Frame < Object

  attr :proto, true
  attr :slots, true
  attr :_methods, true

  def self.new(*args)
    result = super(*args)
    
    return Value.new(result)
  end

  def initialize(proto = nil)
    @proto, @slots, @_methods = proto, {}, {}
    
    if @proto != nil
      @slots.parent = @proto.slots
    else
      @slots.parent = nil
    end
  end

  def init_slots()
  end

  def set_slot(symbol, value)
    return @slots[symbol] = value
  end

  def get_slot(symbol)
    return @slots[symbol]
  end

  def find_slot(symbol)
    #return self.find_symbol(:slots, symbol)
    return @slots.find(symbol)
  end

  def add_method(symbol, value)
    return @_methods[symbol] = value
  end

  def get_method(symbol)
    return @_methods[symbol]
  end

  def xsend_(selector, receiver, args)
    method = find_symbol(:_methods, selector)
    
    if method
      return method.eval_(receiver, args, nil)
    end
    
    return nil
  end

  def send_(selector, receiver, args)
    trace "Frame::send()"

    frame = receiver

    begin
      value = frame.get_method(selector)
      frame = frame.proto
    end while value == nil && frame != nil
    
    if value
      #return value.eval_(receiver, args, nil)
      return value.frame.call_(receiver, args)
    else
      raise "Slot not found: " + self.class.name + "." + selector.to_s
    end
    
    return nil
  end

  def eval_(receiver, args, locals)
    return Value.new(self)
  end
  
end

