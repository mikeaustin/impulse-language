#
# runtime/protos/expression.rb
#

require './core/frame.rb'
require './core/protos/object.rb'


def ExpressionProto(messages)
  return ExpressionProto.new(messages)
end

class ExpressionProto < Frame

  attr :messages

  def initialize(messages)
    super(ObjectProto.instance)
    
    @messages = messages
  end
  
  def eval_(receiver, args, locals)
    trace "ExpressionProto::eval()"
    
    result = @messages.reduce(locals) do |receiver, value|
      value && value.eval_(receiver, [], locals)
    end

    return result
  end

  def frame_inspect(value)
    messages = value.frame.messages.map do |message|
      message.inspect()
    end
    
    return messages
  end

  def frame_to_s(value)
    messages = value.frame.messages.map do |message|
      message.to_s
    end
    
    return "(" + messages.join(" ") + ")"
  end

end

