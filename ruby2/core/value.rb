#
# value.rb
#

def Value(value)
  return Value.new(value)
end

class Value

  attr :frame, true
  attr :float, true

  def initialize(value)
    case value
    when Numeric, TrueClass, FalseClass
      @frame, @float = NumberValue.instance.frame, value
    when Symbol
      @frame, @float = SymbolValue.instance.frame, value
    when Value
      @frame, @float = value.frame, value.float
    when String
      @frame, @float = StringProto.instance.frame.create(value).frame, nil
    else
      @frame = value
    end
  end

  def proto()
    return @frame.proto
  end

  def frame_locals()
    return @frame.frame_locals
  end

  def _methods()
    return @frame._methods
  end

  def eval_(receiver, args, locals)
    trace "Value::eval()"
    
    if @frame == NumberValue.instance.frame
      return self
    end

    return @frame.eval_(receiver, args, locals)
  end

  def value()
    if @frame == NumberValue.instance.frame
      return self.float
    end
    
    return self
  end

  def send_(selector, args)
    return @frame.send_(selector, self, args)
  end

  def set_local(symbol, value)
    return @frame.set_local(symbol, value)
  end
  
  def get_local(symbol)
    return @frame.get_local(symbol)
  end

  def find_local(symbol)
    return @frame.find_local(symbol)
  end

  def add_method(symbol, value)
    return @frame.add_method(symbol, value)
  end
  
  def get_method(symbol)
    return @frame.get_method(symbol)
  end

  def to_s()
    return @frame.frame_to_s(self)
  end

  def inspect()
    return @frame.frame_inspect(self)
  end

end

