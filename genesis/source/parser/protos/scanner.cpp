//
// parser/scanner.cpp
//
// Copyright 2008-2011 Mike Austin
// All rights reserved.
//

#include "parser/protos/scanner.h"

using std::stringstream;

namespace impulse {

	inline Token::Token()                         : _type( &Parser::unknown_ ) { }
	inline Token::Token( Type type, Value value ) : _type( type ), _value( value ) { }

 //
 //
 //

	Scanner::Scanner( istream& stream ) : _stream( stream )
	{
		_readers.push_back( &Scanner::lit_number_ );
		_readers.push_back( &Scanner::lit_string_ );
		_readers.push_back( &Scanner::identifier_ );
		_readers.push_back( &Scanner::operator_ );
		_readers.push_back( &Scanner::openparen_ );
		_readers.push_back( &Scanner::closeparen_ );
		_readers.push_back( &Scanner::verticalbar_ );
		_readers.push_back( &Scanner::endline_ );
	}

	Token Scanner::peekToken()
	{
		if (_token.type() != &Parser::unknown_) return _token;
		
		while (isspace( stream().peek() ) && stream().peek() != 10)
			stream().get();
		
		vector<Token (Scanner::*)(int c)>::iterator reader = _readers.begin();

		while (_token.type() == &Parser::unknown_ && reader != _readers.end())
		{
			_token = (this->*(*reader++))( stream().peek() );
		}

		if (_token.type() == &Parser::unknown_)
		{
			cerr << "*** Unknown token: '" << (unsigned char) stream().peek() << "'" << endl;
			
			while (stream().peek() != 10)
				stream().get();
		}
		
		return _token;
	}
	
	Token Scanner::nextToken()
	{
		Token token = peekToken();
		
		_token = Token();
		
		return token;
	}

	Token Scanner::currToken()
	{
		return _token;
	}

 //
 //
 //
 
	Token Scanner::lit_number_( int c )
	{
		if (!isdigit( c )) return Token();
		
		stringstream buffer;
		
		while (isdigit( stream().peek() ))
		{
			buffer << (unsigned char) stream().get();
		}

		if (stream().peek() == '.')
		{
			buffer << (unsigned char) stream().get();

			if (!isdigit( stream().peek() ))
			{
				stream().unget();
			}
			else while (isdigit( stream().peek() ))
			{
				buffer << (unsigned char) stream().get();
			}
		}

		float value; buffer >> value;

		return Token( &Parser::lit_number_, value );
	}

	Token Scanner::lit_string_( int c )
	{
		if (c != '"') return Token();
		
		stream().get();
		
		stringstream buffer;
		
		while (stream().peek() != '"')
		{
			buffer << (unsigned char) stream().get();
		}
		
		stream().get();
		
		return Token( &Parser::lit_string_, buffer.str() );
	}

	Token Scanner::identifier_( int c )
	{
		if (!isalpha( c )) return Token();
		
		stringstream buffer;
		
		while (isalpha( stream().peek() ))
		{
			buffer << (unsigned char) stream().get();
		}
		
		return Token( &Parser::identifier_, buffer.str() );
	}

	Token Scanner::operator_( int c )
	{
		if (!isopera( c )) return Token();
		
		stringstream buffer;
		
		while (isopera( stream().peek() ))
		{
			buffer << (unsigned char) stream().get();
		}
		
		if (buffer.str() == "=")
		{
			return Token( &Parser::assignment_, buffer.str() );
		}
		
		return Token( &Parser::operator_, buffer.str() );
	}

	Token Scanner::openparen_( int c )
	{
		if (c != '(') return Token();

		return Token( &Parser::openparen_, string( 1, stream().get() ) );
	}

	Token Scanner::closeparen_( int c )
	{
		if (c != ')') return Token();

		return Token( &Parser::closeparen_, string( 1, stream().get() ) );
	}

	Token Scanner::verticalbar_( int c )
	{
		if (c != '|') return Token();

		return Token( &Parser::verticalbar_, string( 1, stream().get() ) );
	}

	Token Scanner::endline_( int c )
	{
		if (c != 10) return Token();

		stream().get();
		
		return Token( &Parser::endline_, string( "\\n" ) );
	}

}

