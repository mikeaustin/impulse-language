
class NumberProto < Frame

  def self.instance()
    return @instance ||= NumberProto.new()
  end

  def initialize()
    super(ObjectProto.instance)
    
    self.add_method(:add, FunctionProto.new(self.method(:add_)))
    self.add_method(:sin, FunctionProto.new(self.method(:sin)))
    self.add_method(:pow, FunctionProto.new(self.method(:pow_)))
  end

  def add_(receiver, args)
    return Value.new(receiver.float + args[0].float)
  end

  def sin(receiver, args)
    return Value.new(Math.sin(receiver.float))
  end

  def pow_(receiver, args)
    return Value.new(receiver.float ** args[0].float)
  end

end


class NumberValue < Frame

  def self.instance()
    return @instance ||= NumberValue.new()
  end
  
  def initialize()
    super(NumberProto.instance)
  end

  def to_s(value)
    return value.float.to_s
  end
  
  def inspect(value)
    return value.float.inspect
  end
  
end

