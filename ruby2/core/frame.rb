#
# core/frame.rb
#


class Hash

  attr :parent, true
  
  def find(symbol)
    hash = self

    #begin
    #  value = hash[symbol]
    #  hash  = hash.parent
    #end while value == nil && hash != nil

    while hash && !hash.has_key?(symbol)
      value = hash[symbol]
      hash  = hash.parent
    end
    
    if hash && hash.has_key?(symbol)
      return hash[symbol]
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

  def equal(other)
    return Value(other && self == other)
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
      puts "*** Runtime error: ##{symbol} not found in current scope"
    end
  end

  def add_method(symbol, value)
    @frame_methods[symbol] = value
    
    return nil
  end

  def add_method2(symbol, arg_types, &block)
    @frame_methods[symbol] = FunctionProto(block, arg_types)
    
    return nil
  end

  def get_method(symbol)
    return @frame_methods[symbol]
  end

  def find_method(symbol)
    return frame_methods.find(symbol)
  end

  def send_(selector, receiver, args)
    block = find_method(selector)
    
    if block
      return block.frame._call(receiver, args)
    else
      puts "*** Runtime error: #{receiver.inspect} does not respond to ##{selector}"
    end
    
    return nil
  end

  def eval_(receiver, args, locals)
    return Value(self)
  end
  
end

