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
    $token = @lexer.frame.next_token()

    return $token
  end

  def expect(token)
    if !peek_token().instance_of?(token)
      puts "*** Unexpected #{peek_token()} after #{$token}"
      
      exit
    end

    return next_token()
  end

  def option(token)
    if !peek_token().instance_of?(token)
      return nil
    end
    
    return next_token()
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
      when NumberToken;      return next_token()
      when StringToken;      return next_token()
      when IdentifierToken;  return LocalMessage(next_token())
      when VerticalBarToken; return BlockParser.new(@lexer).frame.parse()
      #else expect(Token)
    end
  end

end


def MessageParser(lexer)
  return MessageParser.new(lexer).frame.parse()
end

class MessageParser < Parser

  def parse()
    case peek_token()
      when IdentifierToken; return SendMessage(next_token(), [])
    end
  end
  
end


class StatementParser < Parser

  def parse()
    expression = ExpressionParser(@lexer)
    
    expect(NewlineToken)
    
    return expression
  end
  
end


def ExpressionParser(lexer)
  return ExpressionParser.new(lexer).frame.parse()
end

class ExpressionParser < Parser

  def parse(messages = [])
    messages << PrimaryParser(@lexer)
    
    while message = MessageParser(@lexer)
      messages << message
    end
 
    return messages
  end
  
end


class BlockParser < Parser

  def parse(argnames = [])
    expect(VerticalBarToken)

    begin repeat(IdentifierToken) do |argname|
      argnames << argname
    end end while option(CommaToken)

    expect(VerticalBarToken)
    
    return BlockMessage(argnames, [ExpressionParser(@lexer)])
  end
  
end

