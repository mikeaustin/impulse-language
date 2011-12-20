#
# core/frame.rb
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
  attr :frame_locals, true
  attr :frame_methods, true

  def self.new(*args)
    result = super(*args)
    
    return Value(result)
  end

  def initialize(proto = nil)
    @proto, @frame_locals, @frame_methods = proto, {}, {}
    
    if @proto != nil
      @frame_locals.parent = @proto.frame_locals
      @frame_methods.parent = @proto.frame_methods
    else
      @frame_locals.parent = nil
      @frame_methods.parent = nil
    end
  end

  def frame_inspect(value)
    return self.inspect() + @frame_locals.to_s()
  end
  
  def frame_to_s(value)
    return self.to_s()
  end

  def init_slots()
  end

  def set_local(symbol, value)
    return @frame_locals[symbol] = value
  end

  def get_local(symbol)
    return @frame_locals[symbol]
  end

  def find_local(symbol)
    value = @frame_locals.find(symbol)
    
    if value
      return value
    else
      raise "Local not found: " + self.class.name + "." + symbol.to_s
    end
  end

  def add_method(symbol, value)
    return @frame_methods[symbol] = value
  end

  def get_method(symbol)
    return @frame_methods[symbol]
  end

  def find_method(symbol)
    return frame_methods.find(symbol)
  end

  def send_(selector, receiver, args)
    method = find_method(selector)
    
    if method
      return method.frame._call(receiver, args)
    else
      raise "*** Send failed: #{receiver}.#{selector}"
    end
    
    return nil
  end

  def eval_(receiver, args, locals)
    return Value(self)
  end
  
end

