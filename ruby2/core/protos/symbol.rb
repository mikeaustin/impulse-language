#
# symbol.rb
#

require './core/frame.rb'

class SymbolProto < Frame

  def self.instance()
    return @instance ||= SymbolProto.new()
  end

  def initialize()
    super(ObjectProto.instance)
  end

end

class SymbolValue < Frame

  def self.instance()
    return @instance ||= SymbolValue.new()
  end

  def initialize()
    super(SymbolProto.instance)
  end

  def to_s(value)
    return "'" + value.float.to_s
  end

  def inspect(value)
    return value.float.inspect
  end

end
