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

#include "impulse.h"

#include "core/value.cpp"
#include "core/frame.cpp"

#include "core/protos/block.h"

#include "core/protos/symbol.cpp"
#include "runtime/protos/message.h"

#ifdef TEST
	#include "../tests/core.cpp"
	#include "../tests/block.cpp"
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
		std::cout << "sizeof (SymbolId) = " << sizeof (SymbolId) << std::endl;
		std::cout << "sizeof (SlotMap)  = " << sizeof (SlotMap)  << std::endl;
		std::cout << "sizeof (Array)    = " << sizeof (Array)    << std::endl;
	}

	Frame::ReleasePool releasePool;

	NumberProto::initSlots();

#ifdef TEST
	CoreTest().run();
	BlockTest().run();
#endif

#ifdef BENCH
	Array args;
	
	for ( volatile int i = 0; i < 1000000000; i++ )
	{
		//Value( 10 ).apply( 10, args );
		//block.apply( 10, args );
	}
	
	std::cout << "Benchmark results:" << std::endl;
#endif

	Frame lobby;

	std::vector<Value> code;
	code.push_back( *new SelfMessage() );
	code.push_back( *new MessageProto( SymbolProto::at( "foo" ), *new ArrayProto( 10 ) ) );
	
	Value receiver = lobby;
	Array arguments;
	
	std::vector<Value>::iterator message = code.begin();

	while (message != code.end())
	{
		receiver = (*message++).apply( arguments );
	}
	
	return 0;
}

