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

		virtual bool parse( vector<GCValue>& messages );

		virtual void initialize( vector<GCValue>& messages, Token peek ) { }
		virtual void finalize( vector<GCValue>& messages, Token peek ) { }
		
		virtual bool lit_number_( vector<GCValue>& messages, Token peek ) { return false; }
		virtual bool lit_string_( vector<GCValue>& messages, Token peek ) { return false; }
		virtual bool identifier_( vector<GCValue>& messages, Token peek ) { return false; }
		virtual bool operator_( vector<GCValue>& messages, Token peek ) { return false; }
		virtual bool openparen_( vector<GCValue>& messages, Token peek ) { return false; }
		virtual bool closeparen_( vector<GCValue>& messages, Token peek ) { return false; }
		virtual bool verticalbar_( vector<GCValue>& messages, Token peek ) { return false; }
		virtual bool endline_( vector<GCValue>& messages, Token peek ) { return false; }

	 private:
	
		Scanner& _scanner;
	
	};


	class StatementParser : public Parser {
	
	 public:

		StatementParser( Scanner& scanner ) : Parser( scanner ) { }

		virtual bool parse( vector<GCValue>& messages );
		
	};

	class ExpressionParser : public Parser {
	
	 public:

		ExpressionParser( Scanner& scanner ) : Parser( scanner ) { }

		virtual bool lit_number_( vector<GCValue>& messages, Token peek );
		
	};

	class MessageParser : public Parser {
	
	 public:
	
		MessageParser( Scanner& scanner ) : Parser( scanner ) { }

		virtual bool operator_( vector<GCValue>& messages, Token peek );
		
	};

	class BinaryExpressionParser : public ExpressionParser {
	
	 public:

		BinaryExpressionParser( Scanner& scanner ) : ExpressionParser( scanner ) { }

		virtual bool lit_number_( vector<GCValue>& messages, Token peek );
		
	};

	class BinaryMessageParser : public ExpressionParser {
	
	 public:

		BinaryMessageParser( Scanner& scanner ) : ExpressionParser( scanner ) { }

		virtual bool operator_( vector<GCValue>& messages, Token peek );
		
	};

	//
	//
	//

	bool Parser::parse( vector<GCValue>& messages )
	{
		Token peek = _scanner.peekToken();

		//initialize( messages, scanner().currToken() );
		return (this->*peek.type())( messages, peek );
		//finalize( messages, scanner().currToken() );
	}

	bool StatementParser::parse( vector<GCValue>& messages )
	{	
		ExpressionParser( scanner() ).parse( messages );
		
		Token peek = scanner().peekToken();
		
		if (peek.type() != &Parser::endline_)
		{
			cerr << "*** Unexpected token: '" << peek.value() << "'" << endl;
		}
		
		scanner().nextToken();
		
		return true;
	}

	bool ExpressionParser::lit_number_( vector<GCValue>& messages, Token peek )
	{
		Token token = scanner().nextToken();

		messages.push_back( token.value() );

		while (MessageParser( scanner() ).parse( messages ));

		return true;
	}

	bool MessageParser::operator_( vector<GCValue>& messages, Token peek )
	{
		Token token = scanner().nextToken();

		vector<GCValue> argument;
		BinaryExpressionParser( scanner() ).parse( argument );

		Symbol symbol = SymbolProto::at( token.value().get<StringProto>().getString() );

		if (&symbol == &SymbolProto::at( "+" ))
			messages.push_back( *new OperatorMessage<add_>( symbol, *new ArrayProto( argument[0] ) ) );
		else if (&symbol == &SymbolProto::at( "-" ))
			messages.push_back( *new OperatorMessage<sub_>( symbol, *new ArrayProto( argument[0] ) ) );
		else
			messages.push_back( *new MessageProto( symbol, *new ArrayProto( argument[0] ) ) );

		MessageParser( scanner() ).parse( messages );

		return true;
	}

	bool BinaryExpressionParser::lit_number_( vector<GCValue>& messages, Token peek )
	{
		Token token = scanner().nextToken();

		messages.push_back( token.value() );

		while (BinaryMessageParser( scanner() ).parse( messages ));

		return true;
	}

	bool BinaryMessageParser::operator_( vector<GCValue>& messages, Token peek )
	{
		return false;
	}
		
}

#endif

