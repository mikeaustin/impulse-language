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

using std::string;
using std::cin;
using std::cout;
using std::endl;

#include "impulse.h"

#include "core/protos/locals.h"
#include "core/protos/symbol.cpp"
#include "core/protos/locals.h"
#include "core/protos/method.h"
#include "runtime/protos/message.h"

#include "core/value.cpp"
#include "core/frame.cpp"

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

int main( int argc, char* argv[] )
{
	if (1)
	{
		//std::cout << "------------------------------------------------------------" << std::endl;
		std::cout << "Impulse 0.2.0 — Copyright 2008-2011 Mike Austin" << std::endl;

		std::cout << std::endl;
	
		std::cout << "sizeof (double)   = " << sizeof (double)   << std::endl;
		std::cout << "sizeof (Value)    = " << sizeof (Value)    << std::endl;
		std::cout << "sizeof (Frame)    = " << sizeof (Frame)    << std::endl;
		std::cout << "sizeof (SymbolId) = " << sizeof (Frame::SymbolId) << std::endl;
		std::cout << "sizeof (Array)    = " << sizeof (Array)    << std::endl;
	}

	{
		Frame::ReleasePool releasePool;

		ObjectProto::initSlots();
		NumberProto::initSlots();

#ifdef TEST
		CoreTest().run();
		BlockTest().run();
		FrameTest().run();
		NumberTest().run();
#endif

#ifdef BENCH
		Array args;
	
		//for ( volatile int i = 0; i < 1000000000; i++ )
		{
			//Value( 10 ).apply( 10, args );
			//block.apply( 10, args );
		}
	
		std::cout << "Benchmark results:" << std::endl;
#endif

		Frame& lobby  = Frame::create();
		LocalsProto& locals = *new LocalsProto( lobby );

		std::vector< std::vector<Value> > code;
		code.push_back( std::vector<Value>() );
		
		code.back().push_back( *new SelfMessage() );
		//code.back().push_back( *new MessageProto( SymbolProto::at( "pow" ), *new ArrayProto( *new SelfMessage() ) ) );
		code.back().push_back( *new PowMessage( *new ArrayProto( *new SelfMessage() ) ) );
	
		Value receiver = locals;
		Array arguments;
		arguments.self( 5 );

		for (int i = 0; i < 20000000; i++)
		{
			std::vector< std::vector<Value> >::iterator line = code.begin();

			while (line != code.end())
			{
				std::vector<Value>::iterator message = line->begin();

				TRACE( "" );

				while (message != line->end())
				{
					receiver = (*message++).apply( receiver, arguments, locals );
			
					TRACE( "" );
				}
			
				++line;
			}
		}
		
		cout << "result = " << receiver << endl;
	}

	return 0;
}

