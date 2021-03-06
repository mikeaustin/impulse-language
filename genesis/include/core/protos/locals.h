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

		LocalsProto( Frame& proto ) : Frame( proto ) { }

		virtual string inspect( const Value self ) const
		{
			return Frame::inspect( self, "locals" );
		}

	};

	typedef LocalsProto& Locals;

}

#endif

