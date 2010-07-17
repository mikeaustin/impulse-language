//
// parser/protos/lexer.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#include <sstream>
#include <cstdlib>

namespace impulse {

	class Token {

	 public:

		enum Type
		{
			INVALID,
			NEWLINE,
			LITERAL_NUMBER, LITERAL_STRING, IDENTIFIER, KEYWORD, OPERATOR,
			COMMA, ASSIGN, OPEN_BRACKET, CLOSE_BRACKET, VERTICAL_BAR
		};
	
		Token( Type type, Value value ) : _type( type ), _value( value ) { }
		Token()                         : _type( INVALID ) { }

		Type type() { return _type; }
		Value value() { return _value; }

		string getString() { return _value.get<String>().getValue(); }
		Symbol& getSymbol() { return _value.get<Symbol>(); }

	 private:
	
		Type  _type;
		Value _value;
	};

	class Lexer : public Frame {
	
	 public:

		static const char* ops;
	
		Lexer( istream& stream ) : _stream( stream ) { }

		bool isopera( int c )
		{
			char opstring[2] = { c, '\0' };
			char operators[] = "+-*/%<>$";

			return strpbrk( opstring, operators );
		}

		Token peekToken()
		{
			if (_token.type() != Token::INVALID)
				return _token;

			while (isspace( _stream.peek() ) && _stream.peek() != 10) _stream.get();

			int c = _stream.peek();
			
			if      (c == 10)           _token = readNewline();
			else if (c == '=')          _token = read( Token::ASSIGN );
			else if (c == ',')          _token = read( Token::COMMA );
			else if (c == '[')          _token = read( Token::OPEN_BRACKET );
			else if (c == ']')          _token = read( Token::CLOSE_BRACKET );
			else if (c == '|')          _token = read( Token::VERTICAL_BAR );
			else if (isdigit( c ))      _token = readNumber();
			else if (c == '"')          _token = readString();
			else if (isalpha( c ))      _token = readIdentifier();
			else if (isopera( c ))  _token = readIdentifier();
			else
			{
				cerr << "*** Unexpected token " << (unsigned char) c;
				
				exit( 1 );
			}
			
			return _token;
		}

		Token nextToken()
		{
			Token token = peekToken();
			
			_token = Token();
			
			return token;
		}

		Token read( Token::Type type )
		{
			stringstream buffer;

			buffer << (unsigned char) _stream.get();
			
			return Token( type, buffer.str() );
		}
		
		Token readNewline()
		{
			_stream.get();
			
			return Token( Token::NEWLINE, string( "\\n" ) );
		}

		Token readNumber()
		{
			stringstream buffer;
			
			while (isdigit( _stream.peek() ))
			{
				buffer << (unsigned char) _stream.get();
			}
			
			double value; buffer >> value;

			return Token( Token::LITERAL_NUMBER, value );
		}

		Token readString()
		{
			stringstream buffer;
			
			_stream.get();
			
			while (_stream.peek() != '"')
			{
				buffer << (unsigned char) _stream.get();
			}

			_stream.get();
			
			return Token( Token::LITERAL_STRING, buffer.str() );
		}

		Token readIdentifier()
		{
			stringstream buffer;
			bool alphanum = false;

			while (isalpha( _stream.peek() ) || isopera( _stream.peek() ))
			{
				alphanum |= isalpha( _stream.peek() );
				
				buffer << (unsigned char) _stream.get();
			}

			if (!alphanum)
			{
				return Token( Token::OPERATOR, Symbol::at( buffer.str() ) );
			}

			if (_stream.peek() == ':')
			{
				buffer << (unsigned char) _stream.get();
				
				return Token( Token::KEYWORD, Symbol::at( buffer.str() ) );
			}
			
			return Token( Token::IDENTIFIER, Symbol::at( buffer.str() ) );
		}

		Token XreadOperator()
		{
			stringstream buffer;

			while (isopera( _stream.peek() ))
			{
				buffer << (unsigned char) _stream.get();
			}
			
			return Token( Token::OPERATOR, Symbol::at( buffer.str() ) );
		}
	
	 private:
	
		istream& _stream;
		Token    _token;
	
	};

}

