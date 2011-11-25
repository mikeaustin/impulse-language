#
# frame.rb
#

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
    #else
    #  raise "Slot not found: " + self.class.name + "." + symbol.to_s
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
      @_methods.parent = @proto._methods
    else
      @slots.parent = nil
      @_methods.parent = nil
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
    value = @slots.find(symbol)
    
    if value
      return value
    else
      raise "Slot not found: " + self.class.name + "." + symbol.to_s
    end
  end

  def add_method(symbol, value)
    return @_methods[symbol] = value
  end

  def get_method(symbol)
    return @_methods[symbol]
  end

  def find_method(symbol)
    return @_methods.find(symbol)
  end

  def send_(selector, receiver, args)
    method = find_method(selector)
    
    if method
      return method.frame.call_(receiver, args)
    else
      raise "Method not found: " + self.class.name + "." + selector.to_s
    end
    
    return nil
  end

  def xsend_(selector, receiver, args)
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
