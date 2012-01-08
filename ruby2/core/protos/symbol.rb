#
# core/protos/symbol.rb
#

require './core/frame.rb'
require './core/protos/object.rb'


class SymbolProto < Frame

  def self.instance()
    @instance ||= SymbolProto.new(ObjectProto.instance)
    
    @instance.add_method(:slice, FunctionProto(@instance.frame.method(:_slice)))

    return @instance
  end

  def frame_to_s(value)
    return "#" + value.float.to_s
  end

  def frame_inspect(value)
    if value.frame == SymbolProto.instance.frame
      return "<symbol>"
    else
      return "##{value.float.to_s}"
    end
  end

  def arity
    return Value(-3)
  end

  def _call(receiver, args, object_self = nil)
  #p receiver.float
    return args[0].send_(receiver.float, args[1..-1], object_self)
  end

  def _slice(receiver, args)
    selector = receiver.float
    
    return ArrayProto.instance.frame.create(args.map do |item|
      item.send_(selector, [])
    end)
  end
  
end

class SymbolValue < SymbolProto

  def self.instance()
    return @instance ||= SymbolValue.new(SymbolProto.instance)
  end

end

