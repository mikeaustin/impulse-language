//
// core/protos/locals.h
//
// Copyright 2008-2011 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_LOCALS_H
#define IMPULSE_LOCALS_H

#include "core/frame.h"
#include "core/array.h"

namespace impulse {

 //
 // class LocalsProto
 //

	class LocalsProto : public Frame {

	 public:

		LocalsProto( GCValue selfContext ) : _selfContext( selfContext ) { }

		virtual string inspect( const Value self ) const
		{
			return Frame::inspect( self, "locals" );
		}

		GCValue selfContext() { return _selfContext; }

	 private:
	 
		GCValue _selfContext;

	};

	typedef LocalsProto& Locals;

}

#endif

