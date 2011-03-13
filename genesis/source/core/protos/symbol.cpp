//
// core/protos/symbol.cpp
//
// Copyright 2008-2011 Mike Austin
// All rights reserved.
//

#include "core/frame.h"
#include "core/protos/symbol.h"

namespace impulse {

 //
 // class SymbolProto
 //

	Frame::SymbolId        SymbolProto::_nextId = 0;
	SymbolProto::SymbolMap SymbolProto::_symbolMap;

}

