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
#include <cstring>

using std::istream;

namespace impulse {

	class Parser;

 //
 // class Token
 //

	class Token {
	
	 public:
	
		typedef Value (Parser::*Type)(Token, int);

		Token();
		Token( Type type, Value value );
		
		Type  type()  { return _type; }
		Value value() { return _value; }
	
	 private:
	
		Type    _type;
		GCValue _value;

	};

 //
 // class Scanner
 //
 
 	class Scanner : public Frame {
 	
 	 public:
 	
 		Scanner( istream& stream );
 		
 		istream& stream() { return _stream; }
 		
 		Token peekToken();
 		Token nextToken();
 		Token currToken();

		bool isopera( int c )
		{
			char opstring[2] = { c, '\0' };
			char operators[] = "+-*%/<>=.";

			return strpbrk( opstring, operators );
        }
 		
 		Token lit_number_( int c );
 		Token lit_string_( int c );
 		Token identifier_( int c );
 		Token operator_( int c );
 		Token openparen_( int c );
 		Token closeparen_( int c );
 		Token verticalbar_( int c );
 		Token endline_( int c );
 	
 	 private:

		istream& _stream;
		vector<Token (Scanner::*)(int c)> _readers;
		Token _token;

 	};
	
/*
	class Token {
	
	 public:

		enum Type
		{
			EMPTY,
			NUMBER, STRING, IDENT, KEYWORD, OPERAT,
			OPEN_PAREN, CLOSE_PAREN,
			NEWLINE, END_FILE
		};

		Token()                         : _type( EMPTY ) { }
	 	Token( Type type, Value value ) : _type( type ), _value( value ) { }

		Type  type()  const { return _type; }
		Value value() const { return _value; }

		Token operator ||( const Token other )
		{
			if (type() != Token::EMPTY)
				return *this;
			else
				return other;
		}

		operator bool() { return type() != EMPTY; }

	 private:
	 
	 	Type  _type;
		Value _value;
	
	};

	bool operator <( const Token left, const Token right ) { return left.type() < right.type(); }

 //
 // class Scanner
 //
 
 	class Scanner : public Frame {

	 public:

		class Scanlet {
	
		 public:
		 
			Scanlet( istream& stream ) : _stream( stream ) { }
		
			virtual Token scan() = 0;
		
			istream& stream() { return _stream; }

            bool isopera( int c )
            {
                    char opstring[2] = { c, '\0' };
                    char operators[] = "+-*%/<>=";

                    return strpbrk( opstring, operators );
            }
		
		 private:
	
			istream& _stream;
		};
 	
 	 public:
 	 
 		Scanner( istream& stream );
 		
 		istream& stream() { return _stream; }

		void addParselet( Scanlet& scanlet )
		{
			_scanlets.push_back( &scanlet );
		}
 		
 		Token peekToken()
 		{
 			if (_token.type() != Token::EMPTY)
 				return _token;
 			
			while (isspace( _stream.peek() ) && _stream.peek() != 10)
				_stream.get();
 				
 			int c = _stream.peek();

			vector<Scanlet*>::const_iterator iter = _scanlets.begin();

			while (iter != _scanlets.end())
			{
				_token = (*iter)->scan();
				
				if (_token.type() != Token::EMPTY)
				{
					break;
				}
				
				++iter;
			}

			if (_token.type() == Token::EMPTY)
			{
				cerr << "*** Unexpected token: '" << (unsigned char) c << "'" << endl;
			}

 			return _token;
 		}

		Token nextToken()
		{
			Token token = peekToken();
			
			_token = Token();
			
			return token;
		}

 	 private:
 	 
 		istream& _stream;
 		Token         _token;
 		vector<Scanlet*> _scanlets;

 	};

 //
 // SymbolScanlet
 //

	class SymbolScanlet : public Scanner::Scanlet {
	
	 public:
	
		SymbolScanlet( istream& stream ) : Scanlet( stream ) { }

		virtual Token scan()
		{
			int c = stream().peek();
			
			Token token;
			
			if      (c == 10)    token = Token( Token::NEWLINE, string( "\\n" ) );
			else if (c == '(')   token = Token( Token::OPEN_PAREN, string( 1, c ) );
			else if (c == ')')   token = Token( Token::CLOSE_PAREN, string( 1, c ) );
			else if (isopera(c)) token = Token( Token::CLOSE_PAREN, string( 1, c ) );
			
			if (token.type() != Token::EMPTY)
				stream().get();
			
			return token;
		}

	};

	class NumberScanlet : public Scanner::Scanlet {
	
	 public:

		NumberScanlet( istream& stream ) : Scanlet( stream ) { }
	
		virtual Token scan()
		{
			if (!isdigit( stream().peek() )) return Token();
			
			stringstream buffer;
			
			while (isdigit( stream().peek() ))
			{
				buffer << (unsigned char) stream().get();
			}

			if (stream().peek() == '.')
			{
				buffer << (unsigned char) stream().get();
				
				while (isdigit( stream().peek() ))
				{
					buffer << (unsigned char) stream().get();
				}
			}

			float value; buffer >> value;

			return Token( Token::NUMBER, value );
		}
	};

	class StringScanlet : public Scanner::Scanlet {
	
	 public:

		StringScanlet( istream& stream ) : Scanlet( stream ) { }

		virtual Token scan()
		{
			if (stream().peek() != '"') return Token();

			stream().get();
			
			stringstream buffer;
			
			while (stream().peek() != '"')
			{
				buffer << (unsigned char) stream().get();
			}

			stream().get();

			return Token( Token::STRING, buffer.str() );
		}
	};

	class IdentScanlet : public Scanner::Scanlet {
	
	 public:

		IdentScanlet( istream& stream ) : Scanlet( stream ) { }

		virtual Token scan()
		{
			if (!isalpha(stream().peek())) return Token();

			stringstream buffer;
			
			while (isalpha(stream().peek()))
			{
				buffer << (unsigned char) stream().get();
			}

			return Token( Token::IDENT, SymbolProto::at( buffer.str() ) );
		}
	};

	Scanner::Scanner( istream& stream ) : _stream( stream )
	{
		addParselet( *new SymbolScanlet( cin ) );
		addParselet( *new NumberScanlet( cin ) );
		addParselet( *new StringScanlet( cin ) );
		addParselet( *new IdentScanlet( cin ) );
	}
*/

}

#endif

/*
		stringstream _buffer, _subpattern;
		unsigned _index;

		bool parse( string pattern )
		{
			unsigned int index = 0;
			bool found = false;
			
			while (index < pattern.size())
			{
				if (pattern[index] == '%' && pattern[index + 1] == 'd')
				{
					index += 2;

					if (isdigit( _stream.peek() ))
					{
						found = true;
						
						_buffer << (unsigned char) _stream.get();
					}
				}
				else
				{
					index += 1;
				}
			}
			
			return found;
		}

		string match( string pattern )
		{
			//stringstream subpattern2;
	
			while (_index < pattern.size())
			{
				if (pattern[_index] == '(')
				{
					if (_index > 0) parse( _subpattern.str() );

					_index += 1;
					
					match( pattern );
				}
				else if (pattern[_index] == ')')
				{
					_index += 1;
					
					if (pattern[_index] == '*')
					{
						while( parse( _subpattern.str() ) );

						return _buffer.str();
					}
				}
				else _subpattern << pattern[_index];
				
				_index += 1;
			}

			parse( _subpattern.str() );

			return _buffer.str();
		}

		Token scanNumber() const
		{
			double value;
			
			if (scanf( "%lf", &value ))
			{
				return Token( Token::NUMBER, value );
			}
			
			return Token();
		}
*/

