//
// impulse.cpp
//
// Copyright 2008-2011 Mike Austin
// All rights reserved.
//

//#include "impulse.h"
//#include "core/all.h"
//#include "runtime/all.h"

#include <iostream>
#include <string>
#include <vector>
#include <numeric>

using std::string;
using std::vector;

using std::cin;
using std::cout;
using std::cerr;
using std::endl;

#include "impulse.h"

#include "core/protos/symbol.cpp"
#include "core/protos/number.cpp"

#include "core/protos/locals.h"
#include "core/protos/method.h"
#include "core/protos/stream.h"
#include "runtime/protos/message.h"
#include "runtime/protos/expression.h"

#include "core/value.cpp"
#include "core/frame.cpp"

#include "parser/protos/parser.h"

#include "parser/protos/scanner.cpp"
#include "runtime/protos/message.cpp"

#ifdef TEST
	#include "../tests/core.cpp"
	#include "../tests/block.cpp"
	#include "../tests/frame.cpp"
	#include "../tests/number.cpp"
#endif

namespace impulse {

	const char spaces[] = "                                                            ";
	int        indent = 0;
	
}

using namespace impulse;

//Value receiver;
//Array arguments;

int main( int argc, char* argv[] )
{
	if (1)
	{
		//cout << "------------------------------------------------------------" << endl;
		cout << "Impulse 0.2.0 — Copyright 2008-2011 Mike Austin" << endl;

		cout << endl;
	
		cout << "sizeof (double)   = " << sizeof (double)   << endl;
		cout << "sizeof (Value)    = " << sizeof (Value)    << endl;
		cout << "sizeof (Frame)    = " << sizeof (Frame)    << endl;
		cout << "sizeof (SymbolId) = " << sizeof (Frame::SymbolId) << endl;
		cout << "sizeof (Array)    = " << sizeof (Array)    << endl << endl;
	}

	Frame::ReleasePool releasePool;

	ObjectProto::initSlots();
	NumberProto::initSlots();

	if (1)
	{
		Frame::ReleasePool releasePool;

#ifdef TEST
		CoreTest().run();
		BlockTest().run();
		FrameTest().run();
		NumberTest().run();
#endif

#ifdef BENCH
		Array args();
	
		//for ( volatile int i = 0; i < 1000000000; i++ )
		{
			//Value( 10 ).apply( 10, args );
			//block.apply( 10, args );
		}
	
		cout << "Benchmark results:" << endl;
#endif
	}

	if (1)
	{
		Frame::ReleasePool releasePool;

		Frame& lobby = Frame::create();
		Value locals = *new LocalsProto( lobby );

		Scanner& scanner = *new Scanner( cin );
		StatementParser parser( scanner );

		Token token;

//		while (token = scanner.nextToken(), token.type() != NULL)
//		{
//			cout << "=> " << token.value().inspect() << endl;
//		}

		Array arguments;

		while (1)
		{
			cout << "] ";
			
			Expression& expression = parser.parse( 0 );

			Value receiver = expression.apply( locals, arguments, locals );

			if (&receiver.getFrame() != NULL)
			{
				cout << "= " << receiver << endl << endl;
			}
		}
	}
	
	return 0;
}

