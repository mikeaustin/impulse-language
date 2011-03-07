//
// tests/block.cpp
//
// Copyright 2008-2011 Mike Austin
// All rights reserved.
//

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

		static Value foo( Value receiver, const Array& args, Value context )
		{
			return args[Index::_0].getFloat() * 2;
		}
		
		void testBlock()
		{
			using namespace std;
			
			cout << "\nTesting Block..." << endl;
			cout << "------------------------------------------------------------" << endl;

			Value block = *new BlockProto( foo, 1 );
			Array args; args.push_back( 150 );

			ASSERT( Value( 10 ).apply( args ).getFloat() == 10 );
			ASSERT( block.apply( args ).getFloat() == 300 );

			cout << "------------------------------------------------------------" << endl;
		}

	};

}

