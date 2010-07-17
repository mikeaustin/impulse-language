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

#include "core/value.cpp"
#include "core/frame.cpp"

#include "tests/core.h"
#include "tests/number.h"

#include "parser/all.h"

int main()
{
	cout << "------------------------------------------------------------" << endl;
	cout << "Impulse 0.6 — Copyright 2008-2010 Mike Austin" << endl;

	cout << endl;
	
	cout << "sizeof( Value )    = " << sizeof( Value )    << endl;
	cout << "sizeof( Frame )    = " << sizeof( Frame )    << endl;
	cout << "sizeof( SymbolId ) = " << sizeof( SymbolId ) << endl;
	cout << "sizeof( SlotMap )  = " << sizeof( SlotMap )  << endl;
	cout << "sizeof( Array )    = " << sizeof( Array )    << endl;

	cout << endl;

	Object::instance().initSlots();
	Array::instance().initSlots();
	GCArray::instance().initSlots();
	Lambda::instance().initSlots();

	//CoreTest().runTest();
	NumberTest().runTest();

	Value lobby = *new Lobby();

	Lexer lexer( cin );
	Parser parser( lexer );

	Expression expr;
	Array args;

	while (!exitMainLoop)
	{
		cout << "] ";
		expr = parser.parseStatement( true );

		Value result = expr.eval( lobby, args, lobby );

		if (&result.getFrame() != &Void::instance())
		{
			cout << "= ";
			cout << result.inspect() << endl;
		}
		
		vector<Frame*>::iterator iter = Frame::_releasePool.begin();
		while (iter != Frame::_releasePool.end())
		{
			(*iter)->decRef();
				
			iter = Frame::_releasePool.erase( iter );
		}
	}

	return 0;
}

