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

		static Value foo_( Value self, const Array& args )
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

			static vector<ArgType> argtypes;
			argtypes.push_back( ArgType( SymbolProto::at( "n" ), NumberProto::instance() ) );

			Function3& block = *new Function3( &foo_, argtypes );

			ASSERT( block.arity() == 1 );
			ASSERT( block.value( 5, Array( 2 ).self( 5 ) ).getFloat() == 10 );

			vector<Expression> code( 1 );
			code.back().push_back( *new SelfMessage() );

			Block2& block2 = *new Block2( code, argtypes, locals );

			ASSERT( block2.arity() == 1 );
			ASSERT( Block2::value_( block2, Array( 2 ).self( 5 ) ).getFloat() == 5 );
			ASSERT( block2.value( block2, Array( 2 ).self( 5 ) ).getFloat() == 5 );

			cout << "------------------------------------------------------------" << endl;
		}

	};

}

