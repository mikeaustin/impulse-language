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

			Value lobby = Lobby::instance();

			{
				ASSERT( val( 5.0f ) == val( 5.0f ) );
				ASSERT( lobby.setSlot( Symbol::at( "x" ), 3 ) == 3 );
				ASSERT( lobby.getSlot( Symbol::at( "x" ) ) == 3 );
			}
			
			{
				OUTPUT( lobby.setSlot( Symbol::at( "*" ), Symbol::at( "*" ) ) );
				ASSERT( Symbol::at( "*" )._refCount == 3 );
				
				OUTPUT( lobby.setSlot( Symbol::at( "*" ), 5 ) );
				ASSERT( Symbol::at( "*" )._refCount == 2 );

				lobby.setSlot( Symbol::at( "+" ), Void::instance() );
				lobby.setSlot( Symbol::at( "+" ), *new Frame() );
			}

			{
				Array args( 1 ); args[0] = 5;
		
				ASSERT( Value( 3 ).send( Symbol::at( "*" ), args, lobby ) == 15 );
			}

			{
				Array args( 1 ); args[0] = 5;
				
				Message message( Symbol::at( "*" ), args );
				Array msgArgs;

				ASSERT( message.eval( val( 3 ), msgArgs, lobby ) == 15 );
			}

			{
				lobby.setSlot( Symbol::at( "x" ), 3 );

				Array args1;
				Array args2( 1 ); args2[0] = 5;

				vector<Value> messages;

				messages.push_back( *new Message( Symbol::at( "x" ), args1 ) );
				messages.push_back( *new Message( Symbol::at( "*" ), args2 ) );
				//messages.push_back( *new MulMessage( args2 ) );
				Frame& expression = *new Expression( messages );
				Block& block = *new Block( args1, expression, lobby );

				const Array exprArgs;

				ASSERT( block.eval_( lobby, exprArgs, lobby ) == 15 );
#ifndef DEBUG
				if (1)
				{
					for (int i = 0; i < 100000000; i++)
					{
						block.eval( lobby, exprArgs, lobby );
					}
				}

				if (0)
				{
					ASSERT( (block._eval3)( &block, 1, lobby, exprArgs, lobby ) == 15 );
					(block._eval3)( &block, 100000000, lobby, exprArgs, lobby );
				}
#endif
			}

			cout << "------------------------------------------------------------" << endl;
			cout << endl;
		}

	};

}

