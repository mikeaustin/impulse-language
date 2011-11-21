#
# core/protos/string.rb
#

require './core/frame.rb'

class StringProto < Frame

  attr :string, true

  def self.instance()
    return @instance ||= StringProto.new()
  end

  def initialize(string)
    super(ObjectProto.instance)
    
    @string = string
    
    self.add_method(:size, FunctionProto(self.method(:size_)))
  end

  def to_s(value)
    return value.string
  end
  
  def inspect(value)
    return '"' + value.frame.string + '"'
  end

  def size_(receiver, args)
    return Value(@string.size)
  end

end

