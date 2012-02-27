#
# runtime/protos/message.rb
#

require './core/frame.rb'
require './core/protos/object.rb'


def MessageProto(selector, args)
  #return MessageProto.new(selector, args)
  return MessageProto.instance.frame.create(selector, args)
end

class MessageProto < Frame

  attr :selector, true
  attr :args, true

  def self.instance
    @instance ||= self.new(ObjectProto.instance)
    
    return @instance
  end

  def create(selector, args)
    object = self.class.new(Value(self))
    
    object.frame.selector = selector
    object.frame.args = args
    
    return object
  end
  
  def xinitialize(selector, args)
    super(ObjectProto.instance)

    @selector, @args = selector, args
  end

  def frame_inspect(value)
    return "#<#{value.frame.class.name} @selector=#{value.frame.selector} @args=#{value.frame.args.inspect}>"
  end

  def frame_to_s(value)
    if value.frame.selector.match(/\+|\-|\*|\//)
      return "#{value.frame.selector} #{value.frame.args}"
    elsif value.frame.selector == :block
      return "|#{args[0]}| #{value.frame.args[1..-1]}"
    else
      return "#{value.frame.selector}" + (value.frame.args.empty? ? "" : ": #{value.frame.args}")
    end
  end

end


def SendMessage(selector, args)
  #return SendMessage.new(selector, args)
  return SendMessage.instance.frame.create(selector, args)
end

class SendMessage < MessageProto

  def create(selector, args)
    object = super(selector, args)
    
    return object
  end    

  def xinitialize(selector, args)
    super(selector, args)
  end

  def eval_(receiver, locals)
    trace "MessageProto::eval()"

    messageArgs = @args.map do |value|
      value.eval_(locals, locals)
    end

    result = receiver && receiver.send_(@selector, messageArgs, locals)

    return result
  end
  
end


def AssignMessage(symbol, args)
  return AssignMessage.instance.frame.create(symbol, args)
end

class AssignMessage < MessageProto

  def create(symbol, args)
    object = super(:assign, [Value(symbol), args])
    
    return object
  end    

  def xinitialize(symbol, args)
    super(:assign, [Value(symbol), args])
  end

  def eval_(receiver, locals)
    symbol = @args[0].float
    value  = @args[1].eval_(locals, locals)
    
    #if value
  	  return receiver.set_local(symbol, value)
  	#end
  end

end


def LocalMessage(selector)
  #return LocalMessage.new(selector)
  return LocalMessage.instance.frame.create(selector)
end

class LocalMessage < MessageProto

  def create(selector)
    object = super(selector, [])
    
    return object
  end

  def xinitialize(selector)
    super(selector, [])
  end

  def eval_(receiver, locals)
    trace "LocalMessage::eval()"

    return receiver.find_local(@selector)
  end

  def match(receiver, value, locals)
    return { receiver.frame.selector => value }
  end

end


def ArrayMessage(items)
  return ArrayMessage.instance.frame.create(items)
end

class ArrayMessage < MessageProto

  def create(values)
    object = super(:array, [values])
    
    return object
  end    

  def eval_(receiver, locals)
    messageArgs = @args[0].map do |value|
      value.eval_(locals, locals)
    end

    return ArrayProto.instance.frame.create(messageArgs)
  end

  def match(receiver, value, locals)
    items  = receiver.frame.args[0]
    values = value.frame.array
    
    bindings = items.zip(values).reduce({}) do |bindings, (item, value)|
      match = item.match(value, locals)
      match ? bindings.merge(match) : (return nil)
    end

    return bindings;
  end

end


def BlockMessage(argnames, expressions)
  return BlockMessage.instance.frame.create(argnames, expressions)
end

class BlockMessage < MessageProto

  attr :expressions, true
  attr :argnames, true

  def create(argnames, expressions)
    object = super(:block, [argnames, expressions])

    object.frame.argnames, object.frame.expressions = argnames, expressions

    return object
  end

  def xinitialize(argnames, expressions)
    super(:block, [argnames, expressions])
    
    @argnames, @expressions = argnames, expressions
  end

  def eval_(receiver, locals)
    return BlockProto(@argnames, @expressions, locals)
  end

  def to_s(value)
    argnames = value.frame.argnames.map(&:to_s).join(",")
    
    return "(|#{argnames}| #{expressions})"
  end

end


def ObjectMessage(symbol, block)
  return ObjectMessage.instance.frame.create(symbol, block)
end

class ObjectMessage < MessageProto

  def create(symbol, block)
    object = super(:object, [block])

    return object
  end

  def xinitialize(symbol, block)
    super(:object, [block])
  end

  def eval_(receiver, locals)
	block = @args[0].eval_(locals, locals)
	
    object = Frame.new(ObjectProto.instance)
    block.frame._call(receiver, [object])
    
    return object
  end
  
end


def MethodMessage(symbol, block)
  return MethodMessage.instance.frame.create(symbol, block)
end

class MethodMessage < MessageProto

  def create(symbol, block)
    object = super(:method, [symbol, block])

    return object
  end

  def xinitialize(symbol, block)
    super(:method, [symbol, block])
  end

  def eval_(receiver, locals)
    block = @args[1].eval_(locals, locals)
    
    receiver.add_method(@args[0], block)
    
    return nil
  end

end


def PatternMessage(lvalue, value)
  return PatternMessage.instance.frame.create(lvalue, value)
end

class PatternMessage < MessageProto

  def create(lvalue, value)
    return super(:pattern, [lvalue, value])
  end
  
  def eval_(receiver, locals)
    #return receiver.set_local(@args[0].frame.selector, @args[1].eval_(locals, locals))
#p @args[0]
    bindings = @args[0].frame.match(@args[0], @args[1].eval_(locals, locals), locals)
#p bindings
p bindings
    if !bindings
      puts "*** Match error: #{@args[0]}"
      
      return nil
    end
    
    bindings.each do |symbol, value|
      receiver.set_local(symbol, value)
    end
    
    nil
  end

end


def TypeMessage(symbol, value)
  return TypeMessage.instance.frame.create(symbol, value)
end

class TypeMessage < MessageProto

  def create(symbol, value)
    return super(:type, [symbol, value])
  end

  def eval_(receiver, locals)
    proto = @args[1].eval_(locals, locals)

    return Value(@args[0].eval_(locals, locals).frame_proto.frame == proto.frame)
  end

  def match(receiver, value, locals)
    proto = @args[1].eval_(locals, locals)

    if value.frame_is_a(proto)
      return @args[0].match(value, locals)
    end
    
    return nil
  end

end

