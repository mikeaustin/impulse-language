#
# impulse.rb
#
# Copyright 2011 Mike Austin
# All rights reserved.
#

require './core/protos/lobby.rb'

require './runtime/protos/message.rb'
require './runtime/protos/expression.rb'

require './parser/protos/parser.rb'


puts "Ruby Impulse 0.1 (Genesis) | Copyright 2011 Mike Austin"

ObjectProto.instance.frame.init_slots()

$lobby = LobbyProto()

$trace = false
$file = nil
  

def trace(string)
  if $trace
    puts string
  end
end

class Array

  attr :_self, true

  def to_s()
    return self.map do |item| item.to_s end.join ", "
  end

end

require './tests.rb'


ARGV.each do |arg|
  case arg
  when "-t"
    $trace = true
  else
    $file = arg
  end
end


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

