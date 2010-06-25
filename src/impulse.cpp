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

#include "tests/core.h"
#include "tests/number.h"

int main()
{
	cout << "------------------------------------------------------------" << endl;
	cout << "Impulse 0.6 — Copyright 2008-2010 Mike Austin" << endl;
	
	cout << "sizeof( Value )    = " << sizeof( Value ) << endl;
	cout << "sizeof( Frame )    = " << sizeof( Frame ) << endl;
	cout << "sizeof( SymbolId ) = " << sizeof( SymbolId ) << endl;

	cout << endl;

	CoreTest().runTest();
	NumberTest().runTest();

	cout << "------------------------------------------------------------" << endl;
		
	return 0;
}

