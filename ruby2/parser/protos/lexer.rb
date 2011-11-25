#
# parser/protos/lexer.rb
#

require './parser/token.rb'
require './core/frame.rb'
require './core/protos/string.rb'

#
# class IO
#

class IO

  def peek()
    char = self.getc()
    self.ungetc(char)
    
    return char
  end
  
end

#
# class Lexer
#

class Lexer < Frame

  def initialize(stream)
    @stream = stream
  end

  def read(klasses)
    return klasses.reduce(nil) do |token, klass|
      token || klass.read(@stream)
    end
  end

  def peek_token()
    return @token if @token

    return nil if @stream.peek() == nil

    while @stream.peek().chr.match(/[ \t]/)
      @stream.getc()
    end rescue nil
    
    @token = read [NewlineToken, NumberToken, StringToken, IdentifierToken, CommaToken, VerticalBarToken]
    
    return @token
  end
  
  def next_token()
    token = peek_token()
    
    @token = nil
    
    return token
  end

end

