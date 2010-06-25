//
// tests/core.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

namespace impulse {

	class CoreTest : public UnitTest {

	 public:
	 
		void runTest()
		{
			cout << "Testing Core..." << endl;
			cout << "------------------------------------------------------------" << endl;

			Frame lobby;

			{
				ASSERT( val( 5.0f ) == val( 5.0f ) );
				ASSERT( lobby.setSlot( x, 3 ) == 3 );
				ASSERT( lobby.getSlot( x ) == 3 );
			}
			
			{
				OUTPUT( lobby.setSlot( mul.getId(), mul ) );
				ASSERT( mul._refCount == 2 );
				
				OUTPUT( lobby.setSlot( mul.getId(), 5 ) );
				ASSERT( mul._refCount == 1 );

				lobby.setSlot( add.getId(), Void::instance() );
				lobby.setSlot( add.getId(), *new Frame() );
			}

			{
				Array args( 1 ); args[0] = 5;
		
				ASSERT( Value( 3 ).send( mul, args, lobby ) == 15 );
			}

			{
				Array args( 1 ); args[0] = 5;
				
				Message message( mul, args );
				Array msgArgs;

				ASSERT( message.eval( val( 3 ), msgArgs, lobby ) == 15 );
			}

			{
				lobby.setSlot( x, 3 );

				Array args( 1 ); args[0] = 5;

				vector<Value> messages;

				messages.push_back( *new Message( x, args ) );
				messages.push_back( *new Message( mul, args ) );
				//messages.push_back( *new MulMessage( args ) );
				Frame& expression = *new Expression( messages );
				Frame& block = *new Lambda( expression );

				const Array exprArgs;

				//cout << expression.eval( lobby, exprArgs, lobby ).getFloat() << endl;
				ASSERT( (block._eval2)( &block, lobby, exprArgs, lobby ) == 15 );
#ifndef DEBUG
				for (int i = 0; i < 100000000; i++)
				{
					//(expression._eval2)( &expression, lobby, exprArgs, lobby );
					(block._eval2)( &block, lobby, exprArgs, lobby );
					//block.eval( lobby, exprArgs, lobby );
				}
#endif
			}

			cout << "------------------------------------------------------------" << endl;
			cout << endl;
		}

	};

}
