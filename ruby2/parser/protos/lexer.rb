#
# parser/protos/lexer.rb
#

require './core/frame.rb'
require './core/protos/string.rb'
require './parser/token.rb'


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

  READERS = [CommentToken, BlockCommentToken,
             OpenParenToken, CloseParenToken, OpenBracketToken, CloseBracketToken,
             LitNumberToken, LitStringToken, LitSymbolToken,
             CommaToken, DotOperatorToken, VerticalBarToken, DollarSignToken,
             AssignToken, OperatorToken, IdentifierToken,
             NewlineToken]

  def initialize(stream)
    @stream = stream
  end

  def peek_token()
    return @token if @token

    while @stream.peek().chr.match(/[ \t]/)
      @stream.getc()
    end rescue return nil
    
    @token = READERS.reduce(nil) do |token, klass|
      token || klass.read(@stream)
    end

    if !@token
      puts "*** Syntax Error: Invalid character '#{@stream.peek()}' after '#{@last_token.inspect}'"

      while @stream.peek().chr.match(/[^\n]/)
        @stream.getc()
      end rescue return nil
    end

    @last_token = @token
    
    return @token
  end
  
  def next_token()
    token = peek_token()
    
    @token = nil
    
    return token
  end

end

