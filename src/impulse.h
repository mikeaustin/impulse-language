//
// impulse.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#ifdef DEBUG
 const char spaces[] = "                                                            ";
 int        indent = 0;

 #define ENTER( expr ) cout << &spaces[sizeof(spaces) - indent] << "> " << expr << endl; indent += 2
 #define LEAVE( expr ) indent -= 2; cout << &spaces[sizeof(spaces) - indent] << "< " << expr << endl
#else
 #define ENTER( expr )
 #define LEAVE( expr )
#endif

#define ASSERT( code ) cout << ((code) ? \
  "\x1b[32m\x1b[1mpass\x1b[0m" : "\x1b[31m\x1b[1mfail\x1b[0m") << " " << #code << endl
#define OUTPUT( code ) code; cout << "---- " << #code << endl

namespace impulse {

	class UnitTest {

	 public:

		virtual ~UnitTest() { }

		virtual void runTests() { }

    };
}

