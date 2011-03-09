//
// tests/block.cpp
//
// Copyright 2008-2011 Mike Austin
// All rights reserved.
//

#include "core/protos/block.h"

namespace impulse {

 //
 // class CoreTest
 //

	class BlockTest : public UnitTest {
	
	 public:
	 
		void run()
		{
			testBlock();
		}

		static Value foo( Value self, const Array& args, Value locals )
		{
			return self.getFloat() * args[Index::_0].getFloat();
		}
		
		void testBlock()
		{
			using namespace std;
			
			cout << "\nTesting Block..." << endl;
			cout << "------------------------------------------------------------" << endl;

			Frame lobby;

			BlockProto& block = *new BlockProto( foo, *new Array( SymbolProto::at( "x" ) ), lobby );

			ASSERT( block.arity() == 1 );
			ASSERT( block.value( 5, *new Array( 2 ) ).getFloat() == 10 );


			std::vector<Value> code;
			code.push_back( *new MessageProto( SymbolProto::at( "foo" ), *new Array( 10 ) ) );

			BlockProto& block2 = *new BlockProto( code, *new Array( SymbolProto::at( "x" ) ), lobby );

			ASSERT( block2.arity() == 1 );
			ASSERT( BlockProto::value_( block2, *new Array( 5, *new Array( 2 ) ), lobby ).getFloat() == 20 );
			ASSERT( block2.value( block2, *new Array( 5, *new Array( 2 ) ) ).getFloat() == 20 );

			cout << "------------------------------------------------------------" << endl;
		}

	};

}

