#
# impulse.rb
#

require './core/value.rb'
require './core/frame.rb'

require './core/protos/object.rb'
require './core/protos/symbol.rb'
require './core/protos/lobby.rb'
require './core/protos/block.rb'
require './core/protos/number.rb'

require './runtime/protos/message.rb'
require './runtime/protos/expression.rb'

require './parser/protos/parser.rb'


def LocalsProto(frame)
  return LocalsProto.new(frame)
end

class LocalsProto < Frame
end


ObjectProto.instance.frame.init_slots()

$lobby = LobbyProto()

def trace(string)
  if ARGV[0] == "-t"
    puts string
  end
end


class Array

  attr :args, true

  def to_s()
    return self.map do |item| item.to_s end.join ", "
  end

end


messages = [Value(5), SendMessage(:sin, [])]
p ExpressionProto(messages).eval_($lobby, [], $lobby)


lexer = Lexer.new(STDIN)
#lexer = Lexer.new(File.open("test.im"))
parser = StatementParser.new(lexer)

while true
  #token = lexer.frame.next_token()
  #p token
  print "] "
  messages = parser.frame.parse()
  print "= " + ExpressionProto(messages).eval_($lobby, [], $lobby).to_s + "\n"
end

