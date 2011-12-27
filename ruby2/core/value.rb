#
# core/value.rb
#

def Value(value)
  return Value.new(value)
end

class Value

  attr :frame, true
  attr :float, true
  attr :x, true
  attr :y, true

  def initialize(value)
    case value
    when Numeric
      @frame, @float = NumberValue.instance.frame, value
    when TrueClass, FalseClass
      @frame, @float = BooleanValue.instance.frame, value
    when Symbol
      @frame, @float = SymbolValue.instance.frame, value
    when String
      @frame, @float = StringProto.instance.frame.create(value).frame, nil
    when Value
      @frame, @float = value.frame, value.float
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

  def frame_methods()
    return @frame.frame_methods
  end

  #def ==(other)
  #p self.frame.class
  #p other.frame.class if other
  #  return other && (@frame == other.frame)
  #end

  def eval_(receiver, args, locals)
    trace "Value::eval()"
    
    #if @frame == NumberValue.instance.frame
    if @float != nil
      return self
    end

    return @frame.eval_(receiver, args, locals)
  end

  def raw_value()
    if @frame == NumberValue.instance.frame
      return self.float
    end
    
    return self
  end

  def equal(other)
    return Value(@float == other.float && @frame.equal(other.frame))
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

  def add_method2(symbol, arg_types, &block)
    return @frame.add_method2(symbol, arg_types, &block)
  end
  
  def get_method(symbol)
    return @frame.get_method(symbol)
  end

  def to_s()
    return @frame && @frame.frame_to_s(self)
  end

  def inspect()
    return @frame && @frame.frame_inspect(self)
  end

end

