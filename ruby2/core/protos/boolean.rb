#
# core/protos/boolean.rb
#

require './core/frame.rb'
require './core/protos/object.rb'


class BooleanProto < Frame

  def self.instance
    @instance ||= BooleanProto.new(ObjectProto.instance)

    @instance.add_method2(:"==", [@instance])   { |receiver, args| Value(receiver.float == args[0].float) }
    @instance.add_method2(:"!=", [@instance])   { |receiver, args| Value(receiver.float != args[0].float) }
    @instance.add_method2(:"not", [])  { |receiver, args| Value(!receiver.float) }
    
    return @instance
  end

  def frame_inspect(value)
    if value.frame == BooleanProto.instance.frame
      return "<bool>"
    else
      return value.float.to_s
    end
  end

end


class BooleanValue < BooleanProto

  def self.instance
    @instance = BooleanValue.new(BooleanProto.instance)
    
    return @instance
  end

end

