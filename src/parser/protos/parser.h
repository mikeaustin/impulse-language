//
// parser/protos/parser.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_PARSER_H
#define IMPULSE_PARSER_H

#include "lexer.h"

#include <cstdlib>
#include <vector>

using namespace std;

namespace impulse {

	void concat( vector<Value>& vector1, const vector<Value>& vector2 );

	class Parser : public Frame	{

	 public:

		typedef const vector<Value> (Parser::*ParserFunc)();
		
		Parser( Lexer& lexer ) : _lexer( lexer ) { localArgsStack.push_back( Array() ); }

	 //
	 // Convenience methods
	 //
	 
		Token expect( const Token::Type type, const string value ) const;
		Token option( const Token::Type type, const string value ) const;
		int precedence( const string operator_ ) const;

	 //
	 // Parsing methods
	 //

		const Expression parseLine();
		const Expression parseStatement();
		const vector<Value> parseExpression( ParserFunc parseMessage = &Parser::parseMessage );
		const vector<Value> parseSubexpr();

		const vector<Value> parseMessage();
		const vector<Value> parseBinaryMessage();

		const vector<Value> parseNumber();
		const vector<Value> parseString();
		const vector<Value> parseSimpleIdentifier();
		const vector<Value> parseIdentifier();
		const vector<Value> parseKeyword();
		const vector<Value> parseOperator();
		const vector<Value> parseAssign( Symbol& selector );

		const vector<Value> parseArray();
		const vector<Value> parseBlock();
		const vector<Value> parseSlice();

	//
	// Optimization methods
	//

		bool optimizeOperator( vector<Value>& code, Symbol& name, Array& args );
		
	 private:
	 
	 	Lexer& _lexer;
	 	int    _lastOp;

		vector<Array> localArgsStack;
	 	
	};

}

#endif

