//
// tests/frame.cpp
//
// Copyright 2008-2011 Mike Austin
// All rights reserved.
//

namespace impulse {

 //
 // class FrameTest
 //

	class FrameTest : public UnitTest {
	
	 public:

		FrameTest() : _frame( Frame::create() ) { }

		virtual void run()
		{
			testFrame();
		}

		Value foo_( Value self, const Array& args, Value locals )
		{
			ASSERT( &self.getFrame() == &_frame );
			
			return args[Index::_0].getFloat() * 2;
		}

		void testFrame()
		{
			cout << "\nTesting Frame..." << endl;
			cout << "------------------------------------------------------------" << endl;

			Frame& locals = Frame::create();
			
			static std::vector<ArgType> argtypes;
			argtypes.push_back( ArgType( SymbolProto::at( "n" ), NumberProto::instance() ) );

			_frame.setSlot( "foo", *new MethodProto( *new BlockProto<FrameTest>( *this, &FrameTest::foo_, argtypes ) ) );

			ASSERT( _frame.perform( SymbolProto::at( "foo" ), Array( 5, 5 ), locals ).getFloat() == 10 );
			ASSERT( &Value( 5 ).perform( "proto", Array(), locals ).getFrame() == &NumberProto::instance() );

			cout << "------------------------------------------------------------" << endl;
		}

	 private:
	 
		Frame& _frame;
		
	};

}

