//
// parser/protos/parser.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#include "parser.h"

namespace impulse {

	void concat( vector<Value>& vector1, const vector<Value>& vector2 )
	{
		vector1.insert( vector1.end(), vector2.begin(), vector2.end() );
	}

 //
 // Convenience methods
 //

	Token Parser::expect( const Token::Type type, const string value ) const
	{
		if (_lexer.peekToken().type() != type)
		{
			cerr << "*** Expected " << value << ", found " << _lexer.peekToken().value() << endl;
			
			exit( 1 );
		}
		
		return _lexer.nextToken();
	}

	bool Parser::option( const Token::Type type, const string value ) const
	{
		if (_lexer.peekToken().type() == type)
		{
			_lexer.nextToken();
			
			return true;
		}
		
		return false;
	}

	int Parser::precedence( const string operator_ ) const
	{
		if (operator_ == "*") return 3;
		if (operator_ == "/") return 3;
		if (operator_ == "%") return 3;

		if (operator_ == "+") return 4;
		if (operator_ == "-") return 4;

		if (operator_ == "<") return 6;
		if (operator_ == ">") return 6;
		if (operator_ == "<=") return 6;
		if (operator_ == ">=") return 6;
		if (operator_ == "==") return 6;

		return 0;
	}

 //
 // Parsing methods
 //

	const Expression Parser::parseStatement( bool allowEmpty )
	{
		if (allowEmpty && _lexer.peekToken().type() == Token::NEWLINE)
		{
			_lexer.nextToken();
			
			vector<Value> code;
			code.push_back( Value() );
			Expression expr( code );
			
			return expr;
		}

		if (_lexer.stream().peek() == EOF)
		{
			exitMainLoop = true;

			vector<Value> code;
			Expression expr( code );
			
			return expr;
		}
		
		Expression expr( parseExpression() );

		expect( Token::NEWLINE, "\\n" );
		
		return expr;
	}

	const vector<Value> Parser::parseExpression( ParserFunc parseMessage )
	{
		vector<Value> code;
		
		Token token = _lexer.peekToken();

		if      (token.type() == Token::LITERAL_NUMBER) concat( code, parseNumber() );
		else if (token.type() == Token::LITERAL_STRING) concat( code, parseString() );
		else if (token.type() == Token::IDENTIFIER)     concat( code, parseIdentifier() );
		else if (token.type() == Token::KEYWORD)        concat( code, parseKeyword() );
		else if (token.type() == Token::OPEN_PAREN)     concat( code, parseSubexpr() );
		else if (token.type() == Token::OPEN_BRACKET)   concat( code, parseArray() );
		else if (token.type() == Token::VERTICAL_BAR)   concat( code, parseBlock() );
		else
		{
			cerr << "*** Expected expression, found \\n" << endl;

			exit( 1 );
		}

		vector<Value> message;
		while ((message = (this->*parseMessage)()).size() > 0)
		{
			concat( code, message );
		}

		return code;
	}

	const vector<Value> Parser::parseSubexpr()
	{
		vector<Value> code;
		
		expect( Token::OPEN_PAREN, "(" );

		concat( code, parseExpression() );

		expect( Token::CLOSE_PAREN, ")" );
		
		return code;
	}

	const vector<Value> Parser::parseMessage()
	{
		vector<Value> code;
		
		Token token = _lexer.peekToken();

		if (token.type() == Token::IDENTIFIER)        concat( code, parseSimpleIdentifier() );
		else if (token.type() == Token::KEYWORD)      concat( code, parseKeyword() );
		else if (token.type() == Token::OPERATOR)     concat( code, parseOperator() );
		else if (token.type() == Token::OPEN_BRACKET) concat( code, parseSlice() );
		
		return code;
	}

	const vector<Value> Parser::parseBinaryMessage()
	{
		vector<Value> code;
		
		Token token = _lexer.peekToken();

		if (token.type() == Token::IDENTIFIER)        concat( code, parseIdentifier() );
		else if (token.type() == Token::KEYWORD)      concat( code, parseKeyword() );
		else if (token.type() == Token::OPEN_BRACKET) concat( code, parseSlice() );
		
		return code;
	}

	const vector<Value> Parser::parseNumber()
	{
		vector<Value> code;

		Token number = expect( Token::LITERAL_NUMBER, "number" );

		code.push_back( number.value() );
		
		return code;
	}

	const vector<Value> Parser::parseString()
	{
		vector<Value> code;

		Token string_ = expect( Token::LITERAL_STRING, "string" );

		Array& args = *new Array();
		args.push_back( string_.value() );

		code.push_back( *new StringMessage( args ) );
		
		return code;
	}

	const vector<Value> Parser::parseSimpleIdentifier()
	{
		vector<Value> code;

		Token ident = expect( Token::IDENTIFIER, "identifier" );

		Symbol& selector = ident.getSymbol();
		Array&  args     = *new Array();

		code.push_back( *new Message( selector, args ) );
		
		return code;
	}

	const vector<Value> Parser::parseIdentifier()
	{
		vector<Value> code;

		code = parseSimpleIdentifier();

		if (_lexer.peekToken().type() == Token::OPERATOR &&
			_lexer.peekToken().getString() == "=")
			code = parseAssign( code[0].get<Message>()._selector );
	
		return code;
	}

