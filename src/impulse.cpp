//
// impulse.cpp
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#include "impulse.h"
#include "core/all.h"
#include "runtime/all.h"

using namespace std;
using namespace impulse;
using namespace impulse::runtime;

#include "core/value.cpp"
#include "core/frame.cpp"

int main()
{
	cout << "------------------------------------------------------------" << endl;
	cout << "Impulse 0.6 — Copyright 2008-2010 Mike Austin" << endl;
	
	cout << "sizeof( Value )    = " << sizeof( Value ) << endl;
	cout << "sizeof( Frame )    = " << sizeof( Frame ) << endl;
	cout << "sizeof( SymbolId ) = " << sizeof( SymbolId ) << endl;

	Frame lobby;
	lobby.setSlot( x.getId(), 3 );

	if (0)
	{
		Value value( 3 );
	
		Array args( 1 );
		args[0] = 5;

		cout << value.send( mul.getId(), args, lobby ).getFloat() << endl;

		for (int i = 0; i < 100000000; i++)
		{
			value.send( mul.getId(), args, lobby );
		}
	}

	if (0)
	{
		Value value( 3 );
		Array msgArgs( 1 );
		msgArgs[0] = 5;
		Message message( mul, msgArgs );
		Array args;

		cout << message.eval( value, args, lobby ).getFloat() << endl;

		for (int i = 0; i < 100000000; i++)
		{
			message.eval( value, args, lobby );
		}
	}

	if (1)
	{
		vector<Value> messages;
		Array msgArgs( 1 );
		msgArgs[0] = 5;
	
		//messages.push_back( 3 );
		messages.push_back( *new Message( x, msgArgs ) );
		
		messages.push_back( *new Message( mul, msgArgs ) );
		//messages.push_back( *new MulMessage( msgArgs ) );
		Frame& expression = *new Expression( messages );
		Frame& block = *new Lambda( expression );
		const Array exprArgs;

		//cout << expression.eval( lobby, exprArgs, lobby ).getFloat() << endl;
		cout << (block._eval2)( &block, lobby, exprArgs, lobby ).getFloat() << endl;
#ifndef DEBUG
		for (int i = 0; i < 100000000; i++)
		{
			//expression.eval( lobby, exprArgs, lobby );
			(block._eval2)( &block, lobby, exprArgs, lobby );
			//block.eval( lobby, exprArgs, lobby );
		}
#endif
	}

	if (0)
	{
		cout << "------------------------------------------------------------" << endl;
		lobby.setSlot( mul.getId(), mul );
		lobby.setSlot( mul.getId(), 5 );
		lobby.setSlot( add.getId(), Void::instance() );
		lobby.setSlot( add.getId(), *new Frame() );
	}

	cout << "------------------------------------------------------------" << endl;
		
	return 0;
}

