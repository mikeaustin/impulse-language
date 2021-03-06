#
# tests.rb
#


def assert(expression, operator, expected)
  if expression.eval_($lobby, $lobby).raw_value.send(operator, expected)
    print "\x1b[32m\x1b[1mpass\x1b[0m"
  else
    print "\x1b[31m\x1b[1mfail\x1b[0m"
  end
  
  puts "\t#{expression} == #{expected}"
end

def output(expression)
  puts "\t" + expression.to_s
end


# x = 5
messages = [AssignMessage(:x, Value(5))]
assert ExpressionProto(messages), :==, 5

# y = 1
messages = [AssignMessage(:y, Value(1))]
assert ExpressionProto(messages), :==, 1

# x pow: 2
messages = [LocalMessage(:x), SendMessage(:"pow:", [Value(2)])]
assert ExpressionProto(messages), :==, 25

# (x add: 5) pow: (y add: 1)
messages = [ExpressionProto([LocalMessage(:x), SendMessage(:"+", [Value(5)])]),
            SendMessage(:"pow:", [ExpressionProto([LocalMessage(:y), SendMessage(:"+", [Value(1)])])])]
assert ExpressionProto(messages), :==, 100

# |y| (x add: 5) pow: (y add: 1)
messages = [BlockMessage([:y], [ExpressionProto(messages)])]

# block call: 2
messages = [ExpressionProto(messages), SendMessage(:"call:", [Value(2)])]
assert ExpressionProto(messages), :==, 1000

# self
$lobby.set_local(:"self", $lobby)
messages = [LocalMessage(:"self")]
assert ExpressionProto(messages), :==, $lobby

# object :button, |button|
#    button foo = 100
# end
messages = [ObjectMessage(:button, BlockMessage([:button], [
               ExpressionProto([LocalMessage(:button), AssignMessage(:foo, Value(100))])
           ]))]
output ExpressionProto(messages)
ExpressionProto(messages).eval_($lobby, $lobby)

# method :foo, |x|
#    $ print: x
# end
messages = [MethodMessage(:"foo:", BlockMessage([:x], [
               ExpressionProto([LocalMessage(:x)])
           ]))]
output ExpressionProto(messages)
ExpressionProto(messages).eval_($lobby, $lobby)

# foo: 20
messages = [SendMessage(:"foo:", [Value(20)])]
assert ExpressionProto(messages), :==, 20

