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
    super(nil)
    
    self.add_method(:print, FunctionProto(self.method(:print_)))
  end
  
  def print_(receiver, args)
  	puts args[0]
  	
    return nil
  end
  
end

