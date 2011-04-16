//
// impulse.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_H
#define IMPULSE_H

#ifdef TRACE
	#undef TRACE
	#define TRACE( expr ) cout << &spaces[60 - indent] << expr << endl
	#define ENTER( expr ) cout << &spaces[60 - indent] << "> " << expr << endl; indent += 4
	#define LEAVE( expr ) indent -= 4; cout << &spaces[60 - indent] << "< " << expr << endl
#else
	#define TRACE( expr )
	#define ENTER( expr )
	#define LEAVE( expr )
#endif

#define ASSERT( code ) cout << ((code) ? \
	"\x1b[32m\x1b[1mpass\x1b[0m" : "\x1b[31m\x1b[1mfail\x1b[0m") << " " << #code << endl

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

