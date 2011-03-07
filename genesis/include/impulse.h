//
// impulse.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_H
#define IMPULSE_H

#ifdef DEBUG
	#define TRACE( expr ) std::cout << &spaces[60 - indent] << expr << std::endl
	#define ENTER( expr ) std::cout << &spaces[60 - indent] << "> " << expr << std::endl; indent += 4
	#define LEAVE( expr ) indent -= 4; std::cout << &spaces[60 - indent] << "< " << expr << std::endl
#else
	#define TRACE( expr )
	#define ENTER( expr )
	#define LEAVE( expr )
#endif

#define ASSERT( code ) std::cout << ((code) ? \
	"\x1b[32m\x1b[1mpass\x1b[0m" : "\x1b[31m\x1b[1mfail\x1b[0m") << " " << #code << std::endl

namespace impulse {

	extern const char spaces[];
	extern int        indent;
	
	class SymbolProto;
	typedef SymbolProto &Symbol;

 //
 // class UnitTest
 //

	class UnitTest {
	
	 public:
	 
		virtual void run() = 0;

	};

}

#endif

