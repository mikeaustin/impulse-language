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
	 
		virtual void run()
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

			Frame& lobby  = Frame::create();
			Frame& locals = Frame::create( lobby );

			static std::vector<ArgType> argtypes;
			argtypes.push_back( ArgType( SymbolProto::at( "n" ), NumberProto::instance() ) );

			BlockProto<BlockTest>& block = *new BlockProto<BlockTest>( *this, &BlockTest::foo_, argtypes );

			ASSERT( block.arity() == 1 );
			ASSERT( block.value( 5, Array( 2 ) ).getFloat() == 10 );


			vector<GCValue> code;
			code.push_back( *new SelfMessage() );

			BlockProto<Function>& block2 = *new BlockProto<Function>( code, argtypes, locals );

			ASSERT( block2.arity() == 1 );
			ASSERT( block2.value( 5, Array( 2 ) ).getFloat() == 5 );

			cout << "------------------------------------------------------------" << endl;
		}

	};

}

