#
# runtime.rb
#
# Copyright 2010 Mike Austin
# Provided under MIT licence
#

module Impulse

# ValueProto is the base class of all prototypes
# ---------------------------------------------------------------------------- #

  class Trait
  end

  class ValueProto

    attr :proto
    attr :slots
    attr :traits
    attr :eval

    @instance = nil

    def initialize(proto)
      init(proto)
    end

    def init(proto)
      @value = nil
      @proto = proto
      @slots = proto.slots.clone() rescue {}
      @traits = []
      @eval = self.method(:eval_)
    end

    def init_slots()
    end

    def self.instance()
      if !@instance
        @instance = self.new()
      end
      
      return @instance
    end

    def _clone()
      object = self.clone()
      object.init(self)

      return object
    end

    def eval_(symbol, args, context)
      return self
    end

    def send(symbol, args, context)
      self.send_helper(symbol, self, args, context)
    end

    def send_helper(symbol, receiver, args, context)
      method = self.slots[symbol]

      if method
        return method.eval.call(receiver, args, context)
      end

      self.traits.each do |trait|
        method = trait.slots[symbol]
        if method
          return method.eval.call(receiver, args, context)
        end
      end

      if self.proto
        return self.proto.send_helper(symbol, receiver, args, context)
      end
      
      raise "*** No such method: '#{receiver.type}.#{symbol}'"
    end

  end

# ---------------------------------------------------------------------------- #

  class VoidProto < ValueProto

    def initialize()
      super(nil)
    end

  end

# ---------------------------------------------------------------------------- #

  class ObjectProto < ValueProto

    def initialize()
      super(nil)
    end

    def init_slots()
      @traits << ValueProto.new(nil)

      @slots[:clone] = MethodValue.new do |receiver, args, context|
        receiver.clone()
      end

      @traits[0].slots[:type] = MethodValue.new do |receiver, args, context|
        SymbolValue.new(:"<object>")
      end

      @traits[0].slots[:inspect] = MethodValue.new do |receiver, args, context|
        receiver.inspect()
      end

      @traits[0].slots[:"proto"] = MethodValue.new do |receiver, args, context|
        receiver.proto
      end

      @traits[0].slots[:"object-id"] = MethodValue.new do |receiver, args, context|
        receiver.object_id
      end

      @traits[0].slots[:"slots"] = MethodValue.new do |receiver, args, context|
        ArrayProto.instance._clone(receiver.slots.map do |slot|
          slot
        end)
      end

      @traits[0].slots[:assign] = MethodValue.new do |receiver, args, context|
        symbol = args[0].value
        value  = args[1]

        if !value.is_a? VoidProto
          context.slots[symbol] = value
        end
      end
    end

  end

# ---------------------------------------------------------------------------- #

  class MethodProto < ValueProto

    def initialize()
      super(ObjectProto.instance)
    end

  end

  class MethodValue < ValueProto

    def initialize(&block)
      super(MethodProto.instance)

      @block = block

      @eval = self.method(:eval_)
    end

    def eval_(receiver, args, context)
      @block.call(receiver, args, context)
    end

  end

# ---------------------------------------------------------------------------- #

  class SymbolProto < ValueProto

    def initialize()
      super(ObjectProto.instance)

      @slots[:==] = MethodValue.new do |receiver, args, context|
        BooleanValue.new(receiver.value == args[0].value)
      end
    end

  end

  class SymbolValue < ValueProto

    attr :value

    def initialize(value)
      super(SymbolProto.instance)

      @value = value
    end

    def to_s()
      return @value.to_s
    end

  end

