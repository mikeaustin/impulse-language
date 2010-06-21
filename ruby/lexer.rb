#
# lexer.rb
#
# Copyright 2010 Mike Austin
# Provided under MIT licence
#


module Impulse

# ---------------------------------------------------------------------------- #

  class Token

    attr :type
    attr :value

    def initialize(type, value)
      @type, @value = type, value
    end

  end

# ---------------------------------------------------------------------------- #

  class IOString
    
    def initialize(stream)
      @stream = stream
      @buffer = nil
      @pos = 0
    end

    def tty?
      return @stream.tty?
    end

    def peek()
      if @buffer == nil or @buffer[@pos] == nil
        @buffer = @stream.gets()
        @pos = 0
      end

      if @buffer == nil
        return nil
      end
      
      return @buffer[@pos]
    end

    def getc()
      char = self.peek()
      
      @pos += 1
      
      return char
    end

    def ungetc()
      if @pos > 0
        @pos -= 1
      end
    end
        
  end

# ---------------------------------------------------------------------------- #

  class Lexer

    def initialize(stream)
      @stream = IOString.new(stream)
    end

    def tty?
      return @stream.tty?
    end

    def read(match, type, convert, options = {:pre_inc => 0, :post_inc => 0})
      buffer = ""

      @stream.getc() if options[:pre_inc] > 0

      while @stream.peek().chr.match(match)
        buffer << @stream.getc()
      end rescue nil

      @stream.getc() if options[:post_inc] > 0
      
      return Token.new(type, buffer.send(convert))
    end

    def read_comment()
      buffer = ""

      @stream.getc()
      
      while @stream.peek().chr != "\n"
        buffer << @stream.getc()
      end
      
      return Token.new(:comment, buffer)
    end

    def read_number()
      buffer = ""

      while @stream.peek().chr.match(/\d/)
        buffer << @stream.getc()
      end rescue nil

      if @stream.peek().chr != "."
        return Token.new(:number, buffer.to_f)
      end

      buffer << @stream.getc()

      if !@stream.peek().chr.match(/\d/)
        @stream.ungetc()

        return Token.new(:number, buffer.to_f)
      end
      
      while @stream.peek().chr.match(/\d/)
        buffer << @stream.getc()
      end rescue nil
      
      return Token.new(:number, buffer.to_f)
    end

    def read_string()
      read(/[^"]/, :string, :to_s, :pre_inc => 1, :post_inc => 1)
    end

    def read_operator()
      read(/[\=\,\+\-\*\/\<\>\?]/, :operator, :to_sym)
    end

    def read_operator()
      buffer = ""

      while @stream.peek().chr.match(/[\=\,\+\-\*\/\<\>\?]/)
        buffer << @stream.getc()
      end rescue nil
      
      return Token.new(:operator, buffer.to_sym)
    end

    def read_ident()
      buffer = ""

      while @stream.peek().chr.match(/[a-zA-Z\d-]/)
        buffer << @stream.getc()
      end rescue nil

      if @stream.peek().chr == ':'
        @stream.getc()
        
        return Token.new(:keyword, (buffer + '_').to_sym)
      else
        return Token.new(:ident, buffer.to_sym)
      end
    end

    def peek_token()
      if @token
        return @token
      end

      if @stream.peek() == nil
        return nil
      end

      while @stream.peek().chr.match(/ /)
        @stream.getc()
      end rescue nil

      @token = case @stream.peek().chr
      when /\#/
        read_comment()
      when /\s/
        Token.new(:newline, @stream.getc().chr)
      when /[\=\+\-\*\/\<\>\?]/
        read_operator()
      when /[\d]/
        read_number()
      when /["]/
        read_string()
      when /[a-zA-Z]/
        read_ident()
      when /[\,]/
        @stream.getc()
        Token.new(:comma, :",")
      when /[\.]/
        @stream.getc()
        Token.new(:period, :".")
      when /[\:]/
        @stream.getc()
        Token.new(:colon, :":")
      when /[\|]/
        @stream.getc()
        Token.new(:block, :"|")
      when /[(]/
        @stream.getc()
        Token.new(:open_group, '(')
      when /[)]/
        @stream.getc()
        Token.new(:close_group, ')')
      when /[\[]/
        @stream.getc()
        Token.new(:open_array, '[')
      when /[\]]/
        @stream.getc()
        Token.new(:close_array, ']')
      else
        raise "*** Unknown character: '#{@stream.peek().chr}'"
      end

      return @token
    end
    
    def next_token()
      token = peek_token()

      @token = nil

      return token
    end
    
  end

end
