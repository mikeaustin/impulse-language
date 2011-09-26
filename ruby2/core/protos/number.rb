
class NumberProto < Frame

  def self.instance()
    return @instance ||= NumberProto.new()
  end

  def initialize()
    super(ObjectProto.instance)

    self.set_slot(:add, FunctionProto.new(self.method(:add_)))
    self.set_slot(:sin, FunctionProto.new(self.method(:sin)))
    self.set_slot(:pow, FunctionProto.new(self.method(:pow_)))
  end

  def add_(receiver, args, locals)
    return Value.new(receiver.float + args[0].float)
  end

  def sin(receiver, args, locals)
    return Value.new(Math.sin(receiver.float))
  end

  def pow_(receiver, args, locals)
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
  
end

