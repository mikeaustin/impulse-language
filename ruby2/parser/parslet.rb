#
# parser/parselet.rb
#


class Parselet < Frame

  def initialize(parser)
    @parser = parser
  end

end


class StatementParslet < Parser

  def parse()
    messages = ExpressionParslet(@lexer)

    expect(NewlineToken, "Expected a message [2].")
    
    return messages
  end
  
end

