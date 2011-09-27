
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

  attr :argnames, true
  attr :expressions, true
  attr :locals, true

  def initialize(argnames, expressions, locals)
    super(self.method(:eval_block))

    @argnames, @expressions, @locals = argnames, expressions, locals

    self.set_slot(:eval, FunctionProto.new(self.method(:eval_)))
  end

  def eval_block(receiver, args, locals)
    locals = LocalsProto.new(@locals)

    @argnames.each.with_index do |argname, i|
      locals.set_slot(argname, args[i])
    end

    return @expressions.reduce(locals) do |receiver, expression|
      expression.eval_(receiver, [], locals)
    end
  end

end

