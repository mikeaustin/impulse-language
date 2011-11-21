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


def assert(expression, operator, expected)
  if expression.eval_($lobby, [], $lobby).value.send(operator, expected)
    print "\x1b[32m\x1b[1mpass\x1b[0m"
  else
    print "\x1b[31m\x1b[1mfail\x1b[0m"
  end
  
  puts "\t#{expression} == #{expected}"
end

def output(string)
  puts "\t" + string
end


# x = 5
messages = [AssignMessage(:x, Value(5))]
assert(ExpressionProto(messages), :==, 5)

# y = 1
messages = [AssignMessage(:y, Value(1))]
assert(ExpressionProto(messages), :==, 1)

# x pow: 2
messages = [LocalMessage(:x), SendMessage(:pow, [Value(2)])]
assert(ExpressionProto(messages), :==, 25)

# (x add: 5) pow: (y add: 1)
messages = [ExpressionProto([LocalMessage(:x), SendMessage(:add, [Value(5)])]),
            SendMessage(:pow, [ExpressionProto([LocalMessage(:y), SendMessage(:add, [Value(1)])])])]
assert(ExpressionProto(messages), :==, 100)

# |y| (x add: 5) pow: (y add: 1)
messages = [BlockMessage([:y], [ExpressionProto(messages)])]

# block eval: 2
messages = [ExpressionProto(messages), SendMessage(:call, [Value(2)])]
assert(ExpressionProto(messages), :==, 1000)

# self
$lobby.set_slot(:self, $lobby)
messages = [LocalMessage(:self)]
assert(ExpressionProto(messages), :==, $lobby)

# object :button, |button|
#    button foo = 100
# end
messages = [ObjectMessage(:button, BlockMessage([:button], [
               ExpressionProto([LocalMessage(:button), AssignMessage(:foo, Value(100))])
           ]))]
p ExpressionProto(messages).eval_($lobby, [], $lobby)

# method :foo, |x|
#    print: x
# end
messages = [MethodMessage(:foo, BlockMessage([:x], [
               ExpressionProto([SendMessage(:print, [LocalMessage(:x)])])
           ]))]
ExpressionProto(messages).eval_($lobby, [], $lobby)

# print: 20
messages = [SendMessage(:foo, [Value(20)])]
ExpressionProto(messages).eval_($lobby, [], $lobby)


lexer = Lexer.new(STDIN)
parser = Parser.new(lexer)

while true
  #token = lexer.frame.next_token()
  #p token
  p parser.frame.parse()
end

