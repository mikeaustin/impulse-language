
class ObjectProto < Frame

  def self.instance()
    return @instance ||= ObjectProto.new(NilProto.instance)
  end

  def init_slots()
    self.add_method(:id,      FunctionProto(self.method(:id)))
    self.add_method(:type,    FunctionProto(self.method(:type)))
    self.add_method(:is_a,    FunctionProto(self.method(:is_a)))
    self.add_method(:assign,  FunctionProto(self.method(:assign_)))
    self.add_method(:methods, FunctionProto(self.method(:_methods)))
  end

  def frame_inspect(value)
    if value.frame == ObjectProto.instance.frame
      return Value("<object>")
    end
    
    return super()
  end

  def id(receiver, args)
    return Value(receiver.frame.object_id)
  end

  def type(receiver, args)
    return receiver.proto
  end

  def is_a(receiver, args)
    return Value(receiver.proto.frame == args[0].frame)
  end

  def assign_(receiver, args)
  	return receiver.set_slot(args[0].float, args[1])
  end

  def _methods(receiver, args)
  	#puts receiver.frame_methods.keys
  	puts receiver.inspect if !receiver.frame_methods.empty?
  	receiver.frame_methods.keys.each do |selector|
  	  puts "  #{selector}"
  	end
  	self._methods(receiver.proto, args) if receiver.proto
  	
  	return NilProto.instance
  end

end

