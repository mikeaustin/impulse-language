
class Value

  attr :frame, true
  attr :float, true

  def initialize(value)
    case value
    when Numeric, TrueClass, FalseClass, Symbol
      @frame, @float = NumberValue.instance.frame, value
    when Value
      @frame = value.frame
    else
      @frame = value
    end
  end

  def proto()
    return @frame.proto
  end

  def eval_(receiver, args, locals)
    trace "Value::eval()"
    
    if @frame == NumberValue.instance.frame
      return self
    end
    
    return @frame.eval_(receiver, args, locals)
  end

  def send_(selector, args)
    return @frame.send_(selector, self, args)
  end

  def set_slot(symbol, value)
    return @frame.set_slot(symbol, value)
  end
  
  def get_slot(symbol)
    return @frame.get_slot(symbol)
  end

  def inspect()
    case @frame
    when NumberValue.instance.frame
      return "#{@float}"
    else
      return @frame.inspect()
    end
  end

  def to_s()
    case @frame
    when NumberValue.instance.frame
      return @float.to_s
    else
      return @frame.to_s
    end
  end

end

