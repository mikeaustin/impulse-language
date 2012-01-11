#
# core/protos/block.rb
#

require './core/frame.rb'
require './core/protos/object.rb'


def FunctionProto(func, arg_types = [])
  return FunctionProto.new(func, arg_types)
end

class FunctionProto < Frame

  attr :func, true
  attr :arg_types, true
  attr :arg_names, true
  attr :summary_doc, true
  
  def initialize(func, arg_types)
    super(ObjectProto.instance)
    
    @func, @arg_types = func, arg_types
    @arg_names = []
  end

  def _call(receiver, args, object_self = nil)
    trace "FunctionProto::eval()"

    no_match = @arg_types.zip(args) do |proto, arg|
      break true if proto && !arg.frame_is_a(proto)
    end
      
    if no_match
      puts "Argument type mismatch"
      
      return nil
    end
    
    if func.arity == -3 || func.arity == 3
      return func.call(receiver, args, object_self);
    else
      return func.call(receiver, args);
    end
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
    @instance.frame.frame_proto = ObjectProto.instance

    @instance.add_method2(:"arity", []) { |receiver, args| receiver.frame.arity }
    @instance.add_method2(:"call:", []) { |receiver, args| receiver.frame._call(receiver, args) }
    @instance.add_method2(:"slice", []) { |receiver, args| receiver.frame._call(receiver, args) }
    
    return @instance
  end

  def initialize(argnames, expressions, locals)
    super(self.method(:call_block), [])

    if !expressions.empty?
      self.frame_proto = BlockProto.instance
    end
    
    @argnames, @expressions, @locals = argnames, expressions, locals
  end

  def call_block(receiver, args, object_self = nil)
    locals = LocalsProto(@locals)
    locals.add_local(:"self", object_self) if object_self

    @argnames.each.with_index do |argname, i|
      locals.set_local(argname, args[i])
    end

    result = @expressions.reduce(locals) do |receiver, expression|
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

  #
  # <block> methods
  #

  def arity
    return Value(@argnames.size)
  end

end