# ---------------------------------------------------------------------------- #

  class NumberProto < ValueProto

    def initialize()
      super(ObjectProto.instance)
    end

    def init_slots()
      @slots[:type] = MethodValue.new do |receiver, args, context|
        SymbolValue.new(:"<number>")
      end
      
      @slots[:sin] = MethodValue.new do |receiver, args, context|
        NumberValue.new(Math.sin(receiver.value))
      end

      @slots[:pow_] = MethodValue.new do |receiver, args, context|
        NumberValue.new(receiver.value ** args[0].value)
      end

      @slots[:+] = MethodValue.new do |receiver, args, context|
        NumberValue.new(receiver.value + args[0].value)
      end

      @slots[:-] = MethodValue.new do |receiver, args, context|
        NumberValue.new(receiver.value - args[0].value)
      end

      @slots[:*] = MethodValue.new do |receiver, args, context|
        NumberValue.new(receiver.value * args[0].value)
      end

      @slots[:/] = MethodValue.new do |receiver, args, context|
        NumberValue.new(receiver.value / args[0].value)
      end

      @slots[:==] = MethodValue.new do |receiver, args, context|
        BooleanValue.new(receiver.value == args[0].value)
      end

      @slots[:<] = MethodValue.new do |receiver, args, context|
        BooleanValue.new(receiver.value < args[0].value)
      end

      @slots[:"<="] = MethodValue.new do |receiver, args, context|
        BooleanValue.new(receiver.value <= args[0].value)
      end

      @slots[:>] = MethodValue.new do |receiver, args, context|
        BooleanValue.new(receiver.value > args[0].value)
      end

      @slots[:even] = MethodValue.new do |receiver, args, context|
        BooleanValue.new(receiver.value % 2 == 0)
      end

      @slots[:odd] = MethodValue.new do |receiver, args, context|
        BooleanValue.new(receiver.value % 2 != 0)
      end

      @slots[:".."] = MethodValue.new do |receiver, args, context|
        RangeProto.new(receiver.value, args[0].value)
      end

      @slots[:foo_] = MethodValue.new do |receiver, args, context|
        NumberValue.new(receiver.value * args[0].value + receiver.value * args[1].value)
      end

      @slots[:clone] = MethodValue.new do |receiver, args, context|
        receiver.class.new(receiver.value)
      end

      @slots[:each_] = MethodValue.new do |receiver, args, context|
        (receiver.value).times do |i|
          args[0].send(:eval_, [NumberValue.new(i)], context)
        end
        
        VoidProto.instance
      end
    end

  end

  class NumberValue < ValueProto

    attr :value

    def initialize(value = 0)
      super(NumberProto.instance)

      @value = value
    end

    def to_s()
      return value.to_s
    end

  end

# ---------------------------------------------------------------------------- #

  class StringProto < ValueProto

    attr :value, true

    def initialize()
      super(ObjectProto.instance)

      @value = ""

      @slots[:type] = MethodValue.new do |receiver, args, context|
        SymbolValue.new(:"<string>")
      end

      @slots[:size] = MethodValue.new do |receiver, args, context|
        NumberValue.new(receiver.value.size)
      end

      @slots[:clone] = MethodValue.new do |receiver, args, context|
        receiver.clone()
      end
    end

    def _clone(value)
      object = super()
      
      object.value = value
      
      return object
    end

    def to_s()
      return "\"" + value + "\""
    end

  end

# ---------------------------------------------------------------------------- #

  class BooleanProto < ValueProto

    def initialize()
      super(ObjectProto.instance)
    end

    def init_slots()
      @slots[:type] = MethodValue.new do |receiver, args, context|
        SymbolValue.new(:"<boolean>")
      end

      @slots[:size] = MethodValue.new do |receiver, args, context|
        NumberValue.new(receiver.value.size)
      end

      @slots[:"?"] = MethodValue.new do |receiver, args, context|
        if receiver.value
          args[0].send(:eval_, [], context)
        elsif args.size > 1
          args[1].send(:eval_, [], context)
        else
          VoidProto.instance
        end
      end

      @slots[:clone] = MethodValue.new do |receiver, args, context|
        receiver.class.new(receiver.value.clone)
      end
    end

  end

  class BooleanValue < ValueProto

    attr :value

    def initialize(value = false)
      super(BooleanProto.instance)

      @value = value
    end

    def to_s()
      return value.to_s
    end

  end

