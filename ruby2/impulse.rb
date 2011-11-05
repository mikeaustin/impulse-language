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


class LocalsProto < Frame

end


ObjectProto.instance.frame.init_slots()

$lobby = LobbyProto.new()

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
messages = [AssignMessage.new(Value.new(:x), Value.new(5))]
assert(ExpressionProto.new(messages), :==, 5)

# y = 1
messages = [AssignMessage.new(Value.new(:y), Value.new(1))]
assert(ExpressionProto.new(messages), :==, 1)

# x pow: 2
messages = [LocalMessage.new(:x), SendMessage.new(:pow, [Value.new(2)])]
assert(ExpressionProto.new(messages), :==, 25)

# (x add: 5) pow: (y add: 1)
messages = [ExpressionProto.new([LocalMessage.new(:x), SendMessage.new(:add, [Value.new(5)])]),
            SendMessage.new(:pow, [ExpressionProto.new([LocalMessage.new(:y), SendMessage.new(:add, [Value.new(1)])])])]
assert(ExpressionProto.new(messages), :==, 100)

# |y| (x add: 5) pow: (y add: 1)
messages = [BlockMessage.new([:y], [ExpressionProto.new(messages)])]

# block eval: 2
messages = [ExpressionProto.new(messages), SendMessage.new(:eval, [Value.new(2)])]
assert(ExpressionProto.new(messages), :==, 1000)

messages = [LocalMessage.new(:self)]
$lobby.set_slot(:self, $lobby)
assert(ExpressionProto.new(messages), :==, $lobby)

