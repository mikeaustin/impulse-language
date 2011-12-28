#
# core/protos/number.rb
#

require './core/frame.rb'
require './core/protos/object.rb'


class NumberProto < Frame

  def self.instance()
    @instance ||= NumberProto.new(ObjectProto.instance)

    @instance.add_method2(:"+", [@instance])    { |receiver, args| Value(receiver.float + args[0].float) }
    @instance.add_method2(:"-", [@instance])    { |receiver, args| Value(receiver.float - args[0].float) }
    @instance.add_method2(:"*", [@instance])    { |receiver, args| Value(receiver.float * args[0].float) }
    @instance.add_method2(:"/", [@instance])    { |receiver, args| Value(receiver.float / args[0].float.to_f) }
    @instance.add_method2(:"%", [@instance])    { |receiver, args| Value(receiver.float % args[0].float) }

    @instance.add_method2(:"<", [@instance])    { |receiver, args| Value(receiver.float < args[0].float) }
    @instance.add_method2(:">", [@instance])    { |receiver, args| Value(receiver.float > args[0].float) }
    @instance.add_method2(:"==", [@instance])   { |receiver, args| Value(receiver.float == args[0].float) }
    @instance.add_method2(:"!=", [@instance])   { |receiver, args| Value(receiver.float != args[0].float) }
    @instance.add_method2(:">=", [@instance])   { |receiver, args| Value(receiver.float >= args[0].float) }
    @instance.add_method2(:"<=", [@instance])   { |receiver, args| Value(receiver.float <= args[0].float) }

    @instance.add_method2(:"even", [])          { |receiver, args| Value(receiver.float % 2 == 0) }
    @instance.add_method2(:"odd", [])           { |receiver, args| Value(receiver.float % 2 != 0) }

    @instance.add_method2(:"@", [@instance])    { |receiver, args| PointValue.instance.frame._clone(receiver, args[0]) }

    @instance.add_method2(:"sin", [])           { |receiver, args| Value(Math.sin(receiver.float)) }
    @instance.add_method2(:"cos", [])           { |receiver, args| Value(Math.cos(receiver.float)) }
    @instance.add_method2(:"pow:", [@instance]) { |receiver, args| Value(receiver.float ** args[0].float) }
    @instance.add_method2(:"sqrt", [])          { |receiver, args| Value(Math.sqrt(receiver.float)) }

    return @instance
  end

  def frame_to_s(value)
    return value.float.to_s
  end

  def frame_inspect(value)
    if value.frame == NumberProto.instance.frame
      return "<number>"
    end
    
    return value.float.inspect
  end

end


class NumberValue < NumberProto

  def self.instance()
    return @instance ||= NumberValue.new(NumberProto.instance)
  end
  
end

