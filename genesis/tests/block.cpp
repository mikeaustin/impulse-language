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

		Value foo_( Value self, const Array& args, Value locals )
		{
			return self.getFloat() * args[Index::_0].getFloat();
		}
		
		void testBlock()
		{
			using namespace std;
			
			cout << "\nTesting Block..." << endl;
			cout << "------------------------------------------------------------" << endl;

			Frame lobby;
			Frame locals( lobby );
			vector< GCValue::Type<SymbolProto> > argnames; argnames.push_back( SymbolProto::at( "x" ) );

			BlockProto2<BlockTest>& block = *new BlockProto2<BlockTest>( *this, &BlockTest::foo_, argnames, locals );

			ASSERT( block.arity() == 1 );
			ASSERT( block.value( 5, *new Array( 2 ) ).getFloat() == 10 );


			vector<GCValue> code;
			code.push_back( *new MessageProto( SymbolProto::at( "foo" ), *new ArrayProto( 10 ) ) );

			BlockProto2<>& block2 = *new BlockProto2<>( code, argnames, locals );

			ASSERT( block2.arity() == 1 );
			ASSERT( block2.value( 5, *new Array( 2 ) ).getFloat() == 20 );

			cout << "------------------------------------------------------------" << endl;
		}

	};

}

