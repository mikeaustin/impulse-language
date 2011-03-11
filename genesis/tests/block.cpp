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

			LocalsProto& lobby = *new LocalsProto();

			vector< GCValue::Type<SymbolProto> > argnames; argnames.push_back( SymbolProto::at( "x" ) );
			BlockProto& block = *new BlockProto( foo, argnames, lobby );

			ASSERT( block.arity() == 1 );
			ASSERT( block.value( 5, *new Array( 2 ) ).getFloat() == 10 );


			vector<GCValue> code;
			code.push_back( *new MessageProto( SymbolProto::at( "foo" ), *new ArrayProto( 10 ) ) );

			BlockProto& block2 = *new BlockProto( code, argnames, lobby );

			ASSERT( block2.arity() == 1 );
			ASSERT( BlockProto::value_( block2, *new Array( 5, *new ArrayProto( 2 ) ), lobby ).getFloat() == 20 );
			ASSERT( block2.value( block2, *new Array( 5, *new ArrayProto( 2 ) ) ).getFloat() == 20 );

			cout << "------------------------------------------------------------" << endl;
		}

	};

}

