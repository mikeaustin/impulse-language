#
# impulse.rb
#
# Copyright 2011 Mike Austin
# All rights reserved.
#

require './core/value.rb'
require './core/frame.rb'

require './core/protos/nil.rb'
require './core/protos/object.rb'
require './core/protos/boolean.rb'
require './core/protos/symbol.rb'
require './core/protos/lobby.rb'
require './core/protos/block.rb'
require './core/protos/number.rb'
require './core/protos/array.rb'
require './core/protos/point.rb'

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

$lobby.set_local(:"nil",   NilProto.instance)
$lobby.set_local(:"true",  Value(true))
$lobby.set_local(:"false", Value(false))
$lobby.set_local(:"<object>", ObjectProto.instance)
$lobby.set_local(:"<number>", NumberProto.instance)
$lobby.set_local(:"<string>", StringProto.instance)
$lobby.set_local(:"<symbol>", SymbolProto.instance)
$lobby.set_local(:"<array>",  ArrayProto.instance)
$lobby.set_local(:"<block>",  BlockProto.instance)
$lobby.set_local(:"<point>",  PointProto.instance)

$trace = false
$file = nil

ARGV.each do |arg|
  case arg
  when "-t"
    $trace = true
  else
    $file = arg
  end
end
  

def trace(string)
  if $trace
    puts string
  end
end

class Array

  def to_s()
    return self.map do |item| item.to_s end.join ", "
  end

end

require './tests.rb'
require './core/protos/point.rb'


lexer  = Lexer.new($file ? File.open($file) : STDIN)
parser = StatementParser.new(lexer)

print "] " if !$file
while messages = parser.frame.parse()
  result = ExpressionProto(messages).eval_($lobby, [], $lobby)

  if result && !$file
    print "= ", result.inspect(), "\n"
  end

  print "] " if !$file
end

