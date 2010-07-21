//
// tests/number.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

namespace impulse {

	class NumberTest : public UnitTest {

	 public:
	 
		void runTest()
		{
			cout << "Testing Number..." << endl;
			cout << "------------------------------------------------------------" << endl;

			Frame lobby;

			{
				Array args( 1 ); args[0] = 5;
				ASSERT( Number::mul_( val( 3 ), args, lobby ) == 15 );
			}
			
			{
				Array args( 1 ); args[0] = 5;
				ASSERT( val( 3 ).send( Symbol::at( "*" ), args, lobby ) == 15 );
			}
			
			{
				Array args( 1 ); args[0] = 5;
				Frame& message = *new Message( Symbol::at( "*" ), args );
				ASSERT( message.eval( val( 3 ), args, lobby ) == 15 );
			}
			
			cout << "------------------------------------------------------------" << endl;
			cout << endl;
		}

	};

}

