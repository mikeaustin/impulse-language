//
// parser/scanner.cpp
//
// Copyright 2008-2011 Mike Austin
// All rights reserved.
//

#include "parser/scanner.h"

namespace impulse {

	Token Scanner::lit_number_( int c )
	{
		if (!isdigit( c )) return Token();
		
		std::stringstream buffer;
		
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

