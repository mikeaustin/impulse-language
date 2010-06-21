#!/usr/bin/env ruby
#
# impulse.rb
#
# Copyright 2010 Mike Austin
# Provided under MIT licence
#

$VERBOSE = nil

require 'runtime'
require 'parser'

class Symbol
  def to_proc
    Proc.new { |*args| self.send(self, *args) }
  end
end

include Impulse

# ---------------------------------------------------------------------------- #

puts "Ruby-Impulse 0.1.  Copyright 2010 Mike Austin"

lobby = LobbyProto.instance()

=begin
code1 = [MessageProto.new(:inspect, [])]
code2 = [MessageProto.new(:x, []), MessageProto.new(:pow_, [MessageProto.new(:x, [])])]
#code3 = [RangeProto.new(1, 2), MessageProto.new(:each, [Block.new(Expression.new(code1))])]


puts Expression.new(code1).eval.call(lobby, [], lobby)
puts Expression.new(code2).eval.call(lobby, [], lobby)

#Expression.new(code3).eval.call(lobby, [], lobby)
=end

stream = ARGV.size > 0 ? File.open(ARGV[0]) : STDIN
parser = Parser.new(Lexer.new(stream))

print "] " if parser.tty?
while expr = parser.expression()
#  begin
    result = expr.eval.call(lobby, [], lobby)
    if !result.is_a? VoidProto
      print "> " + result.to_s + "\n"
    end
#  rescue StandardError => error
#    puts error
#  end
  print "] " if parser.tty?
end
