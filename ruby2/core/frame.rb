#
# core/frame.rb
#


class XHash

  attr :parent, true
  
  def find_hash(symbol)
    hash = self

    while hash && !hash.has_key?(symbol)
      value = hash[symbol]
      hash  = hash.parent
    end
    
    if hash && hash.has_key?(symbol)
      return hash
    end

    return nil
  end

end


class Frame < Object

  attr :frame_proto, true
  attr :frame_locals, true
  attr :frame_methods, true
  attr :frame_modules, true

  def self.new(*args)
    result = super(*args)
    
    return Value(result)
  end

  def initialize(proto = nil)
    @frame_proto, @frame_modules, @frame_locals, @frame_methods = proto, {}, {}, {}
  end

  def frame_inspect(value)
    return self.inspect() + @frame_locals.to_s()
  end
  
  def frame_to_s(value)
    return self.to_s()
  end

  def equal(other)
    return Value(other && self == other)
  end

  def frame_is_a(proto)
    self.frame_proto.frame == proto.frame
  end

  def find_symbol(symbol, hash_name)
    frame = self
    
    while frame && !frame.send(hash_name).has_key?(symbol)
      value = frame.send(hash_name)[symbol]
      frame = frame.frame_proto
    end
    
    if frame && frame.send(hash_name).has_key?(symbol)
      return frame
    end
    
    return nil
  end


  def set_local(symbol, value)
    frame = find_symbol(symbol, :frame_locals)
    
    if frame
      return frame.frame_locals[symbol] = value
    end
    
    return @frame_locals[symbol] = value
  end

  def get_local(symbol)
    return @frame_locals[symbol]
  end

  def find_local(symbol)
    frame = find_symbol(symbol, :frame_locals)
    
    if frame
      return frame.frame_locals[symbol]
    else
      puts "*** Runtime error: ##{symbol} not found in current scope"
    end
    
    return nil
  end


  def add_module(symbol, value)
    @frame_modules[symbol] = value
    
    return nil
  end

  def find_module(symbol)
    frame = find_symbol(symbol, :frame_modules)
    
    if frame
      return frame.frame_modules[symbol]
    end
    
    return nil
  end


  def add_method(symbol, value)
    @frame_methods[symbol] = value
    
    return nil
  end

  def add_method2(symbol, arg_types, arg_names = [], summary_doc = nil, &block)
    @frame_methods[symbol] = FunctionProto(block, arg_types)
    @frame_methods[symbol].frame.summary_doc = summary_doc
    @frame_methods[symbol].frame.arg_names = arg_names
    
    return nil
  end

  def get_method(symbol)
    return @frame_methods[symbol]
  end

  def find_method(symbol)
    frame = find_symbol(symbol, :frame_methods)

    if frame
      return frame.frame_methods[symbol]
    end
    
    return nil
  end


  def send_(selector, receiver, args, locals)
    block = find_method(selector) || locals.find_module(:"<string>").frame.find_method(selector)
    
    if block
      return block.frame._call(receiver, args, receiver)
    else
      puts "*** Runtime error: #{receiver.inspect} does not respond to ##{selector}"
    end
    
    return nil
  end

  def eval_(receiver, args, locals)
    return Value(self)
  end
  
end

