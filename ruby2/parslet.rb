
require 'parslet'

class Mini < Parslet::Parser
  #rule(:lparen)     { str('(') >> space? }
  #rule(:rparen)     { str(')') >> space? }
  #rule(:comma)      { str(',') >> space? }

  rule(:space)      { match('\s').repeat(1) }
  rule(:space?)     { space.maybe }

  rule(:integer)    { match('[0-9]').repeat(1).as(:int) >> space? }
  #rule(:identifier) { match('[a-z]').repeat(1) }
  rule(:operator)   { match('[+]').as(:op) >> space? }

  rule(:sum)        { integer.as(:left) >> operator >> expression.as(:right) }
  #rule(:arglist)    { expression >> (comma >> expression).repeat }
  #rule(:funcall)    { identifier.as(:funcall) >> lparen >> arglist.as(:arglist) >> rparen }
  rule(:expression) { sum | integer }

  root(:expression)

  rule(:factor)     { integer | expression }
  rule(:term)       { factor.repeat }
  rule(:expression) { term.repeat }
end

def parse(str)
  mini = Mini.new
  print "Parsing #{str}: "

  p mini.parse(str)
rescue Parslet::ParseFailed => error
  puts error, mini.root.error_tree
end

ast = parse "1 + 2 + 3"
