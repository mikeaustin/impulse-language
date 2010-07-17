//
// parser/protos/parser.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#include "lexer.h"

#include <cstdlib>
#include <vector>

using namespace std;

namespace impulse {

	class Parser : public Frame	{

	 public:

		typedef const vector<Value> (Parser::*ParserFunc)();
		
		Parser( Lexer& lexer ) : _lexer( lexer ) { }

		void concat( vector<Value>& vector1, const vector<Value>& vector2 )
		{
			vector1.insert( vector1.end(), vector2.begin(), vector2.end() );
		}

		Token expect( Token::Type type, string value )
		{
			if (_lexer.peekToken().type() != type)
			{
				cerr << "*** Expected " << value << ", found " << _lexer.peekToken().value() << endl;
				
				exit( 1 );
			}
			
			return _lexer.nextToken();
		}

		bool option( Token::Type type, string value )
		{
			if (_lexer.peekToken().type() == type)
			{
				_lexer.nextToken();
				
				return true;
			}
			
			return false;
		}

		const Expression parseStatement( bool allowEmpty = false )
		{
			if (allowEmpty && _lexer.peekToken().type() == Token::NEWLINE)
			{
				_lexer.nextToken();
				
				vector<Value> code;
				code.push_back( Value() );
				Expression expr( code );
				
				return expr;
			}

			Expression expr( parseExpression() );

			expect( Token::NEWLINE, "\\n" );
			
			return expr;
		}

		const vector<Value> parseExpression( ParserFunc parseMessage = &Parser::parseMessage )
		{
			vector<Value> code;
			
			Token token = _lexer.peekToken();

			if      (token.type() == Token::LITERAL_NUMBER) concat( code, parseNumber() );
			else if (token.type() == Token::LITERAL_STRING) concat( code, parseString() );
			else if (token.type() == Token::IDENTIFIER)     concat( code, parseIdentifier() );
			else if (token.type() == Token::KEYWORD)        concat( code, parseKeyword() );
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

		const vector<Value> parseMessage()
		{
			vector<Value> code;
			
			Token token = _lexer.peekToken();

			if (token.type() == Token::IDENTIFIER)        concat( code, parseSimpleIdentifier() );
			else if (token.type() == Token::KEYWORD)      concat( code, parseKeyword() );
			else if (token.type() == Token::OPERATOR)     concat( code, parseOperator() );
			else if (token.type() == Token::OPEN_BRACKET) concat( code, parseSlice() );
			
			return code;
		}

		const vector<Value> parseBinaryMessage()
		{
			vector<Value> code;
			
			Token token = _lexer.peekToken();

			if (token.type() == Token::IDENTIFIER)    concat( code, parseIdentifier() );
			else if (token.type() == Token::KEYWORD)  concat( code, parseKeyword() );
			
			return code;
		}

		const vector<Value> parseNumber()
		{
			vector<Value> code;

			Token number = expect( Token::LITERAL_NUMBER, "number" );

			code.push_back( number.value() );
			
			return code;
		}

		const vector<Value> parseString()
		{
			vector<Value> code;

			Token string_ = expect( Token::LITERAL_STRING, "string" );

			Array& args = *new Array();
			args.push_back( string_.value() );

			code.push_back( *new StringMessage( args ) );
			
			return code;
		}

		const vector<Value> parseSimpleIdentifier()
		{
			vector<Value> code;

			Token ident = expect( Token::IDENTIFIER, "identifier" );

			Symbol& selector = ident.getSymbol();
			Array&  args     = *new Array();

			code.push_back( *new Message( selector, args ) );
			
			return code;
		}

		const vector<Value> parseAssign( Symbol& selector )
		{
			vector<Value> code;

			expect( Token::ASSIGN, "=" );

			Array&  args     = *new Array();
			vector<Value> arg = parseExpression();
			args.push_back( arg.size() == 1 ? arg[0] : *new Expression( arg ) );

			code.push_back( *new AssignMessage( selector, args ) );
			
			return code;
		}

		const vector<Value> parseIdentifier()
		{
			vector<Value> code;

			code = parseSimpleIdentifier();

			if (_lexer.peekToken().type() == Token::ASSIGN)
				code = parseAssign( code[0].get<Message>()._selector );
		
			return code;
		}

		const vector<Value> parseKeyword()
		{
			vector<Value> code;

			Token keyword = expect( Token::KEYWORD, "keyword" );

			Symbol& selector = keyword.getSymbol();
			Array&  args     = *new Array();

			vector<Value> arg = parseExpression();
			args.push_back( arg.size() == 1 ? arg[0] : *new Expression( arg ) );

			while (_lexer.peekToken().type() == Token::COMMA)
			{
				_lexer.nextToken();

				Token keyword = expect( Token::KEYWORD, "keyword" );
				arg = parseExpression();

				args.push_back( arg.size() == 1 ? arg[0] : *new Expression( arg ) );
			}

			code.push_back( *new Message( selector, args ) );
			
			return code;
		}

		const vector<Value> parseOperator()
		{
			vector<Value> code;

			Symbol& selector = _lexer.nextToken().getSymbol();
			Array&  args     = *new Array();

			vector<Value>arg = parseExpression( &Parser::parseBinaryMessage );
			args.push_back( arg.size() == 1 ? arg[0] : *new Expression( arg ) );

			code.push_back( *new Message( selector, args ) );
			
			return code;
		}

		const vector<Value> parseArray()
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

		const vector<Value> parseBlock()
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
				
				while (_lexer.peekToken().getString() != "end")
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
		
		const vector<Value> parseSlice()
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
		
	 private:
	 
	 	Lexer& _lexer;
	 	
	};

}

