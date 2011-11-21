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

    if @stream.peek().chr.match(/\n/)
      @stream.getc()
      
      return NewlineToken.new("\\n")
    end

    while @stream.peek().chr.match(/[ \t]/)
      @stream.getc()
    end
    
    #@token = NumberToken.read(@stream) || StringToken.read(@stream) || IdentifierToken.read(@stream)
    #@token = [NumberToken, StringToken, IdentifierToken].reduce(nil) do |token, klass|
    #  token || klass.read(@stream)
    #end
    
    read [NumberToken, StringToken, IdentifierToken]
    
    return @token
  end
  
  def next_token()
    token = peek_token()
    
    @token = nil
    
    return token
  end

end

