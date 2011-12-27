#
# core/protos/lobby.rb
#


def LobbyProto()
  return LobbyProto.new()
end

class LobbyProto < Frame

  def initialize()
    super(ObjectProto.instance)
    
    self.add_method(:"print:", FunctionProto(self.method(:_print_)))
    self.add_method(:"exit",   FunctionProto(self.method(:_exit)))
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

