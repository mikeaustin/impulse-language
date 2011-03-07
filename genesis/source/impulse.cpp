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

#ifdef TEST
	#include "tests/core.cpp"
	#include "tests/block.cpp"
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
		//std::cout << "sizeof (Array)    = " << sizeof (Array)    << std::endl;
	}

	Frame::ReleasePool releasePool;

	NumberProto::initSlots();

#ifdef TEST
	CoreTest().run();
	BlockTest().run();
#endif

#ifdef BENCH
	for ( volatile int i = 0; i < 1000000000; i++ )
	{
		Value( 10 ).value();
		block.value();
	}
	
	std::cout << "Benchmark results:" << std::endl;
#endif

	return 0;
}

