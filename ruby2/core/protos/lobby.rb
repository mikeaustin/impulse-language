#
# class LobbyProto
#

def LobbyProto()
  return LobbyProto.new()
end

class LobbyProto < Frame

  #def initialize()
  #  super(ObjectProto.instance)
  #end

  def initialize()
    super(ObjectProto.instance)
    
    self.add_method(:print, FunctionProto(self.method(:_print_)))
    self.add_method(:exit, FunctionProto(self.method(:_exit)))
  end
 
  def frame_inspect(value)
    return "<lobby>"
  end
  
  def _print_(receiver, args)
  	puts args[0].to_s
  	
    return nil
  end

  def _exit(receiver, args)
    exit
  end
  
end

