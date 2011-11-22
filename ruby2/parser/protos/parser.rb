#
# parser/protos/parser.rb
#

require './core/frame.rb'
require './parser/protos/lexer.rb'

class Parser < Frame

  def initialize(lexer)
    @lexer = lexer
  end

  def peek_token()
    return @lexer.frame.peek_token()
  end
  
  def next_token()
    return @lexer.frame.next_token()
  end

  def expect(token)
    if peek_token.instance_of?(Token) && !peek_token().instance_of?(token)
      puts "*** Expected #{token}, found #{next_token()}"
      
      return nil
    end

    if peek_token().instance_of? Parser
      return parser.new(@lexer).frame.parse()
    end
    
    if !peek_token().instance_of? NewlineToken
      return next_token()
    end
  end

  def option(token)
    if !peek_token().instance_of? token
      return nil
    end
    
    return next_token()
  end

  def switch()
    return peek_token()
  end

  def parse(type)
    return type.new(@lexer).frame.parse()
  end

end

class StatementParser < Parser

  def parse()
    expression = ExpressionParser(@lexer)

    #expect(NewlineToken)
    
    return expression
  end
  
end


def ExpressionParser(lexer)

  return ExpressionParser.new(lexer).frame.parse()

end

class ExpressionParser < Parser

  def parse()
    case switch()
      when NumberToken;		return next_token()
      when IdentifierToken;	return next_token()
    end
  end    
  
end

