#
# core/protos/array.rb
#

require './core/protos/object.rb'


class ArrayProto < Frame

  attr :array, true
  attr :hash, true

  def self.instance()
    @instance ||= ArrayProto.new(ObjectProto.instance)

    @instance.add_method2(:"size", [])       { |receiver, args| Value(receiver.frame.size) }

    @instance.add_method(:slice, FunctionProto(@instance.frame.method(:_slice)))
    @instance.add_method(:slice_assign, FunctionProto(@instance.frame.method(:_slice_assign)))
    @instance.add_method(:"++", FunctionProto(@instance.frame.method(:_concatenate)))
    @instance.add_method(:reverse, FunctionProto(@instance.frame.method(:_reverse)))
    @instance.add_method(:map,   FunctionProto(@instance.frame.method(:_map)))
    @instance.add_method(:fold,  FunctionProto(@instance.frame.method(:_fold)))
    @instance.add_method(:join,  FunctionProto(@instance.frame.method(:_join)))
    
    return @instance
  end

  def create(array)
    object = self.class.new(Value(self))
    
    object.frame.array = array
    object.frame.hash  = {}
    
    return object
  end

  #
  # <frame> methods
  #

  def frame_inspect(value)
    if value.frame == ArrayProto.instance.frame
      return Value("<array>")
    end
    
    return "#{value.frame.array.inspect} + #{value.frame.hash.inspect}"
  end

  #
  # <array> methods
  #

  def size
    return self.array.size + self.hash.size
  end

  def _slice(receiver, args)
    array = receiver.frame.array
    index = args[0].float.to_i

    if index >= 1 && index <= array.size
      return Value(receiver.frame.array[index - 1])
    else
      #return NilProto.instance
      return Value(receiver.frame.hash[index] || NilProto.instance)
    end
  end

  def _slice_assign(receiver, args)
    array = receiver.frame.array
    index = args[0].float.to_i
    
    if index >= 1 && index <= array.size
      return Value(receiver.frame.array[index - 1] = args[1])
    else
      #return NilProto.instance
      return Value(receiver.frame.hash[index] = args[1])
    end
  end

  def _concatenate(receiver, args)
    array = receiver.frame.array
    other = args[0].frame.array
    
    return ArrayProto.instance.frame.create(array + other)
  end

  def _reverse(receiver, args)
    array = receiver.frame.array
    
    ArrayProto.instance.frame.create(array.reverse)
  end

  def _map(receiver, args)
    array = receiver.frame.array
    block = args[0]

    result = []
    
    array.each do |item|
      result.push(block.frame._call(block, [item]))
    end
    
    return ArrayProto.instance.frame.create(result)
  end

  def _fold(receiver, args)
    array = receiver.frame.array
    block = args[0]
    value = args[1]
    
    if value
      result = array.reduce(value) do |accum, item|
        block.frame._call(block, [accum, item])
      end
    else 
      result = array.reduce() do |accum, item|
        block.frame._call(block, [accum, item])
      end
    end
    
    return result
  end

  def _join(receiver, args)

    array = receiver.frame.array
    if args[0].proto.frame == StringProto.instance.frame
      separator = args[0].frame.string

      return Value(array.join(separator))
    elsif args[0].proto.frame == BlockProto.instance.frame
      #block = args[0]
      
      #BlockProto.instance.frame._call(block, [item])
    end
  end
  
end

