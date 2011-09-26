
require './core/value.rb'
require './core/frame.rb'

require './core/protos/object.rb'
require './core/protos/block.rb'
require './core/protos/number.rb'

require './runtime/protos/message.rb'
require './runtime/protos/expression.rb'


class LobbyProto < Frame

  def initialize()
    super(ObjectProto.instance)
  end
  
end


class LocalsProto < Frame

end


ObjectProto.instance.frame.init_slots()

$lobby = LobbyProto.new()

def trace(string)
  if ARGV[1] == "-t"
    puts string
  end
end

class Array

  def to_s
    return self.map do |item|
      (item.float.is_a?(Symbol) ? "'" : "") + item.to_s
    end.join ", "
  end

end


def assert(expression, operator, expected)
  #if expression.eval_($lobby, [], $lobby).float == expected
  if expression.eval_($lobby, [], $lobby).float.send(operator, expected)
    print "\x1b[32m\x1b[1mpass\x1b[0m"
  else
    print "\x1b[31m\x1b[1mfail\x1b[0m"
  end
  
  puts "\t#{expression} == #{expected}"
end

def output(string)
  puts "\t" + string
end


#$lobby.set_slot(:x, Value.new(5))
#$lobby.set_slot(:y, Value.new(1))

#      (x + 5) pow: (y)
code = [ExpressionProto.new([MessageProto.new(:x, []), MessageProto.new(:add, [Value.new(5)])]),
        MessageProto.new(:pow, [ExpressionProto.new([MessageProto.new(:y, [])])])]
#code = [Value.new(10), MessageProto.new(:sin, [])]
#code = [Value.new(5), MessageProto.new(:is_a, [NumberProto.instance])]
code = [BlockMessage.new(code)]

#expr = ExpressionProto.new(code)
#p expr.eval_(lobby, [], lobby)

block = BlockProto.new([ExpressionProto.new(code)], $lobby)
#p block.eval_(lobby, [], lobby).eval_(lobby, [], lobby)


#code = ExpressionProto.new([Value.new(5), MessageProto.new(:pow, [Value.new(2)])])
#puts code.eval_($lobby, [], $lobby)
#p code
#puts code

# x = 5
messages = [MessageProto.new(:assign, [Value.new(:x), Value.new(5)])]
assert(ExpressionProto.new(messages), :==, 5)

# y = 1
messages = [MessageProto.new(:assign, [Value.new(:y), Value.new(1)])]
assert(ExpressionProto.new(messages), :==, 1)

# 5 pow: 2
messages = [Value.new(5), MessageProto.new(:pow, [Value.new(2)])]
assert(ExpressionProto.new(messages), :==, 25)

# (x add: 5) pow: (y add: 1)
messages = [ExpressionProto.new([MessageProto.new(:x, []), MessageProto.new(:add, [Value.new(5)])]),
            MessageProto.new(:pow, [ExpressionProto.new([MessageProto.new(:y, []), MessageProto.new(:add, [Value.new(1)])])])]
assert(ExpressionProto.new(messages), :==, 100)

# || ((x add: 5) pow: (y add: 1)) eval
messages = [BlockMessage.new([ExpressionProto.new(messages)]), MessageProto.new(:eval, [])]
assert(ExpressionProto.new(messages), :==, 100)

