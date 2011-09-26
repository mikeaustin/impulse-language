
class FunctionProto < Frame

  attr :func, true
  
  def initialize(func)
    super(ObjectProto.instance)
    
    @func = func
  end

  def eval_(receiver, args, locals)
    trace "FunctionProto::eval()"
    
    return func.call(receiver, args, locals);
  end

end


class BlockProto < FunctionProto

  attr :expressions, true
  attr :locals, true

  def initialize(expressions, locals)
    super(self.method(:eval_block))

    self.set_slot(:eval, FunctionProto.new(self.method(:eval_)))
    
    @expressions, @locals = expressions, locals
  end

  def eval_block(receiver, args, locals)
    locals = LocalsProto.new(@locals)

    return @expressions.reduce(locals) do |receiver, expression|
      expression.eval_(receiver, [], locals)
    end
  end

end

