#
# core/protos/block.rb
#

require './core/frame.rb'
require './core/protos/object.rb'


def FunctionProto(function, arg_types = [])
  return FunctionProto.instance.frame.create(function, arg_types)
end

class FunctionProto < Frame

  attr :function, true
  attr :arg_types, true
  attr :arg_names, true
  attr :summary_doc, true

  def self.instance()
    @instance ||= FunctionProto.new(ObjectProto.instance)
  end
  
  def create(function, arg_types)
    object = self.class.new(Value(self))
    
    object.frame.function = function
    object.frame.arg_types = arg_types
    object.frame.arg_names = []

    return object
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
    
    if function.arity == -3 || function.arity == 3
      return function.call(receiver, args, object_self);
    else
      return function.call(receiver, args);
    end
  end

end


def BlockProto(arg_names, expressions, locals)
  return BlockProto.instance.frame.create(arg_names, expressions, locals)
end

class BlockProto < FunctionProto

  attr :expressions, true
  attr :locals, true

  def self.instance()
    @instance ||= BlockProto.new(ObjectProto.instance)

    @instance.add_method2(:"arity", []) { |receiver, args| receiver.frame.arity }
    @instance.add_method2(:"call:", []) { |receiver, args| receiver.frame._call(receiver, args) }
    @instance.add_method2(:"slice", []) { |receiver, args| receiver.frame._call(receiver, args) }
    
    return @instance
  end

  def create(arg_names, expressions, locals)
    object = super(self.method(:_call), []);
    
    object.frame.arg_names = arg_names
    object.frame.expressions = expressions
    object.frame.locals = locals
    
    return object
  end

  def _call(receiver, args, object_self = nil)
    locals = LocalsProto(@locals)
    locals.add_local(:"self", object_self) if object_self
    
    @arg_names.each.with_index do |argname, i|
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
    
    return "|#{value.frame.arg_names}| #{value.frame.expressions}"
  end

  #
  # <block> methods
  #

  def arity
    return Value(@arg_names.size)
  end

end

