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

		virtual const vector<GCValue> parse();

		virtual void initialize( vector<GCValue>& messages, Token peek ) { }
		virtual void finalize( vector<GCValue>& messages, Token peek ) { }
		
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

		virtual const vector<GCValue> parse();
		
	};

	class ExpressionParser : public Parser {
	
	 public:

		ExpressionParser( Scanner& scanner ) : Parser( scanner ) { }

		virtual const vector<GCValue> parse();

		virtual Value lit_number_( Token peek );
		
	};

	class MessageParser : public Parser {
	
	 public:
	
		MessageParser( Scanner& scanner ) : Parser( scanner ) { }

		virtual Value operator_( Token peek );
		
	};

	class BinaryExpressionParser : public ExpressionParser {
	
	 public:

		virtual const vector<GCValue> parse();

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

	const vector<GCValue> Parser::parse()
	{
		vector<GCValue> messages;

		Token peek = scanner().peekToken();

		Value operand = (this->*peek.type())( peek );

		if (&operand.getFrame() != NULL)
		{
			messages.push_back( operand );
		}
		
		return messages;
	}

	const vector<GCValue> StatementParser::parse()
	{	
		vector<GCValue> messages = ExpressionParser( scanner() ).parse();
		
		Token peek = scanner().peekToken();
		
		if (peek.type() != &Parser::endline_)
		{
			cerr << "*** Unexpected token: '" << peek.value() << "'" << endl;
		}
		
		scanner().nextToken();
		
		return messages;
	}
	
	const vector<GCValue> ExpressionParser::parse()
	{
		vector<GCValue> messages = Parser::parse();

		if (&messages[0].getFrame() == NULL)
		{
			cerr << "*** Expected operand" << endl;
			
			return messages;
		}
		
		vector<GCValue> messages2;
		while (messages2 = MessageParser( scanner() ).parse(), messages2.size() > 0)
		{
			messages.insert( messages.end(), messages2.begin(), messages2.end() );
		}

		return messages;
	}

	const vector<GCValue> BinaryExpressionParser::parse()
	{
		vector<GCValue> messages = Parser::parse();

		if (&messages[0].getFrame() == NULL)
		{
			cerr << "*** Expected operand" << endl;
			
			return messages;
		}
		
		vector<GCValue> messages2;
		while (messages2 = BinaryMessageParser( scanner() ).parse(), messages2.size() > 0)
		{
			messages.insert( messages.end(), messages2.begin(), messages2.end() );
		}

		return messages;
	}

	Value ExpressionParser::lit_number_( Token peek )
	{
		Token token = scanner().nextToken();

		return token.value();
	}

	Value MessageParser::operator_( Token peek )
	{
		Token token = scanner().nextToken();

		vector<GCValue> argument = BinaryExpressionParser( scanner() ).parse();

		Symbol symbol = SymbolProto::at( token.value().get<StringProto>().getString() );

		if (&symbol == &SymbolProto::at( "+" ))
			return *new OperatorMessage<add_>( symbol, *new ArrayProto( argument[0] ) );
		else if (&symbol == &SymbolProto::at( "-" ))
			return *new OperatorMessage<sub_>( symbol, *new ArrayProto( argument[0] ) );
		else
			return *new MessageProto( symbol, *new ArrayProto( argument[0] ) );

		return Value();
	}

	Value BinaryMessageParser::operator_( Token peek )
	{
		return Value();
	}
		
}

#endif

