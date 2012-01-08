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

    @instance.add_method2(:"size", [])         { |receiver, args| receiver.frame.size }

    @instance.add_method2(:"slice", [])        { |receiver, args| receiver.frame.slice(args[0]) }
    @instance.add_method2(:"slice_assign", []) { |receiver, args| receiver.frame.slice_assign(args[0], args[1]) }
    @instance.add_method2(:"++", [@instance])  { |receiver, args| receiver.frame.concatenate(args[0]) }
    @instance.add_method2(:"reverse", [])      { |receiver, args| receiver.frame.reverse }

    @instance.add_method2(:"map:", [])         { |receiver, args| receiver.frame.map(args[0]) }
    @instance.add_method2(:"fold:", [])        { |receiver, args| receiver.frame.fold(args[0], args[1]) }
    @instance.add_method2(:"join:", [])        { |receiver, args| receiver.frame.join(args[0]) }
    @instance.add_method2(:"each-pair:", [])   { |receiver, args| receiver.frame.each_pair(args[0]) }
    @instance.add_method2(:"count:", [])       { |receiver, args| receiver.frame.count(args[0]) }
    @instance.add_method2(:"**", [])           { |receiver, args| receiver.frame.repeat(args[0]) }

    @instance.add_method2(:"==", [])           { |receiver, args| receiver.frame.equal(args[0].frame) }
    @instance.add_method2(:"any:", [])         { |receiver, args| receiver.frame.any(args[0]) }
    @instance.add_method2(:"all:", [])         { |receiver, args| receiver.frame.all(args[0]) }
    
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
    return Value(self.array.size + self.hash.size)
  end

  def find(block)
    self.array.each do |item|
      if block.frame._call(block, [item]).float == true
        return item
      end
    end

    return NilProto.instance
  end

  def any(block)
    return Value(self.find(block) != NilProto.instance)
  end

  def all(block)
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
    return create(self.array + other.frame.array)
  end

  def reverse
    return create(self.array.reverse)
  end

  def map(block)
    if block.frame.arity.float == 2
      result = []
      self.array[0].frame.range.zip(array[1].frame.range) do |items|
        result << block.frame._call(block, [Value(items[0]), Value(items[1])])
      end
    else
      result = self.array.map do |item|
        block.frame._call(block, [item])
      end
    end
    
    return create(result)
  end

  def fold(block, value)
    if value
      result = self.array.reduce(value) do |accum, item|
        block.frame._call(block, [accum, item])
      end
    else 
      result = self.array.reduce() do |accum, item|
        block.frame._call(block, [accum, item])
      end
    end
    
    return result
  end

  def join(string)
    return Value(self.array.join(string.frame.string))
  end

  def each_pair(block)
    (0...self.array.size).each do |i|
      ((i + 1)...self.array.size).each do |j|
        block.frame._call(block, [self.array[i], self.array[j]])
      end
    end
    
    return nil
  end

  def count(block)
    result = self.array.reduce(0) do |count, item|
      if block.frame._call(block, [item]).float == true
        count + 1
      else
        count
      end
    end
    
    return result
  end

  def repeat(number)
    return create(self.array * number.float)
  end
  
end

