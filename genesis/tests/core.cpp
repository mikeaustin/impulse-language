//
// tests/core.cpp
//
// Copyright 2008-2011 Mike Austin
// All rights reserved.
//

namespace impulse {

 //
 // class CoreTest
 //

	class CoreTest : public UnitTest {
	
	 public:
	 
		virtual void run()
		{
			testGC();
			testValue();
			testSlots();
		}
		
		void testGC()
		{
			using namespace std;

			cout << "\nTesting GC..." << endl;
			cout << "------------------------------------------------------------" << endl;

			Frame::ReleasePool releasePool;

			{
				Frame::ReleasePool releasePool;

				GCValue value = Frame::create();

				ASSERT( releasePool.size() == 1 );
				ASSERT( releasePool.depth() == 3 );

				ASSERT( value.getFloat() == std::numeric_limits<double>::max() );
				ASSERT( value.getFrame().referenceCount() == 2 );

				std::vector<GCValue> vector; vector.push_back( value );
				
				ASSERT( value.getFrame().referenceCount() == 3 );
				
				value = Value();

				ASSERT( value.getFloat() != std::numeric_limits<double>::max() );
				ASSERT( value.getFrame().referenceCount() == 1 );
			}

			ASSERT( releasePool.size() == 0 );
			ASSERT( releasePool.depth() == 2 );

			cout << "------------------------------------------------------------" << endl;
		}

		void testValue()
		{
			using namespace std;

			cout << "\nTesting Value..." << endl;
			cout << "------------------------------------------------------------" << endl;

			ASSERT( Value( 5.0f ).getFloat() == Value( 5.0f ).getFloat() );
			ASSERT( Value( 5.0f ).getFloat() != Value( 10.0f ).getFloat() );

			Value a, b = 5.0f;

			ASSERT( &b.getFrame() == &NumberValue::instance() );
			ASSERT( &b.getFrame().getProto() == &NumberProto::instance() );

			a = b;

			ASSERT( &a.getFrame() == &b.getFrame() );
			ASSERT( a.getFloat() == b.getFloat() );

			cout << "------------------------------------------------------------" << endl;
		}

		void testSlots()
		{
			using namespace std;

			cout << "\nTesting Slots..." << endl;
			cout << "------------------------------------------------------------" << endl;

			const Symbol foo = SymbolProto::at( "foo" );

			ASSERT( &foo == &SymbolProto::at( "foo" ) );

			Frame& frame = Frame::create( VoidProto::instance() );
			
			frame.setSlot( foo, 10 );
			ASSERT( frame.getSlot( foo ).getFloat() == 10 );

			frame.setSlot( "foo", 20 );
			ASSERT( frame.getSlot( "foo" ).getFloat() == 20 );

			ASSERT( &frame.getProto() == &VoidProto::instance() );

			cout << "------------------------------------------------------------" << endl;
		}
		
	};

}

