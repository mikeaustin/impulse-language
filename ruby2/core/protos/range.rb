#
# core/protos/range.rb
#

require './core/protos/object.rb'


class RangeProto < Frame

  attr :range, true
  attr :step, true

  def self.instance
    @instance ||= self.new(ObjectProto.instance)

    @instance.add_method2(:"create:", [NumberProto.instance, NumberProto.instance]) \
                                                              { |receiver, args| receiver.frame.create(args[0], args[1]) }
    @instance.add_method2(:"each:",   [BlockProto.instance])  { |receiver, args| receiver.frame.each(args[0], args[1]) }
    @instance.add_method2(:"map:",    [BlockProto.instance])  { |receiver, args| receiver.frame.map(args[0]) }
    @instance.add_method2(:"sample:", [NumberProto.instance]) { |receiver, args| receiver.frame.sample(args[0]) }

    @instance.add_method2(:"%",       [NumberProto.instance]) { |receiver, args| receiver.frame.step(args[0]) }
    
    return @instance
  end

  def create(first, last, step = nil)
    object = self.class.new(Value(self))
    
    object.frame.range = first.float .. last.float
    object.frame.step  = step && step.float || 1

    return object
  end

  def frame_inspect(value)
    if value.frame == RangeProto.instance.frame
      return "<range>"
    end

    return "#{range.inspect}#{" % " + @step.to_s if @step != 1}"
  end

  def each(block, step)
    step = step && step.float || @step
    
    self.range.step(step) do |item|
      block.frame._call(block, [Value(item)])
    end
    
    return nil
  end

  def map(block)
    result = self.range.map do |item|
      block.frame._call(block, [Value(item)])
    end
    
    return ArrayProto.instance.frame.create(result)
  end

  def sample(count)
    result = (1..count.float).map do |i|
      rand(self.range.last - self.range.first + 1) + self.range.first
    end
    
    return ArrayProto.instance.frame.create(result)
  end

  def step(step)
    self.step = step.float
    
    Value(self)
  end

end

