#
# core/protos/array.rb
#

require './core/protos/object.rb'
require './core/protos/block.rb'


class ArrayProto < Frame

  attr :array, true
  attr :hash, true

  def self.instance()
    @instance ||= ArrayProto.new(ObjectProto.instance)

    @instance.add_method2(:"size", [])         { |receiver, args| Value(receiver.frame.size) }

    @instance.add_method2(:"slice", [])        { |receiver, args| receiver.frame.slice(args[0]) }
    @instance.add_method2(:"slice_assign", []) { |receiver, args| receiver.frame.slice_assign(args[0], args[1]) }
    @instance.add_method2(:"++", [@instance])  { |receiver, args| receiver.frame.concatenate(args[0]) }
    @instance.add_method(:"reverse",      FunctionProto(@instance.frame.method(:_reverse)))

    @instance.add_method(:"map:",         FunctionProto(@instance.frame.method(:_map_)))
    @instance.add_method(:"fold:",        FunctionProto(@instance.frame.method(:_fold_)))
    @instance.add_method(:"join:",        FunctionProto(@instance.frame.method(:_join_)))
    @instance.add_method2(:"each-pair:", []) { |receiver, args| receiver.frame._each_pair_(receiver, args[0]) }
    @instance.add_method2(:"count:", []) { |receiver, args| receiver.frame._count_(receiver, args[0]) }

    @instance.add_method2(:"==", [])     { |receiver, args| receiver.frame.equal(args[0].frame) }
    @instance.add_method2(:"any:", [])   { |receiver, args| receiver.frame.any(receiver, args[0]) }
    @instance.add_method2(:"all:", [])   { |receiver, args| receiver.frame.all(receiver, args[0]) }
    
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

  def frame_to_s(value)
    if value.frame == ArrayProto.instance.frame
      return "<array>"
    end
    
    return "#{value.frame.array} + #{value.frame.hash}"
  end

  def frame_inspect(value)
    if value.frame == ArrayProto.instance.frame
      return "<array>"
    end
    
    return "#{value.frame.array.inspect} + #{value.frame.hash.inspect}"
  end

  #
  # <array> methods
  #

  def equal(other)
    self.array.zip(other.array) do |item_a, item_b|
      if item_a.equal(item_b).float == false
        return Value(false)
      end
    end
    
    return Value(true)
  end

  def size
    return self.array.size + self.hash.size
  end

  def find(block)
    self.array.each do |item|
      if block.frame._call(block, [item]).float == true
        return item
      end
    end

    return NilProto.instance
  end

  def any(receiver, block)
    return Value(receiver.frame.find(block) != NilProto.instance)
  end

  def all(receiver, block)
    not_block = FunctionProto(proc { |receiver, args| Value(!block.frame._call(block, args).float) })

    return Value(self.find(not_block) == NilProto.instance)
  end

  def slice(index)
    if index.float >= 1 && index.float <= self.array.size
      return Value(self.array[index.float - 1])
    end

    return self.hash[index.float]
  end

  def slice_assign(index, value)
    if index.float >= 1 && index.float <= self.array.size
      if value.frame == NothingProto.instance.frame
        return Value(self.array.delete_at(index.float - 1))
      end

      return Value(self.array[index.float - 1] = value)
    else
      if value.frame == NothingProto.instance.frame
        return Value(self.hash.delete(index.float))
      end

      return Value(self.hash[index.float] = value)
    end
  end

  def concatenate(other)
  p other.frame.proto.frame == ArrayProto.instance.frame
    #array = receiver.frame.array
    #other = args[0].frame.array
    
    return create(self.array + other.frame.array)
  end

  def _reverse(receiver, args)
    array = receiver.frame.array
    
    ArrayProto.instance.frame.create(array.reverse)
  end

  def _map_(receiver, args)
    array = receiver.frame.array
    block = args[0]

    result = []
    
    array.each do |item|
      result.push(block.frame._call(block, [item]))
    end
    
    return ArrayProto.instance.frame.create(result)
  end

  def _fold_(receiver, args)
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

  def _join_(receiver, args)
    array = receiver.frame.array
    
    if args[0].proto.frame == StringProto.instance.frame
      separator = args[0].frame.string

      return Value(array.join(separator))
    elsif args[0].proto.frame == BlockProto.instance.frame
      #block = args[0]
      
      #BlockProto.instance.frame._call(block, [item])
    end
  end

  def _each_pair_(receiver, block)
    array = receiver.frame.array
    
    (0...array.size).each do |i|
      ((i + 1)...array.size).each do |j|
        block.frame._call(block, [array[i], array[j]])
      end
    end
    
    return nil
  end

  def _count_(receiver, block)
    array = receiver.frame.array
    
    return array.reduce(0) do |count, item|
      if block.frame._call(block, [item]).float == true
        count + 1
      else
        count
      end
    end
  end
  
end

