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
    if !peek_token().is_a?(token)
      puts "*** Syntax Error: Unexpected token '#{peek_token()}'. #{message}"
      
      return nil
    end

    return next_token()
  end

  def option(token)
    if !peek_token().is_a?(token)
      return nil
    end
    
    return next_token()
  end

  def keyword(symbol)
    if !(peek_token().is_a?(IdentifierToken) && peek_token().float == symbol)
      return nil
    end
    
    next_token()
  end

  def repeat(token)
    if peek_token().is_a?(token)
      yield next_token()
    end
  end

  def precedence(operator)
    case operator
    when "*", "/", "%"
      return 5
    when "+", "-"
      return 6
    when ">", "<", ">=", "<="
      return 8
    when "=="
      return 9
    end
  end

end


def PrimaryParser(lexer)
  return PrimaryParser.new(lexer).frame.parse()
end

class PrimaryParser < Parser

  def parse()
    messages = []

    case peek_token()
      when LitNumberToken
        return [Value(next_token().float)]
      when LitStringToken
        string = next_token().frame.string
        
        return [StringProto.instance.frame.create(string)]
      when LitSymbolToken
        return [Value(next_token().float)]
      when IdentifierToken
        identifier = next_token()
        self_message = []
        
        if identifier.frame.proto.frame == SymbolProto.instance.frame && identifier.float == :"self"
          if option(DotOperatorToken)
            self_message += [LocalMessage(:self)]
            
            identifier = expect(IdentifierToken)
          end
        end
        
        if option(AssignToken)
          messages += ExpressionParser(@lexer)
          
          return self_message + [AssignMessage(identifier.float, ExpressionProto(messages))]
        end
        
      	return self_message + [LocalMessage(identifier.float)]
      when VerticalBarToken
        return BlockParser.new(@lexer).frame.parse()
      when OpenParenToken
        return SubExpressionParser.new(@lexer).frame.parse()
      when OpenBracketToken
        return ArrayParser(@lexer)
    end
    
    return []
  end

end


def MessageParser(lexer)
  return MessageParser.new(lexer).frame.parse()
end

class MessageParser < Parser

  def parse()
    case peek_token()
      when IdentifierToken
      	return [SendMessage(next_token().float, [])]
      when KeywordToken
        keyword = next_token()
        msg_args = [ExpressionProto(ExpressionParser(@lexer))]

        begin repeat(KeywordToken) do |keyword|
          msg_args << ExpressionProto(ExpressionParser(@lexer))
        end end while option(CommaToken)

        return [SendMessage(keyword.float, msg_args)]
      when OperatorToken
        operator = expect(OperatorToken)
        argument = ExpressionParser(@lexer, method(:BinaryMessageParser))
        
        return [SendMessage(operator.float, [ExpressionProto(argument)])]
      when OpenBracketToken
        arguments = []
        
        expect(OpenBracketToken)
        if !peek_token().is_a? CloseBracketToken
          begin
            arguments << ExpressionProto(ExpressionParser(@lexer))
          end while option(CommaToken)
        end
        expect(CloseBracketToken)

        if option(AssignToken)
          messages = ExpressionParser(@lexer)
          
          return [SendMessage(:slice_assign, [arguments[0], ExpressionProto(messages)])]
        end
        
        return [SendMessage(:slice, arguments)]
      #when NewlineToken
      #	return []
    end
    
    return []
  end
  
end


def BinaryMessageParser(lexer)
  return BinaryMessageParser.new(lexer).frame.parse()
end

class BinaryMessageParser < Parser

  def parse()
    case peek_token()
      when OperatorToken
        return []
      else
        return MessageParser(@lexer)
    end
    
    return []
  end
  
end


def StatementParser(lexer)
  return StatementParser.new(lexer).frame.parse()
end

class StatementParser < Parser

  def parse()
    messages = []

    option(CommentToken)

    messages += PrimaryParser(@lexer)

    begin
      while (message = MessageParser(@lexer)) != []
        messages += message
      end
    end while option(DollarSignToken)

    option(CommentToken)

    expect(NewlineToken, "Expected an expression [2].")

    #if $file && peek_token() == nil
    if $file && !peek_token()
      return nil
    end

    if messages == []
      messages << nil
    end
      
    return messages
  end
  
end



class SubExpressionParser < Parser

  def parse()
    messages = []
    expression = []

    expect(OpenParenToken, "Expected '('")
    
    #if !peek_token().is_a? CloseParenToken
    #  expression = ExpressionParser(@lexer)
    #end

    messages = PrimaryParser(@lexer)

    begin
      while (message = MessageParser(@lexer)) != []
        messages += message
      end
    end while option(DollarSignToken)
    
    expect(CloseParenToken, "Expected ')'")

    #messages += expression

    if messages == []
       messages << NothingProto.instance
    end
    
    return messages
  end
  
end


def ExpressionParser(lexer, messageParser = method(:MessageParser))
  return ExpressionParser.new(lexer, messageParser).frame.parse()
end

class ExpressionParser < Parser

  def initialize(lexer, messageParser)
    super(lexer)
    
    @messageParser = messageParser
  end

  def parse()
    messages = PrimaryParser(@lexer)

    if messages == []
      expect(UnknownToken, "Expected an expression [3].")
    end

    while (message = @messageParser.call(@lexer)) != []
      messages += message
    end

    return messages
  end
  
end


def ArrayParser(lexer)
  return ArrayParser.new(lexer).frame.parse()
end

class ArrayParser < Parser

  def parse()
    items = []

    expect(OpenBracketToken, "Expcected '['")

    if !peek_token().is_a? CloseBracketToken
      begin
        items << ExpressionProto(ExpressionParser(@lexer))
      end while option(CommaToken)
    end
    
    expect(CloseBracketToken, "Expected ']'")
    
    return [ArrayMessage(items)]
  end

end


class BlockParser < Parser

  def parse()
    argnames = []

    expect(VerticalBarToken, "Expected '|'")

    begin repeat(IdentifierToken) do |argname|
      argnames << argname.float
    end end while option(CommaToken)
    
    expect(VerticalBarToken, "Expected block arguments.")

    expressions = []

    if keyword(:do)
      expect(NewlineToken)
      print "| " if !$file

      begin
        expressions << ExpressionProto(StatementParser(@lexer))
        print "| " if !$file
      end while !keyword(:end)
    else
      expressions << ExpressionProto(ExpressionParser(@lexer))
    end
    
    return [BlockMessage(argnames, expressions)]
  end
  
end

