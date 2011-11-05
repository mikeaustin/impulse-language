#
# class MessageProto
#

class MessageProto < Frame

  attr :selector, true
  attr :args, true
  
  def initialize(selector, args)
    super(ObjectProto.instance)

    @selector, @args = selector, args
  end

  def inspect(value)
    return "#<#{value.frame.class.name} @selector=#{value.frame.selector} @args=#{value.frame.args.inspect}>"
  end

  def to_s(value)
    return "#{value.frame.selector}" + (value.frame.args.empty? ? "" : ": #{value.frame.args}")
  end

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


class AssignMessage < MessageProto

  def initialize(symbol, args)
    super(:assign, [symbol, args])
  end

  def eval_(receiver, args, locals)
  	return receiver.set_slot(@args[0].float, @args[1])
  end

end


class LocalMessage < MessageProto

  def initialize(selector)
    super(selector, [])
  end

  def eval_(receiver, args, locals)
    trace "LocalMessage::eval()"

    return receiver.find_slot(@selector)
  end

end


class BlockMessage < MessageProto

  attr :expressions, true
  attr :argnames, true

  def initialize(argnames, expressions)
    super(:block, [])
    
    @argnames, @expressions = argnames, expressions
  end

  def eval_(receiver, args, locals)
    return BlockProto.new(@argnames, @expressions, locals)
  end

  def to_s(value)
    argnames = value.frame.argnames.map(&:to_s).join(",")
    
    return "(|#{argnames}| #{expressions})"
  end

end

