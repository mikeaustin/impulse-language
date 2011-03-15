//
// core/protos/object.h
//
// Copyright 2008-2011 Mike Austin
// All rights reserved.
//

#ifndef IMPULSE_OBJECT_H
#define IMPULSE_OBJECT_H

#include "core/frame.h"
#include "core/array.h"

#include "core/protos/symbol.h"
#include "core/protos/block.h"
#include "core/protos/method.h"

namespace impulse {

 //
 // class ObjectProto
 //

	class ObjectProto : public Frame {

	 public:

	 //
	 // Lifecycle
	 //

		static ObjectProto& instance()
		{
			static ObjectProto number;

			return number;
		}

		static void initSlots()
		{
			static std::vector<ArgType> noArgs;

			instance().setSlot( "version", 0.2 );
			instance().setSlot( "proto", *new MethodProto( *new BlockProto<ObjectProto>( instance(), &ObjectProto::proto, noArgs ) ) );
		}

	 //
	 // Inspection
	 //

		virtual string inspect( const Value self ) const
		{
			return Frame::inspect( self, "object" );
		}

	 //
	 // Methods
	 //

		Value proto( Value self, const Array& args, Value locals )
		{
			return self.getFrame().getProto();
		}

	};
	
}

#endif

