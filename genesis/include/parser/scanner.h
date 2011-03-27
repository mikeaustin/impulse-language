//
// core/parser/scanner.h
//
// Copyright 2008-2011 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_SCANNER_H
#define IMPULSE_SCANNER_H

#include "core/frame.h"
#include "core/array.h"

#include <sstream>
#include <cstdio>
#include <cstdlib>

namespace impulse {

 //
 // class Token
 //

	class Token {
	
	 public:

		enum Type
		{
			EMPTY,
			NUMBER, STRING, IDENT, KEYWORD, OPERAT,
			NEWLINE, END_FILE
		};

		Token()                         : _type( EMPTY ) { }
	 	Token( Type type, Value value ) : _type( type ), _value( value ) { }

		Type type() const { return _type; }
		Value value() const { return _value; }

	 private:
	 
	 	Type  _type;
		Value _value;
	
	};

 //
 // class Scanner
 //
 
 	class Scanner : public Frame {
 	
 	 public:
 	 
 		Scanner( std::istream& stream ) : _stream( stream ) { }
 		
 		std::istream& stream() { return _stream; }
 		
 		Token peekToken()
 		{
 			while (isspace( _stream.peek() ) && _stream.peek() != 10)
 				_stream.get();
 				
 			int c = _stream.peek();

//			std::vector<Token (Scanner::*)(const int) const>::const_iterator iter = _scanners.begin();
			
/*			while (iter != _scanners.end())
			{
				(*iter)->scan( c );
			}*/
 			
 			_token = c == 10  ? read( Token::NEWLINE )  :
 			         c == EOF ? read( Token::END_FILE ) :
			         Token();
 			         
 			return _token;
 		}

		Token read( Token::Type type )
		{
			std::stringstream buffer;
			
			buffer << (unsigned char) _stream.get();
			
			return Token( type, buffer.str() );
		}

	 	virtual Token scan( const int c ) const { return Token(); }
 		
 		void add( Token (Scanner::*scanner)(const int) const ) { }

 	 private:
 	 
 		std::istream& _stream;
 		Token         _token;
 		std::vector<Token (Scanner::*)(const int) const> _scanners;

 	};

	class NumberScanner : public Scanner {
	
	 public:
	 
	 	virtual Token scanNumber( const int c ) const { return Token(); }
	 	
	};

}

#endif

