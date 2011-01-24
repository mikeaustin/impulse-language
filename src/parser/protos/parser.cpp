//
// parser/protos/parser.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#include "parser.h"

#include <algorithm>

namespace impulse {

 //
 // Convenience methods
 //

	vector<Value>& operator <<( vector<Value>& vector1, const vector<Value>& vector2 )
	{
		vector1.insert( vector1.end(), vector2.begin(), vector2.end() );
		
		return vector1;
	}

	Token Parser::expect( const Token::Type type, const string value ) const
	{
		if (_lexer.peekToken().type() != type)
		{
			_lexer.flushLine();
			
			FAIL( "Expected " << value << ", found " << _lexer.nextToken().value().inspect() );
		}
		
		return _lexer.nextToken();
	}

	Token Parser::option( const Token::Type type, const string value ) const
	{
		if (_lexer.peekToken().type() == type)
		{
			return _lexer.nextToken();
		}
		
		return Token();
	}
/*
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
*/
 //
 // Parsing methods
 //

	const Expression Parser::parseLine()
	{
		if (_lexer.peekToken().type() == Token::NEWLINE)
		{
			_lexer.nextToken();
			
			vector<Value> code;
			code.push_back( Value() );
			Expression expr( code );
			
			return expr;
		}
		
		return parseStatement();
	}

	const Expression Parser::parseStatement()
	{
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

		switch (token.type())
		{
			case Token::LITERAL_NUMBER:	code << parseNumber(); break;
			case Token::LITERAL_STRING:	code << parseString(); break;
			case Token::IDENTIFIER:		code << parseIdentifier(); break;
			case Token::KEYWORD:		code << parseKeyword(); break;
			case Token::OPEN_PAREN:		code << parseSubexpr(); break;
			case Token::OPEN_BRACKET:	code << parseArray(); break;
			case Token::VERTICAL_BAR:	code << parseBlock(); break;
			default:
			{
				_lexer.flushLine();
				
				FAIL( "Expected expression, found \\n" );
			}
		}
		
		vector<Value> message;
		while ((message = (this->*parseMessage)()).size() > 0)
		{
			code << message;
		}

		return code;
	}

	const vector<Value> Parser::parseSubexpr()
	{
		vector<Value> code;
		
		expect( Token::OPEN_PAREN, "(" );

		code << parseExpression();

		expect( Token::CLOSE_PAREN, ")" );
		
		return code;
	}

	const vector<Value> Parser::parseMessage()
	{
		vector<Value> code;
		
		Token token = _lexer.peekToken();

		switch( token.type() )
		{
			case Token::IDENTIFIER:		code << parseSimpleIdentifier(); break;
			case Token::KEYWORD:		code << parseKeyword(); break;
			case Token::OPERATOR:		code << parseOperator(); break;
			case Token::OPEN_BRACKET:	code << parseSlice(); break;
			default:					;
		}
		
		return code;
	}

	const vector<Value> Parser::parseBinaryMessage()
	{
		vector<Value> code;
		
		Token token = _lexer.peekToken();

		switch (token.type())
		{
			case Token::IDENTIFIER:		code << parseIdentifier(); break;
			case Token::KEYWORD:		code << parseKeyword(); break;
			case Token::OPEN_BRACKET:	code << parseSlice(); break;
			default:					;
		}
		
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

		if (ident.getString() == "self")
		{
			code.push_back( *new SelfMessage() );
			
			return code;
		}

		Symbol& selector  = ident.getSymbol();
		Array&  emptyArgs = *new Array();

		Array& args = localArgsStack[localArgsStack.size() - 1];

		unsigned int index;
		for (index = 0; index < args.size(); ++index)
		{
			if (&args[index].get<Symbol>() == &ident.getSymbol())
			{
				//cout << "Found local " << ident.value() << endl;
				
				break;
			}
		}
		
		if (localsAccessEnabled && index != args.size())
		{
			code.push_back( *new LocalMessage( index, emptyArgs ) );
		
			return code;
		}

		code.push_back( *new Message( selector, emptyArgs ) );
		
		return code;
	}

	const vector<Value> Parser::parseIdentifier()
	{
		vector<Value> code;

		code = parseSimpleIdentifier();

		if (_lexer.peekToken().type() == Token::ASSIGN)
		{
			code = parseAssign( code[0].get<Message>()._selector );
		}
	
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

			Token keyword = option( Token::KEYWORD, "keyword" );
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

		if (&name == &Symbol::at( "?" ))
		{
			Expression& trueExpr = *new Expression( parseExpression() );
			args.push_back( trueExpr );

			if (option( Token::COLON, ":" ))
			{
				option( Token::NEWLINE, "\\n" );
				
				Expression& falseExpr = *new Expression( parseExpression() );
				args.push_back( falseExpr );
			}
		
			//code.push_back( *new Message( name, args ) );
			code.push_back( *new TernaryMessage( args ) );

			return code;
		}

		vector<Value> arg = parseExpression( &Parser::parseBinaryMessage );
		args.push_back( arg.size() == 1 ? arg[0] : *new Expression( arg ) );

		if (!optimizeOperator( code, name, args ))
		{
			code.push_back( *new Message( name, args ) );
		}
					
		return code;
	}

	const vector<Value> Parser::parseAssign( Symbol& selector )
	{
		vector<Value> code;

		expect( Token::ASSIGN, "=" );

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

		localArgsStack.push_back( args );

		expect( Token::VERTICAL_BAR, string( "|" ) );

		if (option( Token::NEWLINE, "\\n" ))
		{
			if (_lexer.peekToken().getString() != "end")
			{
				body = new Expression( parseStatement() );
				Expression* body2 = body;
			
				while (_lexer.peekToken().getString() != "end")
				{
					body2 = body2->setNextExpr( *new Expression( parseStatement() ) );
				}
			}
			else
			{
				vector<Value> empty;
				empty.push_back( Value() );
				
				body = new Expression( empty );
			}
			
			expect( Token::IDENTIFIER, "end" );
		}
		else
		{
			body = new Expression( parseExpression() );
		}

		localArgsStack.pop_back();

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
		
		//code.push_back( *new Message( Symbol::at( "[]" ), args ) );
		code.push_back( *new SliceMessage( args ) );
		
		return code;
	}

 //
 // Optimization methods
 //

#define OPERATOR_PARSER( _name_, _op_ ) \
	if (&name == &Symbol::at( #_op_ )) \
	{ \
		if (&args[0].getProto() == &Number::instance()) \
			code.push_back( *new Const##_name_##Message( args[0].getFloat(), args ) ); \
		else \
			code.push_back( *new _name_##Message( args ) ); \
	} else

	bool Parser::optimizeOperator( vector<Value>& code, Symbol& name, Array& args )
	{
		OPERATOR_PARSER( Add, + )
		OPERATOR_PARSER( Sub, - )
		OPERATOR_PARSER( Mul, * )
		OPERATOR_PARSER( Div, / )
		OPERATOR_PARSER( Mod, % )
		OPERATOR_PARSER( Equal, == )
		OPERATOR_PARSER( LessThan, < )
		OPERATOR_PARSER( LessEqual, <= )
		OPERATOR_PARSER( GreaterThan, > )
		OPERATOR_PARSER( GreaterEqual, >= )
		{
			return false;
		}
		
		return true;
	}

}

