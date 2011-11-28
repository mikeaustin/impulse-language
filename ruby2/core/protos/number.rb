#
# core/protos/number.rb
#

require './core/frame.rb'

class NumberProto < Frame

  def self.instance()
    @instance ||= NumberProto.new(ObjectProto.instance)

    @instance.add_method(:add, FunctionProto(@instance.frame.method(:add_)))
    @instance.add_method(:sin, FunctionProto(@instance.frame.method(:sin)))
    @instance.add_method(:cos, FunctionProto(@instance.frame.method(:cos)))
    @instance.add_method(:pow, FunctionProto(@instance.frame.method(:pow_)))
    
    return @instance
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

  def frame_inspect(value)
    return Value("<number>")
  end

end


class NumberValue < Frame

  def self.instance()
    return @instance ||= NumberValue.new(NumberProto.instance)
  end
  
  def frame_to_s(value)
    return value.float.to_s
  end
  
  def frame_inspect(value)
    return value.float.inspect
  end
  
end

