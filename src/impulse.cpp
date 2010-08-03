//
// impulse.cpp
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#include <fstream>

#include "impulse.h"
#include "core/all.h"
#include "runtime/all.h"

#include "core/value.cpp"
#include "core/frame.cpp"
#include "core/protos.cpp"

#include "parser/all.h"

#include "tests/core.h"
#include "tests/number.h"

//#include "addons/opengl/opengl.h"

using namespace std;
using namespace impulse;

int main( int argc, char* argv[] )
{
	Lexer lexer( argc > 1 ? *new fstream( argv[1] ) : cin );
	Parser parser( lexer );

	if (lexer.stream() == cin)
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
	}
	
	Object::instance().initSlots();
	GCArray::instance().initSlots();
	Boolean::instance().initSlots();
	Number::instance().initSlots();
	String::instance().initSlots();
	Range::instance().initSlots();
	Block::instance().initSlots();

	//CoreTest().runTest();
	//NumberTest().runTest();

	Value lobby = Lobby::instance();

	//lobby.setSlot( Symbol::at( "gl" ), OpenGL::instance() );

	Expression expr;
	const static Array args;

	do
	{
		if (lexer.stream() == cin) cout << "] ";
		expr = parser.parseStatement( true );

		Value result = expr.eval( lobby, args, lobby );

		if (&result.getFrame() != &Void::instance())
		{
			if (lexer.stream() == cin) cout << "= ";
			if (lexer.stream() == cin) cout << result.inspect() << endl;
		}
		
		vector<Frame*>::iterator iter = Frame::_releasePool.begin();
		while (iter != Frame::_releasePool.end())
		{
			(*iter)->decRef();
				
			iter = Frame::_releasePool.erase( iter );
		}
	}
	while (!exitMainLoop);

	return 0;
}

