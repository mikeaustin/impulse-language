#
# core/protos/lobby.rb
#

require './core/protos/nil.rb'
require './core/protos/object.rb'
require './core/protos/boolean.rb'
require './core/protos/number.rb'
require './core/protos/string.rb'
require './core/protos/symbol.rb'
require './core/protos/array.rb'
require './core/protos/range.rb'
require './core/protos/set.rb'
require './core/protos/block.rb'
require './core/protos/point.rb'

require './runtime/protos/message.rb'


def LobbyProto()
  return LobbyProto.new()
end

class LobbyProto < Frame

  def initialize()
    super(ObjectProto.instance)
    
    self.add_method(:"print:", FunctionProto(self.method(:_print_)))
    self.add_method(:"exit",   FunctionProto(self.method(:_exit)))

    self.set_local(:"nil",   NilProto.instance)
    self.set_local(:"true",  Value(true))
    self.set_local(:"false", Value(false))
    
    self.set_local(:"<object>",  ObjectProto.instance)
    self.set_local(:"<number>",  NumberProto.instance)
    self.set_local(:"<string>",  StringProto.instance)
    self.set_local(:"<symbol>",  SymbolProto.instance)
    self.set_local(:"<array>",   ArrayProto.instance)
    self.set_local(:"<range>",   RangeProto.instance)
    self.set_local(:"<set>",     SetProto.instance)
    self.set_local(:"<block>",   BlockProto.instance)
    self.set_local(:"<point>",   PointProto.instance)

    self.set_local(:"<message>", MessageProto.instance)

    self.set_local(:"lobby",    Value(self))
  end
 
  def frame_inspect(value)
    return "<lobby>"
  end
  
  def _print_(receiver, args)
    print args[0]
    print "\n= " + args[1].inspect + "\n" if args[1]
  	
    puts
  	
    return nil
  end

  def _exit(receiver, args)
    exit
  end
  
end


def LocalsProto(frame)
  return LocalsProto.new(frame)
end

class LocalsProto < Frame

  def frame_inspect(value)
    return "<locals>"
  end

end

