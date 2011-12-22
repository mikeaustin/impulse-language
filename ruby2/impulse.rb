#
# impulse.rb
#

require './core/value.rb'
require './core/frame.rb'

require './core/protos/nil.rb'
require './core/protos/object.rb'
require './core/protos/symbol.rb'
require './core/protos/lobby.rb'
require './core/protos/block.rb'
require './core/protos/number.rb'
require './core/protos/array.rb'

require './runtime/protos/message.rb'
require './runtime/protos/expression.rb'

require './parser/protos/parser.rb'


def LocalsProto(frame)
  return LocalsProto.new(frame)
end

class LocalsProto < Frame
end

puts "Ruby Impulse 0.1 (Genesis) | Copyright 2011 Mike Austin"

ObjectProto.instance.frame.init_slots()

$lobby = LobbyProto()

$lobby.set_local(:nil, NilProto.instance)
$lobby.set_local(:block, BlockProto.instance)


def trace(string)
  if ARGV[0] == "-t"
    puts string
  end
end


class Array

  def to_s()
    return self.map do |item| item.to_s end.join ", "
  end

end

#require './tests.rb'
require './core/protos/point.rb'

lexer = Lexer.new(STDIN)
#lexer = Lexer.new(File.open("test.im"))
parser = StatementParser.new(lexer)

while true
  print "] "
  messages = parser.frame.parse()
  result = ExpressionProto(messages).eval_($lobby, [], $lobby)

  if result
    print "= ", result.inspect(), "\n"
  end
end

