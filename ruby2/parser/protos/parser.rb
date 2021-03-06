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

      next_token()
      
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

  def peek_keyword(symbol)
    if !(peek_token().is_a?(IdentifierToken) && peek_token().float == symbol)
      return nil
    end
    
    peek_token()
  end

  def repeat(token)
    if peek_token().is_a?(token)
      yield next_token()
    end
  end

  def precedence(operator)
    case operator.float
    when :"::"
      return 0
    when :"*", :"/", :"%"
      return 5
    when :"+", :"-"
      return 6
    when :">", :"<", :">=", :"<="
      return 8
    when :"=="
      return 9
    else
      return 0
    end
  end

end


def PrimaryParser(lexer)
  return PrimaryParser.new(lexer).frame.parse()
end

class PrimaryParser < Parser

  def parse()
    messages = []

    if peek_keyword(:method)
      return MethodParser.new(@lexer).frame.parse()
    elsif peek_keyword(:object)
      return ObjectParser.new(@lexer).frame.parse()
    elsif peek_keyword(:field)
      return FieldParser.new(@lexer).frame.parse()
    end

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
        
        if identifier.frame_is_a(SymbolProto.instance) && identifier.float == :"self"
          if option(DotOperatorToken)
            self_message += [LocalMessage(:"self")]
            
            identifier = expect(IdentifierToken)
          end
        end
=begin        
        if option(AssignToken)
          messages += ExpressionParser(@lexer)
          
          return self_message + [AssignMessage(identifier.float, ExpressionProto(messages))]
        end
=end
=begin
        if option(ColonColonToken)
          messages2 = ExpressionParser(@lexer)

          return [TypeMessage(LocalMessage(identifier.float), ExpressionProto(messages2))]
        end
=end
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


def MessageParser(lexer, precedence = 0)
  return MessageParser.new(lexer).frame.parse(precedence)
end

class MessageParser < Parser

  def parse(precedence)
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
        argument = ExpressionParser(@lexer, method(:BinaryMessageParser), precedence(operator))
        
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
    end
    
    return []
  end
  
end


def BinaryMessageParser(lexer, precedence = 0)
  return BinaryMessageParser.new(lexer).frame.parse(precedence)
end

class BinaryMessageParser < Parser

  def parse(operator_prec)
    peek_token = peek_token()
    
    if peek_token.is_a?(OperatorToken) && precedence(peek_token) > operator_prec
      return []
    end

    return MessageParser(@lexer)
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

    if option(ColonColonToken)
      messages2 = ExpressionParser(@lexer)

      messages = [TypeMessage(messages[0], ExpressionProto(messages2))]
    end

    if option(AssignToken)
      messages2 = ExpressionParser(@lexer)

      messages = [PatternMessage(messages[0], ExpressionProto(messages2))]
    end

    option(CommentToken)

    expect(NewlineToken, "Expected an expression [2].")

    if $file && !peek_token()
      return nil
    end
#p messages
    return messages
  end
  
end


class SubExpressionParser < Parser

  def parse(precedence = 0)
    messages = []
    expression = []

    expect(OpenParenToken, "Expected '('")
    
    messages = PrimaryParser(@lexer)

    begin
      while (message = MessageParser(@lexer, precedence)) != []
        messages += message
      end
    end while option(DollarSignToken)
    
    expect(CloseParenToken, "Expected ')'")

    if messages == []
       messages << NothingProto.instance
    end
    
    return messages
  end
  
end


def ExpressionParser(lexer, messageParser = method(:MessageParser), precedence = 0)
  return ExpressionParser.new(lexer, messageParser).frame.parse(precedence)
end

class ExpressionParser < Parser

  def initialize(lexer, messageParser)
    super(lexer)
    
    @messageParser = messageParser
  end

  def parse(precedence)
    messages = PrimaryParser(@lexer)

    if messages == []
      expect(UnknownToken, "Expected an expression [3].")
    end

    while (message = @messageParser.call(@lexer, precedence)) != []
      messages += message
    end

    if option(ColonColonToken)
      messages2 = ExpressionParser(@lexer)

      messages = [TypeMessage(messages[0], ExpressionProto(messages2))]
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
        expr = ExpressionParser(@lexer)
        
        if expr.size > 1
          items << ExpressionProto(expr)
        else
          items << expr[0]
        end
        #items << ExpressionProto(ExpressionParser(@lexer))
      end while option(CommaToken)
    end
    
    expect(CloseBracketToken, "Expected ']'")
    
    return [ArrayMessage(items)]
  end

end


class FileParser < Parser

  def parse()
    expressions = []

    parser = StatementParser.new(@lexer)

    begin
      messages = parser.frame.parse()

      if messages != [] && messages != nil
        expressions << ExpressionProto(messages)
      end
    end while messages
    
    return expressions
  end

end


class BodyParser < Parser

  def parse()
    expect(NewlineToken)
    print "| " if !$file

    expressions = []

    begin
      messages = StatementParser(@lexer)
        
      if messages != []
        expressions << ExpressionProto(messages)
      end
        
      print "| " if !$file
    end while !keyword(:end)
      
    return expressions
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
      expressions = BodyParser.new(@lexer).frame.parse()
    else
      expressions = [ExpressionProto(ExpressionParser(@lexer))]
    end
    
    return [BlockMessage(argnames, expressions)]
  end
  
end


class ObjectParser < Parser

  def parse()
    argnames = [:"obj"]
    proto_name = nil
    
    if keyword(:object)
      name = expect(IdentifierToken)

      if option(ColonColonToken)
        proto_name = expect(IdentifierToken)
      end

      expressions = BodyParser.new(@lexer).frame.parse()

      proto = (proto_name && LocalMessage(proto_name.float)) || ObjectProto.instance
      
      return [SendMessage(:"add-object:", [Value(name), proto, BlockMessage(argnames, expressions)])]
    end
  end

end


class FieldParser < Parser

  def parse()
    if keyword(:field)
      name = expect(IdentifierToken)

      messages = []

      if option(AssignToken)
        messages += ExpressionParser(@lexer)
          
        return [LocalMessage(:"self"), SendMessage(:"add-field:", [Value(name), ExpressionProto(messages)])]
      end

      return [LocalMessage(:"self"), SendMessage(:"add-field:", [Value(name), NilProto.instance])]
    end
  end
  
end


class MethodParser < Parser

  def parse()
    argnames = []
    
    if keyword(:method)
      case peek_token()
      when IdentifierToken
        name = next_token().float
      when KeywordToken
        name = next_token().float
        
        begin repeat(IdentifierToken) do |argname|
          argnames << argname.float
        end end while option(CommaToken)
      end

      expressions = BodyParser.new(@lexer).frame.parse()

      return [LocalMessage(:"self"), SendMessage(:"add-method:", [Value(name), BlockMessage(argnames, expressions)])]
    end
  end

end

