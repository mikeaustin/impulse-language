#
# parser/parselet.rb
#


class Parselet < Frame

end


class ArrayParser < Parslet

  def self.parse(parser)
    parser.expect(OpenBracketToken, "Expcected '['")

    items = []

    if !parser.peek_token().is_a? CloseBracketToken
      begin
        items << ExpressionProto(ExpressionParser(@lexer))
      end while option(CommaToken)
    end
    
    parser.expect(CloseBracketToken, "Expected ']'")
    
    return [ArrayMessage(items)]
  end

end

