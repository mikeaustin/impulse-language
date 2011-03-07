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

#include "core/protos/symbol.cpp"

namespace impulse {

	const char spaces[] = "                                                            ";
	int        indent = 0;
	
}

using namespace impulse;

int main( int argc, char* argv[] )
{
	if (1)
	{
		std::cout << "------------------------------------------------------------" << std::endl;
		std::cout << "Impulse 0.2.0 — Copyright 2008-2011 Mike Austin" << std::endl;

		std::cout << std::endl;
	
		std::cout << "sizeof (double)   = " << sizeof (double)   << std::endl;
		std::cout << "sizeof (Value)    = " << sizeof (Value)    << std::endl;
		std::cout << "sizeof (Frame)    = " << sizeof (Frame)    << std::endl;
		std::cout << "sizeof (SymbolId) = " << sizeof (SymbolId) << std::endl;
		std::cout << "sizeof (SlotMap)  = " << sizeof (SlotMap)  << std::endl;
		//std::cout << "sizeof (Array)    = " << sizeof (Array)    << std::endl;

		std::cout << std::endl;
	}

	Frame::ReleasePool releasePool;

	NumberProto::initSlots();

	Frame lobby;

	lobby.setSlot( "foo", 10 );
	lobby.setSlot( "bar", 20 );

	// TODO: Make instance() hidden to users so that it is can't be collected?
	//GCValue test( VoidProto::instance() );
	GCValue test2();

	const Symbol foo = SymbolProto::at( "foo" );
	
	for ( volatile int i = 0 ; i < 3 ; i++ )
	{
		Frame::ReleasePool releasePool;

		std::cout << "..." << std::endl;
		
		for ( volatile int j = 0 ; j < 3 ; j++ )
		{
			//std::cout << lobby.getSlot( "foo" ).getFrame().getProto().getSlot( "foo" ) << std::endl;
			//lobby.getSlot( "foo" ).getFrame().getProto().getSlot( "x" );
			lobby.setSlot( foo, foo );
			lobby.setSlot( "bar", *new Frame() );
		}
		
		std::cout << lobby.getSlot( foo ) << std::endl;
		std::cout << "Release pool size = " << releasePool.size() << std::endl;
	}

	//std::cin.get();
	
	return 0;
}

