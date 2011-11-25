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
    return $token = @lexer.frame.next_token()
  end

  def expect(token, message = "")
    if !peek_token().instance_of?(token)
      puts "*** Syntax Error | Unexpected token #{peek_token()} after #{$token}. #{message}"
      
      exit
    end

    return next_token()
  end

  def expect2(parser, message = "")
    token = parser.new(@lexer).frame.parse()
    
    if !token
      #expect(Token, message)
    end
    
    return token
   end

  def option(token)
    if !peek_token().instance_of?(token)
      return nil
    end
    
    return next_token()
  end

  def option2(parser)
    return parser.new(@lexer).frame.parse()
  end

  def repeat(token)
    if peek_token().instance_of?(token)
      yield next_token()
    end
  end

end


def PrimaryParser(lexer)
  return PrimaryParser.new(lexer).frame.parse()
end

class PrimaryParser < Parser

  def parse()
    case peek_token()
      when LitNumberToken;   return next_token()
      when LitStringToken;   return next_token()
      when IdentifierToken;  return LocalMessage(next_token())
      when VerticalBarToken; return BlockParser.new(@lexer).frame.parse()
      else expect(Token, "Expected an expression.")
    end
  end

end


def MessageParser(lexer)
  return MessageParser.new(lexer).frame.parse()
end

class MessageParser < Parser

  def parse()
    case peek_token()
      when IdentifierToken; return SendMessage(next_token().float, [])
      when NewlineToken;    return nil;
      else expect(Token, "Expected a message.")
    end
  end
  
end


class StatementParser < Parser

  def parse()
    messages = ExpressionParser(@lexer)
    
    expect(NewlineToken, "Expected a message.")
    
    return messages
  end
  
end


def ExpressionParser(lexer)
  return ExpressionParser.new(lexer).frame.parse()
end

class ExpressionParser < Parser

  def parse(messages = [])
    messages << expect2(PrimaryParser)
    
    while message = option2(MessageParser)
      messages << message
    end
 
    return messages
  end
  
end


class BlockParser < Parser

  def parse(argnames = [])
    expect(VerticalBarToken, "foo")

    begin repeat(IdentifierToken) do |argname|
      argnames << argname
    end end while option(CommaToken)

    expect(VerticalBarToken, "Expected block arguments.")
    
    return BlockMessage(argnames, [ExpressionParser(@lexer)])
  end
  
end
