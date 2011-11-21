#
# parser/protos/parser.rb
#

require './core/frame.rb'
require './parser/protos/lexer.rb'

class Parser < Frame

  def initialize(lexer)
    @lexer = lexer
  end

  def parse()
    ExpressionParser.new(@lexer).frame.parse()
    
    #token = @lexer.frame.peek_token()
    #self.send(token.PARSER)
  end


end

class ExpressionParser < Parser

  def parse()
    #token = lexer.frame.peek_token()
    #token = NumberParser.parse(lexer)

    @lexer.frame.next_token()
  end

  def lit_number()
    token = @lexer.next_token()
    
    return token;
  end
  
end

class NumberParser < Parser

  def parse()
    if @lexer.frame.peek_token().instance_of? NumberToken
      token = @lexer.frame.next_token()
      return token
    end
  end

end

class IdentifierParser < Parser

  def parse()
    if @lexer.frame.peek_token().instance_of? IdentifierToken
      token = @lexer.frame.next_token()
      return token
    end
  end

end