	const vector<Value> Parser::parseKeyword()
	{
		vector<Value> code;

		Token keyword = expect( Token::KEYWORD, "keyword" );

		Symbol& name = keyword.getSymbol();
		Array&  args = *new Array();

		vector<Value> arg = parseExpression();
		args.push_back( arg.size() == 1 ? arg[0] : *new Expression( arg ) );

		while (_lexer.peekToken().type() == Token::COMMA)
		{
			_lexer.nextToken();

			Token keyword = expect( Token::KEYWORD, "keyword" );
			arg = parseExpression();

			args.push_back( arg.size() == 1 ? arg[0] : *new Expression( arg ) );
		}

		code.push_back( *new Message( name, args ) );
		
		return code;
	}

	const vector<Value> Parser::parseOperator()
	{
		vector<Value> code;

		Symbol& name = expect( Token::OPERATOR, "operator" ).getSymbol();
		Array&  args = *new Array();
/*
		if (precedence( name.getName() ) == 6)
		{
			Expression& expr = *new Expression( parseExpression() );
			
			args.push_back( expr );
			code.push_back( *new Message( name, args ) );

			_lastOp = 6;

			return code;
		}
*/
		if (&name == &Symbol::at( "?" ))
		{
			Expression& trueExpr = *new Expression( parseExpression() );
			Array& blockArgs = *new Array();
			
			args.push_back( *new BlockMessage( blockArgs, trueExpr ) );

			if (option( Token::COLON, ":" ))
			{
				option( Token::NEWLINE, "\\n" );
				
				Expression& falseExpr = *new Expression( parseExpression() );

				args.push_back( *new BlockMessage( blockArgs, falseExpr ) );
			}
		
			code.push_back( *new Message( name, args ) );

			_lastOp = 3;
			
			return code;
		}

		vector<Value>arg = parseExpression( &Parser::parseBinaryMessage );
		args.push_back( arg.size() == 1 ? arg[0] : *new Expression( arg ) );

		if (&name == &Symbol::at( "*" ))
		{
			if (&args[0].getProto() == &Number::instance())
				code.push_back( *new ConstMulMessage( args[0].getFloat(), args ) );
			else
				code.push_back( *new MulMessage( args ) );
		}
		else
		{
			code.push_back( *new Message( name, args ) );
		}
					
		return code;
	}

	const vector<Value> Parser::parseAssign( Symbol& selector )
	{
		vector<Value> code;

		expect( Token::OPERATOR, "=" );

		Array&  args     = *new Array();
		vector<Value> arg = parseExpression();
		args.push_back( arg.size() == 1 ? arg[0] : *new Expression( arg ) );

		code.push_back( *new AssignMessage( selector, args ) );
		
		return code;
	}

	const vector<Value> Parser::parseArray()
	{
		vector<Value> code;
		Array values;

		expect( Token::OPEN_BRACKET, string( "[" ) );

		if (_lexer.peekToken().type() != Token::CLOSE_BRACKET)
		{
			do
			{
				vector<Value> arg = parseExpression();
				
				values.push_back( arg.size() == 1 ? arg[0] : *new Expression( arg ) );
			}
			while (option( Token::COMMA, "," ));
		}
		
		expect( Token::CLOSE_BRACKET, string( "]" ) );

		code.push_back( *new ArrayMessage( values ) );
		
		return code;
	}

	const vector<Value> Parser::parseBlock()
	{
		vector<Value> code;
		Expression* body;
		Array args;
		
		expect( Token::VERTICAL_BAR, string( "|" ) );

		if (_lexer.peekToken().type() != Token::VERTICAL_BAR)
		{
			do
			{
				Symbol& symbol = expect( Token::IDENTIFIER, "identifier" ).getSymbol();

				args.push_back( symbol );
			}
			while (option( Token::COMMA, "," ));
		}

		expect( Token::VERTICAL_BAR, string( "|" ) );

		if (option( Token::NEWLINE, "\\n" ))
		{
			body = new Expression( parseStatement() );
			Expression* body2 = body;
			
			while (_lexer.peekToken().type() == Token::IDENTIFIER
				   && _lexer.peekToken().getString() != "end")
			{
				body2 = body2->setNextExpr( *new Expression( parseStatement() ) );
			}

			expect( Token::IDENTIFIER, "end" );
		}
		else
		{
			body = new Expression( parseExpression() );
		}

		code.push_back( *new BlockMessage( args, *body ) );
		
		return code;
	}

	const vector<Value> Parser::parseSlice()
	{
		vector<Value> code;
		Array args;
		
		expect( Token::OPEN_BRACKET, string( "[" ) );

		if (_lexer.peekToken().type() != Token::CLOSE_BRACKET)
		{
			do
			{
				args.push_back( *new Expression( parseExpression() ) );
			}
			while (option( Token::COMMA, "," ));
		}

		expect( Token::CLOSE_BRACKET, string( "]" ) );
		
		code.push_back( *new Message( Symbol::at( "[]" ), args ) );
		
		return code;
	}

}

