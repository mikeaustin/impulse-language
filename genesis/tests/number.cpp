//
// tests/number.cpp
//
// Copyright 2008-2011 Mike Austin
// All rights reserved.
//

namespace impulse {

 //
 // class NumberTest
 //

	class NumberTest : public UnitTest {
	
	 public:

		virtual void run()
		{
			testNumber();
		}

		void testNumber()
		{
			cout << "\nTesting Number..." << endl;
			cout << "------------------------------------------------------------" << endl;

			Frame& locals = Frame::create();

			// TODO: Does perform() need locals?
			ASSERT( Value( 5 ).perform( "pow", Array( 5, 2 ), locals ).getFloat() == 25 );
			ASSERT( Value( 5 ).perform( "sin", Array( 5 ), locals ).getFloat() == std::sin( 5 ) );

			cout << "------------------------------------------------------------" << endl;
		}
		
	};

}

