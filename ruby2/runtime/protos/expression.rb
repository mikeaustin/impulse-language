
class ExpressionProto < Frame

  def initialize(messages)
    super(ObjectProto.instance)
    
    @messages = messages
  end
  
  def eval_(receiver, args, locals)
    trace "ExpressionProto::eval()"
    
    return @messages.reduce(locals) do |receiver, value|
      value.eval_(receiver, [], locals)
    end
  end

  def inspect()
    messages = @messages.map do |message|
      message.inspect()
    end
    
    return messages
  end

  def to_s()
    messages = @messages.map do |message|
      message.to_s
    end
    
    return "(" + messages.join(" ") + ")"
  end

end

