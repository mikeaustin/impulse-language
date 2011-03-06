//
// impulse.h
//
// Copyright 2008-2010 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_H
#define IMPULSE_H

#define TRACE( expr ) std::cout << &spaces[60 - indent] << expr << std::endl
#define ENTER( expr ) std::cout << &spaces[60 - indent] << "> " << expr << std::endl; indent += 4
#define LEAVE( expr ) indent -= 4; std::cout << &spaces[60 - indent] << "< " << expr << std::endl

namespace impulse {

	extern const char spaces[];
	extern int        indent;
	
	class SymbolProto;
	typedef SymbolProto &Symbol;

}

#endif

