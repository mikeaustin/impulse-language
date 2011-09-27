
class MessageProto < Frame

  attr :selector, true
  attr :args, true

  def initialize(selector, args)
    super(ObjectProto.instance)
    
    @selector, @args = selector, args
  end

  def eval_(receiver, args, locals)
    trace "MessageProto::eval()"

    messageArgs = @args.map do |value|
      value.eval_(locals, [], locals)
    end

    return receiver.send_(@selector, messageArgs)
  end

  def inspect()
    return "#<#{self.class.name} @selector=#{@selector} @args=#{@args.inspect}>"
  end

  def to_s()
    return "#{@selector}" + (@args.empty? ? "" : ": #{@args}")
  end
  
end


class LocalMessage < MessageProto

  def initialize(selector)
    super(selector, [])
  end

  def eval_(receiver, args, locals)
    trace "LocalMessage::eval()"

    return receiver.slots[@selector]
  end

end


class BlockMessage < MessageProto

  attr :expressions, true
  attr :argnames, true
  attr :locals, true

  def initialize(argnames, expressions)
    super(:block, [])
    
    @argnames, @expressions = argnames, expressions
  end

  def eval_(receiver, args, locals)
    return BlockProto.new(@argnames, @expressions, locals)
  end

  def to_s()
    argnames = @argnames.map(&:to_s).join(",")
    
    return "(|#{argnames}| #{expressions})"
  end

end

