
class ObjectProto < Frame

  def self.instance()
    return @instance ||= ObjectProto.new()
  end
  
  def initialize()
    super(nil)
  end
  
  def init_slots()
    self.add_method(:id,     FunctionProto(self.method(:id)))
    self.add_method(:type,   FunctionProto(self.method(:type)))
    self.add_method(:is_a,   FunctionProto(self.method(:is_a)))
    self.add_method(:assign, FunctionProto(self.method(:assign_)))
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

end

