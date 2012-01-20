#
# core/protos/string.rb
#

require './core/frame.rb'
require './core/protos/object.rb'


class StringProto < Frame

  attr :string, true

  def self.instance()
    @instance ||= StringProto.new(ObjectProto.instance)

    @instance.add_method2(:"size", [])            { |receiver, args| receiver.frame.size }
    @instance.add_method2(:"slice", [])           { |receiver, args| receiver.frame.slice(args[0]) }
    @instance.add_method2(:"upper-case", [])      { |receiver, args| receiver.frame.upper_case }
    @instance.add_method2(:"lower-case", [])      { |receiver, args| receiver.frame.lower_case }
    @instance.add_method2(:"capitalize", [])      { |receiver, args| receiver.frame.capitalize }
    @instance.add_method2(:"reverse", [])         { |receiver, args| receiver.frame.reverse }
    @instance.add_method2(:"split:", [@instance]) { |receiver, args| receiver.frame.split(args[0].frame) }

    @instance.add_method2(:"each:", [])           { |receiver, args| self.instance.frame.each(receiver, args[0].frame) }

    #@instance.add_method2(:"++", [@instance])     { |receiver, args| receiver.frame.concatenate(args[0].frame) }
    @instance.add_method2(:"++", [])              { |receiver, args| receiver.frame.concatenate(args[0]) }

    @instance.add_method2(:"==", [@instance])     { |receiver, args| receiver.frame.equal(args[0].frame) }

    return @instance
  end

  def initialize(proto)
    super(proto)
    
    self.string = "todo"
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
      return "<string>"
    end

    return "\"#{value.frame.string}\""
  end

  def equal(other)
    return Value(self.string == other.string)
  end

  def size
    return Value(self.string.size)
  end

  def slice(index)
    if index.frame_is_a(NumberProto.instance)
      if index.float >= 1 && index.float <= self.string.size
        return Value(self.string[index.float - 1])
      end
    elsif index.frame_is_a(RangeProto.instance)
      return Value(self.string[index.frame.range.first - 1 .. index.frame.range.last - 1])
    end
    
    return NothingProto.instance
  end

  def upper_case
    return Value(self.string.upcase)
  end

  def lower_case
    return Value(self.string.downcase)
  end

  def capitalize
    return Value(self.string[0].upcase + self.string[1..-1])
  end

  def reverse
    StringProto.instance.frame.create(self.string.reverse)
  end

  def split(separator)
    result = self.string.split(separator.string).map do |item|
      #StringProto.instance.frame.create(item)
      Value(item)
    end

    return ArrayProto.instance.frame.create(result)
  end

  def each(receiver, block)
    receiver.frame.string.each_char do |char|
      block._call(block, [char])
    end
  end

  def concatenate(other)
    return StringProto.instance.frame.create(self.string + other.to_s)
  end

end

