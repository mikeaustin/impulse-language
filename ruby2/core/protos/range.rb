#
# core/protos/range.rb
#

require './core/protos/object.rb'


class RangeProto < Frame

  attr :range, true

  def self.instance
    @instance ||= self.new(ObjectProto.instance)

    @instance.add_method2(:"create:", [NumberProto.instance, NumberProto.instance]) \
                                      { |receiver, args| receiver.frame.create(args[0], args[1]) }
    @instance.add_method2(:"map:",    [BlockProto.instance])   { |receiver, args| receiver.frame.map(args[0]) }
    
    return @instance
  end

  def create(first, last)
    object = self.class.new(Value(self))
    
    object.frame.range = first.float .. last.float

    return object
  end

  def frame_inspect(value)
    if value.frame == RangeProto.instance.frame
      return "<range>"
    end

    return "#{range.inspect}"
  end

  def map(block)
    result = self.range.map do |item|
      block.frame._call(block, [Value(item)])
    end
    
    return ArrayProto.instance.frame.create(result)
  end

end

