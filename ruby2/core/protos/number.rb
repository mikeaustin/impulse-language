#
# core/protos/number.rb
#

require './core/frame.rb'

class NumberProto < Frame

  def self.instance()
    return @instance ||= NumberProto.new()
  end

  def initialize()
    super(ObjectProto.instance)
    
    self.add_method(:add, FunctionProto(self.method(:add_)))
    self.add_method(:sin, FunctionProto(self.method(:sin)))
    self.add_method(:cos, FunctionProto(self.method(:cos)))
    self.add_method(:pow, FunctionProto(self.method(:pow_)))
  end

  def add_(receiver, args)
    return Value(receiver.float + args[0].float)
  end

  def sin(receiver, args)
    return Value(Math.sin(receiver.float))
  end

  def cos(receiver, args)
    return Value(Math.cos(receiver.float))
  end

  def pow_(receiver, args)
    return Value(receiver.float ** args[0].float)
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

