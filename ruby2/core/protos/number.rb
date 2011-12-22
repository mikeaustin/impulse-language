#
# core/protos/number.rb
#

require './core/frame.rb'

class NumberProto < Frame

  def self.instance()
    @instance ||= NumberProto.new(ObjectProto.instance)

    @instance.add_method2(:"+", [@instance])   { |receiver, args| Value(receiver.float + args[0].float) }
    @instance.add_method2(:"-", [@instance])   { |receiver, args| Value(receiver.float - args[0].float) }
    @instance.add_method2(:"*", [@instance])   { |receiver, args| Value(receiver.float * args[0].float) }
    @instance.add_method2(:"/", [@instance])   { |receiver, args| Value(receiver.float / args[0].float.to_f) }
    @instance.add_method2(:"%", [@instance])   { |receiver, args| Value(receiver.float % args[0].float) }

    @instance.add_method2(:"<", [@instance])   { |receiver, args| Value(receiver.float < args[0].float) }
    @instance.add_method2(:">", [@instance])   { |receiver, args|
     result = receiver.float > args[0].float
     if result then Value(receiver.float) else NilProto.instance end
    }
    @instance.add_method2(:"==", [@instance])  { |receiver, args| Value(receiver.float == args[0].float) }
    @instance.add_method2(:">=", [@instance])  { |receiver, args| Value(receiver.float >= args[0].float) }
    @instance.add_method2(:"<=", [@instance])  { |receiver, args| Value(receiver.float <= args[0].float) }

    @instance.add_method2(:"@", [@instance])   { |receiver, args| PointValue.instance.frame._clone(receiver, args[0]) }

    @instance.add_method2(:"sin", [])          { |receiver, args| Value(Math.sin(receiver.float)) }
    @instance.add_method2(:"cos", [])          { |receiver, args| Value(Math.cos(receiver.float)) }
    @instance.add_method2(:"pow", [@instance]) { |receiver, args| Value(receiver.float ** args[0].float) }
    @instance.add_method2(:"sqrt", [])         { |receiver, args| Value(Math.sqrt(receiver.float)) }

    return @instance
  end

  def frame_to_s(value)
    return Value(value.float.to_s)
  end

  def frame_inspect(value)
    if value.frame == ObjectProto.instance.frame
      return Value("<number>")
    end
    
    return Value(value.float.inspect)
  end

end


class NumberValue < NumberProto

  def self.instance()
    return @instance ||= NumberValue.new(NumberProto.instance)
  end
  
end

