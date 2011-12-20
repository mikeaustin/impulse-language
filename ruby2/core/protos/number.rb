#
# core/protos/number.rb
#

require './core/frame.rb'

class NumberProto < Frame

  def self.instance()
    @instance ||= NumberProto.new(ObjectProto.instance)

    @instance.add_method(:+,    FunctionProto(@instance.frame.method(:add_), [@instance]))
    @instance.add_method(:-,    FunctionProto(@instance.frame.method(:sub_)))
    @instance.add_method(:*,    FunctionProto(@instance.frame.method(:mul_)))
    @instance.add_method(:/,    FunctionProto(@instance.frame.method(:div_)))
    @instance.add_method(:sin,  FunctionProto(@instance.frame.method(:sin)))
    @instance.add_method(:cos,  FunctionProto(@instance.frame.method(:cos)))
    @instance.add_method(:pow,  FunctionProto(@instance.frame.method(:pow_)))
    @instance.add_method(:sqrt, FunctionProto(@instance.frame.method(:_sqrt)))

    return @instance
  end

  def frame_inspect(value)
    return Value("<number>")
  end

  def add_(receiver, args)
    return Value(receiver.float + args[0].float)
  end

  def sub_(receiver, args)
    return Value(receiver.float - args[0].float)
  end

  def mul_(receiver, args)
    return Value(receiver.float * args[0].float)
  end

  def div_(receiver, args)
    return Value(receiver.float / args[0].float)
  end

  def sin(receiver, args)
    return Value(Math.sin(receiver.float))
  end

  def cos(receiver, args)
    return Value(Math.cos(receiver.float))
  end

  def pow_(receiver, args)
    if args[0].proto.frame != NumberProto.instance.frame
      puts "Argument not a number"
      
      return nil
    end

    return Value(receiver.float ** args[0].float)
  end

  def _sqrt(receiver, args)
    return Value(Math.sqrt(receiver.float))
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

