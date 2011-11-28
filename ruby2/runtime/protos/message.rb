#
# class MessageProto
#

def MessageProto(selector, args)
  return MessageProto.new(selector, args)
end

class MessageProto < Frame

  attr :selector, true
  attr :args, true
  
  def initialize(selector, args)
    super(ObjectProto.instance)

    @selector, @args = selector, args
  end

  def frame_inspect(value)
    return "#<#{value.frame.class.name} @selector=#{value.frame.selector} @args=#{value.frame.args.inspect}>"
  end

  def frame_to_s(value)
    return "#{value.frame.selector}" + (value.frame.args.empty? ? "" : ": #{value.frame.args}")
  end

end


def SendMessage(selector, args)
  return SendMessage.new(selector, args)
end

class SendMessage < MessageProto

  def initialize(selector, args)
    super(selector, args)
  end

  def eval_(receiver, args, locals)
    trace "MessageProto::eval()"

    messageArgs = @args.map do |value|
      value.eval_(locals, [], locals)
    end

    result = receiver.send_(@selector, messageArgs)

    return result
  end
  
end


def AssignMessage(symbol, args)
  return AssignMessage.new(symbol, args)
end

class AssignMessage < MessageProto

  def initialize(symbol, args)
    super(:assign, [Value(symbol), args])
  end

  def eval_(receiver, args, locals)
  	return receiver.set_local(@args[0].float, @args[1])
  end

end


def LocalMessage(selector)
  return LocalMessage.new(selector)
end

class LocalMessage < MessageProto

  def initialize(selector)
    super(selector, [])
  end

  def eval_(receiver, args, locals)
    trace "LocalMessage::eval()"

    return receiver.find_local(@selector)
  end

end


def BlockMessage(argnames, expressions)
  return BlockMessage.new(argnames, expressions)
end

class BlockMessage < MessageProto

  attr :expressions, true
  attr :argnames, true

  def initialize(argnames, expressions)
    super(:block, [argnames, expressions])
    
    @argnames, @expressions = argnames, expressions
  end

  def eval_(receiver, args, locals)
    return BlockProto(@argnames, @expressions, locals)
  end

  def to_s(value)
    argnames = value.frame.argnames.map(&:to_s).join(",")
    
    return "(|#{argnames}| #{expressions})"
  end

end


def ObjectMessage(symbol, block)
  return ObjectMessage.new(symbol, block)
end

class ObjectMessage < MessageProto

  def initialize(symbol, block)
    super(:object, [block])
  end

  def eval_(receiver, args, locals)
	block = @args[0].eval_(locals, [], locals)
	
    object = Frame.new(ObjectProto.instance)
    block.frame.call_(locals, [object])
    
    return object
  end
  
end


def MethodMessage(symbol, block)
  return MethodMessage.new(symbol, block)
end

class MethodMessage < MessageProto

  def initialize(symbol, block)
    super(:method, [symbol, block])
  end

  def eval_(receiver, args, locals)
    block = @args[1].eval_(locals, [], locals)
    
    receiver.add_method(@args[0], block)
    
    return block
  end

end

