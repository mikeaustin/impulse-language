//
// core/protos/block.h
//
// Copyright 2008-2011 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_PARSER_H
#define IMPULSE_PARSER_H

#include "core/frame.h"
#include "core/array.h"

#include "scanner.h"

using namespace std;

namespace impulse {

 //
 // class Parser
 //

	class Parser : public Frame {
	
	 public:
	
		Parser( Scanner& scanner ) : _scanner( scanner ) { }
		
		Scanner& scanner() { return _scanner; }

		Token expect( Token::Type type, const string value )
		{
			if (scanner().peekToken().type() != type)
			{
				cerr << "Expected " << value << ", found " << scanner().peekToken().value().inspect() << endl;
			}
			
			return scanner().nextToken();
		}

		Token option( Token::Type type, const string value )
		{
			if (scanner().peekToken().type() != type)
			{
				return Token();
			}

			return scanner().nextToken();
		}

		virtual Expression& parse();

		//virtual void initialize( vector<GCValue>& messages, Token peek ) { }
		//virtual void finalize( vector<GCValue>& messages, Token peek ) { }
		
		virtual Value lit_number_( Token peek ) { return Value(); }
		virtual Value lit_string_( Token peek ) { return Value(); }
		virtual Value identifier_( Token peek ) { return Value(); }
		virtual Value operator_( Token peek ) { return Value(); }
		virtual Value openparen_( Token peek ) { return Value(); }
		virtual Value closeparen_( Token peek ) { return Value(); }
		virtual Value verticalbar_( Token peek ) { return Value(); }
		virtual Value endline_( Token peek ) { return Value(); }

	 private:
	
		Scanner& _scanner;
	
	};


	class StatementParser : public Parser {
	
	 public:

		StatementParser( Scanner& scanner ) : Parser( scanner ) { }

		virtual Expression& parse();
		
	};

	class ExpressionParser : public Parser {
	
	 public:

		ExpressionParser( Scanner& scanner ) : Parser( scanner ) { }

		virtual Expression& parse();

		virtual Value lit_number_( Token peek );
		virtual Value openparen_( Token peek );
		
	};

	class MessageParser : public Parser {
	
	 public:
	
		MessageParser( Scanner& scanner ) : Parser( scanner ) { }

		virtual Value operator_( Token peek );
		
	};

	class BinaryExpressionParser : public ExpressionParser {
	
	 public:

		virtual Expression& parse();

		BinaryExpressionParser( Scanner& scanner ) : ExpressionParser( scanner ) { }
		
	};

	class BinaryMessageParser : public MessageParser {
	
	 public:

		BinaryMessageParser( Scanner& scanner ) : MessageParser( scanner ) { }

		virtual Value operator_( Token peek );
		
	};

 //
 //
 //

	Expression& Parser::parse()
	{
		Expression& expression = *new Expression();

		Token peek = scanner().peekToken();

		Value operand = (this->*peek.type())( peek );

		if (&operand.getFrame() != NULL)
		{
			expression.push_back( operand );
		}
		
		return expression;
	}

	Expression& StatementParser::parse()
	{	
		Expression& expression = ExpressionParser( scanner() ).parse();
		
		Token peek = scanner().peekToken();
		
		if (peek.type() != &Parser::endline_)
		{
			cerr << "*** Unexpected token: '" << peek.value() << "'" << endl;
		}
		
		scanner().nextToken();
		
		return expression;
	}
	
	Expression& ExpressionParser::parse()
	{
		Expression& expression = Parser::parse();

		if (expression.size() == 0)
		{
			cerr << "*** Expected operand" << endl;
			
			return expression;
		}
		
		Expression* expression2;
		while (expression2 = &MessageParser( scanner() ).parse(), expression2->size() > 0)
		{
			//messages.insert( messages.end(), messages2.begin(), messages2.end() );
			expression.concat( *expression2 );
		}

		return expression;
	}

	Expression& BinaryExpressionParser::parse()
	{
		Expression& expression = Parser::parse();

		if (expression.size() == 0)
		{
			cerr << "*** Expected operand" << endl;
			
			return expression;
		}
		
		Expression* expression2;
		while (expression2 = &BinaryMessageParser( scanner() ).parse(), expression2->size() > 0)
		{
			//messages.insert( messages.end(), messages2.begin(), messages2.end() );
			expression.concat( *expression2 );
		}

		return expression;
	}

	Value ExpressionParser::lit_number_( Token peek )
	{
		Token token = scanner().nextToken();

		return token.value();
	}

	Value ExpressionParser::openparen_( Token peek )
	{
		expect( &Parser::openparen_, "(" );
		
		Expression& expression = ExpressionParser( scanner() ).parse();

		expect( &Parser::closeparen_, ")" );
		
		return expression;
	}

	Value MessageParser::operator_( Token peek )
	{
		Token token = scanner().nextToken();

		Expression& argument = BinaryExpressionParser( scanner() ).parse();

		Symbol symbol = SymbolProto::at( token.value().get<StringProto>().getString() );

		if (&symbol == &SymbolProto::at( "+" ))
			return *new OperatorMessage<add_>( symbol, *new ArrayProto( argument ) );
		else if (&symbol == &SymbolProto::at( "-" ))
			return *new OperatorMessage<sub_>( symbol, *new ArrayProto( argument ) );
		else
			return *new MessageProto( symbol, *new ArrayProto( argument ) );

		return Value();
	}

	Value BinaryMessageParser::operator_( Token peek )
	{
		return Value();
	}
		
}

#endif