# ---------------------------------------------------------------------------- #

  class BlockCreator < ValueProto
    
    def initialize(args, exprs)
      super(ObjectProto.instance)

      @args, @exprs = args, exprs

      @eval = self.method(:eval_)
    end
    
    def eval_(receiver, args, context)
      #new_context = ValueProto.new(context)
      
      return Block.new(@args, @exprs, context)
    end
    
  end

  class Block < ValueProto

    def value
      return @exprs
    end

    def initialize(args, exprs, context)
      super(ObjectProto.instance)

      @args, @exprs, @context = args, exprs, context

      @traits << ValueProto.new(nil)

      @traits[0].slots[:type] = MethodValue.new do |receiver, args, context|
        SymbolValue.new(:"<block>")
      end

      @slots[:eval] = MethodValue.new do |receiver, args, context|
        value = nil
        
        @exprs.each do |expr|
          value = expr.eval.call(@context, [], @context)
        end
        
        value
      end

      @slots[:eval_] = MethodValue.new do |receiver, args, context|
        block_context = ValueProto.new(@context)

        @args.zip(args) do |symbol, value|
          block_context.slots[symbol] = value.dup()
        end
        
        value = nil
        
        @exprs.each do |expr|
          value = expr.eval.call(block_context, [], block_context)
        end
        
        value
      end

      @slots[:args] = MethodValue.new do |receiver, args, context|
        ArrayProto.instance._clone(@args)
      end
    end

  end

# ---------------------------------------------------------------------------- #

  class RangeCreator < ValueProto
    
    def initialize(from, to)
      super(ObjectProto.instance)

      @from, @to = from, to

      @eval = self.method(:eval_)
    end
    
    def eval_(receiver, args, context)
      from = @from.eval.call(context, [], context).value
      to = @to.eval.call(context, [], context).value
      
      return RangeProto.new(from, to)
    end
    
  end

  class RangeProto < ValueProto

    attr :from
    attr :to

    def value
      from..to
    end

    def initialize(from, to)
      super(ObjectProto.instance)

      @from, @to = from.to_i, to.to_i

      @traits << ValueProto.new(nil)

      @traits[0].slots[:type] = MethodValue.new do |receiver, args, context|
        SymbolValue.new(:"<range>")
      end

      @slots[:each] = MethodValue.new do |receiver, args, context|
        (receiver.from..receiver.to).each do |i|
          args[0].eval.call(context, [NumberValue.new(i)], context)
        end
      end

      @slots[:each_] = MethodValue.new do |receiver, args, context|
        (receiver.from..receive.to).each do |obj|
          args[0].send(:eval_, [NumberValue.new(obj)], context)
        end
        
        VoidProto.instance
      end

      @slots[:map_] = MethodValue.new do |receiver, args, context|
        values = []
        
        (receiver.from..receiver.to).each do |obj|
          value = args[0].send(:eval_, [NumberValue.new(obj)], context)

          if !value.is_a? VoidProto
            values << value
          end
        end

        ArrayProto.instance._clone(values)
      end

      @slots[:"fold_"] = MethodValue.new do |receiver, args, context|
        accum = args[1]
        
        receiver.value.each do |value|
          value = NumberValue.new(value) if receiver.is_a? RangeProto
          value = args[0].send(:eval_, [accum, value], context)
          
          if !value.is_a? VoidProto
            accum = value
          end
        end
        
        accum
      end

      @slots[:size] = MethodValue.new do |receiver, args, context|
        NumberValue.new(@to - @from + 1)
      end
    end

    def to_s()
      return "[#{from}..#{to}]"
    end

  end

