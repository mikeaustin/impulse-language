
require 'parslet'

class Mini < Parslet::Parser
  #rule(:lparen)     { str('(') >> space? }
  #rule(:rparen)     { str(')') >> space? }
  rule(:comma)      { str(',') >> space? }
  rule(:arglist)	{ identifier >> (comma >> identifier).repeat }

  rule(:space)      { match('\s').repeat(1) }
  rule(:space?)     { space.maybe }
  rule(:newline)	{ str("\n") >> str("\r").maybe }

  rule(:integer)    { match('[0-9]').repeat(1) }
  rule(:identifier) { match('[a-z]').repeat(1) }
  rule(:operator)   { match('[+]').as(:op) }

  #rule(:sum)        { integer.as(:left) >> operator >> expression.as(:right) }
  #rule(:arglist)    { expression >> (comma >> expression).repeat }
  #rule(:funcall)    { identifier.as(:funcall) >> lparen >> arglist.as(:arglist) >> rparen }
  rule(:keyword)			{ identifier.as(:keyword) >> str(':') >> space? }
  rule(:keyword_message)	{ keyword >> space? >> expression.as(:argument) }
  rule(:unary_message)		{ identifier.as(:selector) >> space? }
  rule(:message)			{ keyword_message.as(:keyword_message) | unary_message.as(:unary_message) }
  rule(:block)				{ str("|") >> arglist.as(:arglist).maybe >> str("|") >> space? >>
								((str("do") >> newline >> statements) |
								expression.as(:expression)) }

  rule(:receiver)			{ integer.as(:number) | identifier.as(:identifier) | block.as(:block) }
  rule(:expression)			{ receiver >> (space? >> message.repeat(0)) }
  rule(:statement)			{ space? >> expression.as(:expression) >> newline.maybe }
  rule(:comment)			{ space? >> str("//") >> any }
  rule(:statements)			{ statement.repeat }

  root(:statements)
end

def parse(str)
  mini = Mini.new
  print "Parsing #{str}: "

  p mini.parse(str)
rescue Parslet::ParseFailed => error
  puts error, mini.root.error_tree
end

parse """
  list sort: |a, b| a sin
"""

