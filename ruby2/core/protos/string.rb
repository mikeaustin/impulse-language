#
# core/protos/string.rb
#

require './core/frame.rb'

class StringProto < Frame

  attr :string, true

  def self.instance()
    @instance ||= StringProto.new(ObjectProto.instance)

    @instance.add_method(:size, FunctionProto(@instance.frame.method(:size_)))
    @instance.add_method(:capitalize, FunctionProto(@instance.frame.method(:capitalize)))
    @instance.add_method(:split, FunctionProto(@instance.frame.method(:_split)))
    @instance.add_method(:"++", FunctionProto(@instance.frame.method(:_concatenate)))

    return @instance
  end

  def create(string)
    object = StringProto.new(Value(self))
    
    object.frame.string = string
    
    return object
  end

  def frame_to_s(value)
    return value.frame.string
  end
  
  def frame_inspect(value)
    if value.frame == StringProto.instance.frame
      return Value("<string>")
    end
    
    return "\"#{value.frame.string}\""
  end

  def size_(receiver, args)
    return Value(receiver.frame.string.size)
  end

  def capitalize(receiver, args)
    return Value(receiver.frame.string[0].upcase + receiver.frame.string[1..-1])
  end

  def _split(receiver, args)
    string = receiver.frame.string
    separator = args[0].frame.string
    
    result = string.split(separator).map do |string|
      StringProto.instance.frame.create(string)
    end

    return ArrayProto.instance.frame.create(result)
  end

  def _concatenate(receiver, args)
    string = receiver.frame.string
    other = args[0].frame.string
    
    return Value(string + other)
  end

end

