
def FunctionProto(func)
  return FunctionProto.new(func)
end

class FunctionProto < Frame

  attr :func, true
  
  def initialize(func)
    super(ObjectProto.instance)
    
    @func = func
  end

  def call_(receiver, args)
    trace "FunctionProto::eval()"
    
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

  def initialize(argnames, expressions, locals)
    super(self.method(:call_block))

    @argnames, @expressions, @locals = argnames, expressions, locals

    self.add_method(:call, FunctionProto(self.method(:call_)))
  end

  def call_block(receiver, args)
    locals = LocalsProto(@locals)

    @argnames.each.with_index do |argname, i|
      locals.set_slot(argname, args[i])
    end

    result = @expressions.reduce(receiver) do |receiver, expression|
      expression.eval_(receiver, [], locals)
    end
    
    return result
  end

end

