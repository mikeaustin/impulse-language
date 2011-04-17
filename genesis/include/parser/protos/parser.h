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

		int precedence( Token token )
		{
			if (token.value().get<StringProto>().getString() == "*")
				return 10;
			else if (token.value().get<StringProto>().getString() == "+")
				return 20;
			
			return 0;
		}

		virtual Expression& parse( int prec );

		//virtual void initialize( vector<GCValue>& messages, Token peek ) { }
		//virtual void finalize( vector<GCValue>& messages, Token peek ) { }
		
		virtual Value lit_number_( Token peek, int prec ) { return Value(); }
		virtual Value lit_string_( Token peek, int prec ) { return Value(); }
		virtual Value identifier_( Token peek, int prec ) { return Value(); }
		virtual Value operator_( Token peek, int prec ) { return Value(); }
		virtual Value openparen_( Token peek, int prec ) { return Value(); }
		virtual Value closeparen_( Token peek, int prec ) { return Value(); }
		virtual Value verticalbar_( Token peek, int prec ) { return Value(); }
		virtual Value endline_( Token peek, int prec ) { return Value(); }

	 private:
	
		Scanner& _scanner;
	
	};

 //
 //
 //

	class StatementParser : public Parser {
	
	 public:

		StatementParser( Scanner& scanner ) : Parser( scanner ) { }

		virtual Expression& parse( int prec );

	};

	class ExpressionParser : public Parser {
	
	 public:

		ExpressionParser( Scanner& scanner ) : Parser( scanner ) { }

		virtual Expression& parse( int prec );

		virtual Value lit_number_( Token peek, int prec );
		virtual Value openparen_( Token peek, int prec );
		
	};

	class MessageParser : public Parser {
	
	 public:
	
		MessageParser( Scanner& scanner ) : Parser( scanner ) { }

		virtual Value operator_( Token peek, int prec );
		
	};

	class BinaryExpressionParser : public ExpressionParser {
	
	 public:

		virtual Expression& parse( int prec );

		BinaryExpressionParser( Scanner& scanner ) : ExpressionParser( scanner ) { }
		
	};

	class BinaryMessageParser : public MessageParser {
	
	 public:

		BinaryMessageParser( Scanner& scanner ) : MessageParser( scanner ) { }

		virtual Value operator_( Token peek, int prec );
		
	};

 //
 //
 //

	Expression& Parser::parse( int prec )
	{
		Expression& expression = *new Expression();

		Token peek = scanner().peekToken();

		Value operand = (this->*peek.type())( peek, prec );

		if (&operand.getFrame() != NULL)
		{
			expression.push_back( operand );
		}
		
		return expression;
	}

	Expression& StatementParser::parse( int prec )
	{	
		Expression& expression = ExpressionParser( scanner() ).parse( prec );
		
		Token peek = scanner().peekToken();
		
		if (peek.type() != &Parser::endline_)
		{
			cerr << "*** Unexpected token: '" << peek.value() << "'" << endl;
		}
		
		scanner().nextToken();
		
		return expression;
	}
	
	Expression& ExpressionParser::parse( int prec )
	{
		Expression& expression = Parser::parse( prec );
/*
		if (expression.size() == 0)
		{
			cerr << "*** Expected operand" << endl;
			
			return expression;
		}
*/
		Expression* expression2;
		while (expression2 = &MessageParser( scanner() ).parse( prec ),
		       expression2->size() > 0)
		{
			expression.concat( *expression2 );
		}

		return expression;
	}

	Expression& BinaryExpressionParser::parse( int prec )
	{
		Expression& expression = Parser::parse( prec );

		if (expression.size() == 0)
		{
			cerr << "*** Expected operand" << endl;
			
			return expression;
		}
		
		Expression* expression2;
		while (expression2 = &BinaryMessageParser( scanner() ).parse( prec ),
		       expression2->size() > 0)
		{
			expression.concat( *expression2 );
		}

		return expression;
	}

	Value ExpressionParser::lit_number_( Token peek, int prec )
	{
		Token token = scanner().nextToken();

		return token.value();
	}

	Value ExpressionParser::openparen_( Token peek, int prec )
	{
		expect( &Parser::openparen_, "(" );
		
		Expression& expression = ExpressionParser( scanner() ).parse( prec );

		expect( &Parser::closeparen_, ")" );
		
		return expression;
	}

	Value MessageParser::operator_( Token peek, int prec )
	{
		Token token = scanner().nextToken();

		prec = precedence( peek );
		Expression& argument = BinaryExpressionParser( scanner() ).parse( prec );

		Symbol symbol = SymbolProto::at( token.value().get<StringProto>().getString() );

		if (&symbol == &SymbolProto::at( "+" ))
			return *new OperatorMessage<add_>( symbol, *new ArrayProto( argument ) );
		else if (&symbol == &SymbolProto::at( "-" ))
			return *new OperatorMessage<sub_>( symbol, *new ArrayProto( argument ) );
		if (&symbol == &SymbolProto::at( "*" ))
			return *new OperatorMessage<mul_>( symbol, *new ArrayProto( argument ) );
		else if (&symbol == &SymbolProto::at( "/" ))
			return *new OperatorMessage<div_>( symbol, *new ArrayProto( argument ) );
		else
			return *new MessageProto( symbol, *new ArrayProto( argument ) );

		return Value();
	}

	Value BinaryMessageParser::operator_( Token peek, int prec )
	{
		if (precedence( peek ) > prec)
			return MessageParser::operator_( peek, prec );
		else
			return Value();
	}
		
}

#endif

