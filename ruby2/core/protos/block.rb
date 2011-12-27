#
# core/protos/block.rb
#


def FunctionProto(func, arg_types = [])
  return FunctionProto.new(func, arg_types)
end

class FunctionProto < Frame

  attr :func, true
  attr :arg_types, true
  
  def initialize(func, arg_types)
    super(ObjectProto.instance)
    
    @func, @arg_types = func, arg_types
  end

  def _call(receiver, args, object_self = nil)
    trace "FunctionProto::eval()"

    if !@arg_types.empty? && args.map(&:proto) != @arg_types
      puts "Argument type mismatch"
      
      return nil
    end
    
    return func.call(receiver, args);
  end

end


def BlockProto(argnames, expressions, locals)
  return BlockProto.new(argnames, expressions, locals)
end

class BlockProto < FunctionProto

  attr :argnames, true
  attr :expressions, true
  attr :locals, true

  def self.instance()
    @instance ||= BlockProto.new([], [], nil)
    @instance.frame.proto = ObjectProto.instance

    @instance.add_method(:"arity", FunctionProto(@instance.frame.method(:_arity)))
    @instance.add_method(:"call:", FunctionProto(@instance.frame.method(:call_block2)))
    @instance.add_method(:"slice", FunctionProto(@instance.frame.method(:call_block2)))
    
    return @instance
  end

  def initialize(argnames, expressions, locals)
    super(self.method(:call_block), [])

    if !expressions.empty?
      self.proto = BlockProto.instance
      self.frame_locals.parent = self.proto.frame_locals
      self.frame_methods.parent = self.proto.frame_methods
    end
    
    @argnames, @expressions, @locals = argnames, expressions, locals
  end

  def call_block(receiver, args, object_self = nil)
    locals = LocalsProto(@locals)
    locals.set_local(:self, object_self)

    @argnames.each.with_index do |argname, i|
      locals.set_local(argname, args[i])
    end

    result = @expressions.reduce(locals) do |receiver, expression|
      expression.eval_(receiver, [], locals)
    end
    
    return result
  end

  def call_block2(receiver, args, object_self = nil)
    locals = LocalsProto(receiver.frame.locals)
    locals.set_local(:self, object_self)
	
    receiver.frame.argnames.each.with_index do |argname, i|
      locals.set_local(argname, args[i])
    end

    result = receiver.frame.expressions.reduce(locals) do |receiver, expression|
      expression.eval_(receiver, [], locals)
    end
    
    return result
  end

  def frame_inspect(value)
    if value.frame == BlockProto.instance.frame
      return "<block>"
    end
    
    return "|#{value.frame.argnames}| #{value.frame.expressions}"
  end

  def _arity(receiver, args)
    return Value(receiver.frame.argnames.size)
  end

end

