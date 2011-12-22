#
# core/protos/string.rb
#

require './core/frame.rb'

class StringProto < Frame

  attr :string, true

  def self.instance()
    @instance ||= StringProto.new(ObjectProto.instance)

    @instance.add_method2(:"size", [])           { |receiver, args| receiver.frame.size }
    @instance.add_method2(:"capitalize", [])     { |receiver, args| receiver.frame.capitalize }
    @instance.add_method2(:"split", [@instance]) { |receiver, args| receiver.frame.split(args[0].frame.string) }
    @instance.add_method2(:"++", [])    { |receiver, args| receiver.frame.concatenate(args[0].frame.string) }

    return @instance
  end

  def create(string)
    object = StringProto.new(StringProto.instance)
    
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

  def size
    return self.string.size
  end

  def capitalize
    return self.string[0].upcase + self.string[1..-1]
  end

  def split(separator)
    result = self.string.split(separator).map do |item|
      StringProto.instance.frame.create(item)
    end

    return ArrayProto.instance.frame.create(result)
  end

  def concatenate(other)
    return StringProto.instance.frame.create(self.string + other)
  end

end

