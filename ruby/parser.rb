#
# parser.rb
#
# Copyright 2010 Mike Austin
# Provided under MIT licence
#

require 'lexer'

module Impulse

# ---------------------------------------------------------------------------- #

  class Expression < ValueProto

    attr :code

    def initialize(code)
      super(ObjectProto.new)

      @code = code
      @eval = method(:eval_)
    end

    def eval_(receiver, args, context)
      @code.each do |value|
        receiver = value.eval.call(receiver, [], context)
      end

      return receiver
    end

  end

# ---------------------------------------------------------------------------- #

  class MessageProto < ValueProto

    attr :symbol

    def initialize(symbol, args)
      super(ObjectProto.new)

      @symbol = symbol
      @args = args

      @eval = self.method(:eval_)
    end

    def eval_(receiver, args, context)
      margs = @args.map do |arg|
        arg.eval.call(context, [], context)
      end

      receiver.send(@symbol, margs, context)
    end

  end

  class LazyValue < ValueProto

    attr :value

    def initialize(symbol, args)
      super(ObjectProto.new)

      @value = value
      
      @slots[:eval_] = MethodValue.new do |receiver, args, context|
        @value
      end
      
    end

  end

# ---------------------------------------------------------------------------- #

  class Parser

    def initialize(lexer)
      @lexer = lexer
    end

    def tty?
      return @lexer.tty?
    end

    def expect_type(type)
      token = @lexer.next_token()

      if token.type != type
        raise "*** Error: expected '#{type}'"
      end
    end

    def expect_value(value)
      token = @lexer.next_token()

      if token.value != value
        raise "*** Error: expected '#{value}'"
      end
    end

    def emit_number()
      number = @lexer.next_token().value
      
      return [NumberValue.new(number)]
    end

    def emit_string()
      string = @lexer.next_token().value
      
      return [StringProto.instance._clone(string)]
    end

    def emit_ident()
      symbol = @lexer.next_token().value
      
      if @lexer.peek_token().value == :"="
        expect_value(:"=")

        return [MessageProto.new(:assign, [SymbolValue.new(symbol), nested_expression()])]
      else
        return [MessageProto.new(symbol, [])]
      end
    end

    def keyword()
      args = []

      symbol = @lexer.next_token().value
      args << nested_expression()

      while @lexer.peek_token().value == :","
        expect_value(:",")
        expect_type(:keyword)
        
        args << nested_expression()
      end

      return [MessageProto.new(symbol, args)]
    end

    def emit_operator()
      symbol = @lexer.next_token().value

      if symbol == :"=="
        return [MessageProto.new(symbol, [nested_expression()])]
      elsif symbol == :"?"
        true_block  = nested_expression()
        args = [BlockCreator.new([], [true_block])]
        
        if @lexer.peek_token().value == :":"
          expect_value(:":")
          false_block = nested_expression()
          args << BlockCreator.new([], [false_block])
        end
        
        return [MessageProto.new(symbol, args)]
      else
        return [MessageProto.new(symbol, [binary_expression()])]
      end
    end

    def array()
      expect_type(:open_array)
      
      values = []
      
      if @lexer.peek_token().type != :close_array
        values << nested_expression()

        if @lexer.peek_token().value == :"."
          @lexer.next_token();
          
          if @lexer.peek_token().value == :"."
            @lexer.next_token()
            
            values << nested_expression()
            
            expect_type(:close_array)
            
            return [RangeCreator.new(values[0], values[1])]
          else
            @lexer.ungetc();
          end
        end

        while @lexer.peek_token().value == :","
          expect_value(:",")
          
          values << nested_expression()
        end
      end

      expect_type(:close_array)

      return [ArrayCreator.new(values)]
    end

    def access()
        args = []

        expect_type(:open_array)
      
        if @lexer.peek_token().type != :close_array
            args << nested_expression()

            while @lexer.peek_token().value == :","
                expect_value(:",")
          
                args << nested_expression()
            end
        end

        expect_type(:close_array)

        return [MessageProto.new(:"eval_", args)]
    end

    def block()
      expect_value(:"|")

      args = []
      
      if @lexer.peek_token().value != :"|"
        args << @lexer.next_token().value
        
        while @lexer.peek_token().value == :","
          expect_value(:",")
          
          args << @lexer.next_token().value
        end
      end

      expect_value(:"|")

      exprs = []
      
      if @lexer.peek_token().type == :newline
        expect_type(:newline)

        print "] " if @lexer.tty?

        while @lexer.peek_token().value != :"end"
          exprs << expression()

          print "] " if @lexer.tty?
        end
        
        expect_value(:"end")
      else
        exprs << nested_expression()
      end

      return [BlockCreator.new(args, exprs)]
    end

    def message()
      token = @lexer.peek_token()
      code = []

      code += case token.type
      when :ident
        emit_ident()
      when :keyword
        keyword()
      when :operator
        emit_operator()
      when :open_array
        access()
      else
        []
      end
      
      return code.size > 0 ? code : nil
    end

    def expression()
      expr = nested_expression()
      
      if @lexer.peek_token() == nil
        return nil
      end

      if @lexer.peek_token().type == :comment
        @lexer.next_token()
      end

      expect_type(:newline)
      
      return expr
    end

    def nested_expression(options = {:binary => false})
      token = @lexer.peek_token()
      code = []

      code += case token.type
      when :eof
        return nil
      when :number
        emit_number()
      when :string
        emit_string()
      when :ident
        emit_ident()
      when :keyword
        keyword()
      when :open_array
        array()
      when :open_group
        expect_value('(')
        expr = nested_expression()
        expect_value(')')

        [expr]
      when :block
        block()
      else
        []
      end

      if options[:binary]
        while @lexer.peek_token().type != :operator && message = message()
          code += message
        end
      else
        while message = message()
          code += message
        end
      end
      
      return code.size > 1 ? Expression.new(code) : code.size > 0 ? code[0] : VoidProto.instance
    end

    def binary_expression()
      return nested_expression(:binary => true)
    end

  end

end