# ---------------------------------------------------------------------------- #

  class ArrayCreator < ValueProto
    
    def initialize(array)
      super(ObjectProto.instance)

      @array = array

      @eval = self.method(:eval_)
    end
    
    def eval_(receiver, args, context)
      values = @array.map do |obj|
        obj.eval.call(context, [], context)
      end
      
      return ArrayProto.instance._clone(values)
    end
    
  end

  class Enumerable < Trait

    def initialize()
      @slots[:map_] = MethodValue.new do |receiver, args, context|
        values = []
        
        receiver.send(:each_) do |obj|
          values << args[0].send(:eval_, [obj], context)
        end

        ArrayProto.instance._clone(values)
      end
    end
    
  end

  class ArrayProto < ValueProto

    attr :value, true

    def initialize()
      super(ObjectProto.instance)

      @value = []

      @slots[:type] = MethodValue.new do |receiver, args, context|
        SymbolValue.new(:"<array>")
      end

      @slots[:size] = MethodValue.new do |receiver, args, context|
        NumberValue.new(receiver.value.size)
      end

      @slots[:each_] = MethodValue.new do |receiver, args, context|
        receiver.value.each do |obj|
          args[0].send(:eval_, [obj], context)
        end
        
        VoidProto.instance
      end

      @slots[:map_] = MethodValue.new do |receiver, args, context|
        values = []
        
        receiver.value.each do |obj|
          value = args[0].send(:eval_, [obj], context)

          if !value.is_a? VoidProto
            values << value
          end
        end

        ArrayProto.instance._clone(values)
      end

      @slots[:"zip_"] = MethodValue.new do |receiver, args, context|
        values = []
        
        receiver.value[0].value.zip(receiver.value[1].value) do |obj1, obj2|
          obj1 = NumberValue.new(obj1) if receiver.value[0].value.is_a? Range
          obj2 = NumberValue.new(obj2) if receiver.value[1].value.is_a? Range

          value = args[0].send(:eval_, [obj1, obj2], context)

          if !value.is_a? VoidProto
            values << value
          end
        end

        ArrayProto.instance._clone(values)
      end

      @slots[:zip] = MethodValue.new do |receiver, args, context|
        values = []
        
        receiver.value[0].value.zip(receiver.value[1].value) do |obj1, obj2|
          values << ArrayProto.instance._clone([obj1, obj2])
        end

        ArrayProto.instance._clone(values)
      end

      @slots[:sort_] = MethodValue.new do |receiver, args, context|
        values = receiver.value.sort do |a, b|
          args[0].send(:eval_, [a, b], context).value ? 1 : -1
        end

        ArrayProto.instance._clone(values)
      end

      @slots[:"fold_"] = MethodValue.new do |receiver, args, context|
        accum = args[1]
        
        receiver.value.each do |value|
          value = args[0].send(:eval_, [accum, value], context)
          
          if !value.is_a? VoidProto
            accum = value
          end
        end
        
        accum
      end
    end

    def _clone(value)
      object = super()
      
      object.value = value
      
      return object
    end

    def to_s()
      "[" + @value.join(", ") + "]"
    end

  end

# ---------------------------------------------------------------------------- #

  class LobbyProto < ValueProto
    def initialize()
      super(ObjectProto.instance)
    end

    def value
      true
    end

    def init_slots()
      @slots[:print_] = MethodValue.new do |receiver, args, context|
        puts args[0].value
        
        VoidProto.instance
      end

      @slots[:"?"] = MethodValue.new do |receiver, args, context|
        ArrayProto.instance._clone(args[0].value[0].code)
      end

      @slots[:help] = MethodValue.new do |receiver, args, context|
        puts "No help yet!"
        
        VoidProto.instance
      end

      @slots[:exit] = MethodValue.new do |receiver, args, context|
        exit()
      end

      @slots[:PI] = NumberValue.new(Math::PI)

      @slots[:false] = BooleanValue.new(false)
      @slots[:true] = BooleanValue.new(true)
      
      @slots[:"lobby"] = LobbyProto.instance
      
      #@slots[:"<number>"] = NumberProto.instance
    end

  end

# ---------------------------------------------------------------------------- #

ObjectProto.instance.init_slots()
MethodProto.instance.init_slots()
NumberProto.instance.init_slots()
BooleanProto.instance.init_slots()
#StringProto.instance.init_slots()
ArrayProto.instance.init_slots()
#RangeProto.instance.init_slots()
LobbyProto.instance.init_slots()

end
