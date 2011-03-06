//
// core/protos/void.h
//
// Copyright 2008-2011 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_VOID_H
#define IMPULSE_VOID_H

#include "core/frame.h"

namespace impulse {

 //
 // class Void
 //

	class VoidProto : public Frame {

	 public:

		VoidProto() { TRACE( "VoidProto::VoidProto()" ); }

		static Frame& instance()
		{
			TRACE( "VoidProto::instance()" );
			
			static VoidProto void_;

			return void_;
		}

		virtual string inspect( const Value receiver ) const { return "void"; }

	};

}

#endif

