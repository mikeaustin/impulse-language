//
// impulse.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_H
#define IMPULSE_H

#ifdef DEBUG
 #define ENTER( expr ) clog << &spaces[60 - indent] << "> " << expr << endl; indent += 4
 #define LEAVE( expr ) indent -= 4; clog << &spaces[60 - indent] << "< " << expr << endl
#else
 #define ENTER( expr )
 #define LEAVE( expr )
#endif
#define TRACE( expr ) clog << &spaces[60 - indent] << expr << endl;

#define ASSERT( code ) cout << ((code) ? \
  "\x1b[32m\x1b[1mpass\x1b[0m" : "\x1b[31m\x1b[1mfail\x1b[0m") << " " << #code << endl
#define OUTPUT( code ) code; cout << "---- " << #code << endl

#define OPERATOR_METHOD( function, op ) \
		static Value function( Value receiver, const Array& args, Value context ) \
		{ \
			return receiver.getFloat() op args[0].getFloat(); \
		}

namespace impulse {

	const char spaces[] = "                                                            ";
	int        indent = 0;

	const bool methodCaching = false;
	const bool debugGarbage  = false;
	
	bool exitMainLoop = false;

	class UnitTest {

	 public:

		virtual ~UnitTest() { }

		virtual void runTests() { }

    };
}

#endif

