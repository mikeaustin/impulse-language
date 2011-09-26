
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


class BlockMessage < MessageProto

  attr :expressions, true
  attr :paramters, true
  attr :locals, true

  def initialize(expressions)
    super(:block, [])
    
    @expressions = expressions
  end

  def eval_(receiver, args, locals)
    return BlockProto.new(@expressions, locals)
  end

end

